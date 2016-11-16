/* IBM has disclaimed copyright on this module.  */

/***************************************************************/
/*                                                             */
/* Function: hftctl                                            */
/*                                                             */
/* Syntax:                                                     */
/*    #include <sys/ioctl.h>                                   */
/*    #include <sys/hft.h>                                     */
/*                                                             */
/*    int hftctl(fildes, request, arg )                        */
/*    int fildes, request;                                     */
/*    char *arg;                                               */
/*                                                             */
/* Description:                                                */
/*                                                             */
/*    Does the following:                                      */
/*      1. determines if fildes is pty                         */
/*         does normal ioctl it is not                         */
/*      2. places fildes into raw mode                         */
/*      3. converts ioctl arguments to datastream              */
/*      4. waits for 2 secs for acknowledgement before         */
/*         timing out.                                         */
/*      5. places response in callers buffer ( just like       */
/*         ioctl.                                              */
/*      6. returns fildes to its original mode                 */
/*                                                             */
/*    User of this program should review steps 1,4, and 3.     */
/*    hftctl makes no check on the request type. It must be    */
/*    a HFT ioctl that is supported remotely.                  */
/*    This program will use the SIGALRM and alarm(2).  Any     */
/*    Previous alarms are lost.                                */
/*                                                             */
/*    Users of this program are free to modify it any way      */
/*    they want.                                               */
/*                                                             */
/* Return Value:                                               */
/*                                                             */
/*    If ioctl fails, a value of -1 is returned and errno      */
/*    is set to indicate the error.                            */
/*                                                             */
/***************************************************************/

#include <config.h>
#include <sys/signal.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <setjmp.h>
/* get the Darwin <sys/ioctl.h> to include some compatibility code: */
#ifndef USE_OLD_TTY
# define USE_OLD_TTY 1
#endif /* !USE_OLD_TTY */
#include <sys/ioctl.h>
#ifdef HAVE_SYS_DEVINFO_H
# include <sys/devinfo.h>
#else
# if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#  warning "hftctl.c expects <sys/devinfo.h> to be included."
# endif /* __GNUC__ && !__STRICT_ANSI__ */
/* guess: */
struct devinfo;
#endif /* HAVE_SYS_DEVINFO_H */
#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#else
# ifdef HAVE_MACH_MACH_TYPES_H
#  include <mach/mach_types.h>
# endif /* HAVE_MACH_MACH_TYPES_H */
#endif /* HAVE_SYS_TYPES_H */
#ifdef HAVE_MACHINE_TYPES_H
# include <machine/types.h>
#endif /* HAVE_MACHINE_TYPES_H */
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif /* HAVE_UNISTD_H */
#include <termios.h>
#ifdef HAVE_TERMIO_H
# include <termio.h>
#else
# ifdef HAVE_TERM_H
#  include <term.h>
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "hftctl.c expects <termio.h> or <term.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
# endif /* HAVE_TERM_H */
/* copy-and-paste a copy of parts of "../mac/inc/termio.h" here,
 * just in case: */
# ifndef _SYS_TERMIO_H
#  define _SYS_TERMIO_H 1
#  ifndef NCCS
#   define NCCS 32
#  endif /* !NCCS */
/* c_cc subscript names: */
#  ifndef VINTR
#   define VINTR 1
#  endif /* !VINTR */
#  ifndef VQUIT
#   define VQUIT 2
#  endif /* !VQUIT */
#  ifndef VERASE
#   define VERASE 3
#  endif /* !VERASE */
#  ifndef VTIME
#   define VTIME 4
#  endif /* !VTIME */
#  ifndef VMIN
#   define VMIN 5
#  endif /* !VMIN */
/* c_iflag fields: */
#  ifndef IGNBRK
#   define IGNBRK 0x1	/* ignore break condition */
#  endif /* !IGNBRK */
#  ifndef ICRNL
#   define ICRNL 0x2	/* map CR to NL on input */
#  endif /* !ICRNL */
#  ifndef IXON
#   define IXON 0x4	/* enable start/stop output control */
#  endif /* !IXON */
/* c_oflag fields: */
#  ifndef ONLCR
#   define ONLCR 0x1	/* map CR to NL on output */
#  endif /* !ONLCR */
#  ifndef TABDLY
#   define TABDLY 0x2	/* horizontal tab delays */
#  endif /* !TABDLY */
#  ifndef TAB3
#   define TAB3 0x4	/* expand tab to spaces */
#  endif /* !TAB3 */
/* c_cflag fields: */
#  ifndef CBAUD
#   define CBAUD 0x1
#  endif /* !CBAUD */
#  ifndef B9600
#   define B9600 0x2
#  endif /* !B9600 */
/* c_lflag fields: */
#  ifndef ISIG
#   define ISIG 0x1	/* enable signals */
#  endif /* !ISIG */
#  ifndef ICANON
#   define ICANON 0x2	/* canonical input (erase and kill processing) */
#  endif /* !ICANON */
#  ifndef ECHO
#   define ECHO 0x3	/* enable echo */
#  endif /* !ECHO */
/* 2 extra things: */
#  ifndef TCSETAW
#   define TCSETAW 4
#  endif /* !TCSETAW */
#  ifndef TCSETAF
#   define TCSETAF 5
#  endif /* !TCSETAF */
# endif /* _SYS_TERMIO_H */
#endif /* HAVE_TERMIO_H */
#ifdef HAVE_SYS_HFT_H
# include <sys/hft.h>
#else
# if 1
/* found one! */
#  include "sys/hft.h"
# else
#  if defined(__GNUC__) && !defined(__STRICT_ANSI__)
#   warning "hftctl.c expects <sys/hft.h> to be included."
#  endif /* __GNUC__ && !__STRICT_ANSI__ */
/* guess at what these are supposed to be: */
struct hfbuf {
    int hf_buflen;
    char *hf_bufp;
};
struct hfquery {
    int hf_cmdlen;
    char *hf_cmd;
    int hf_resplen;
    char *hf_resp;
};
/* also need to figure out what the order is for the names of the fields
 * for 'struct hfctlack' and 'struct hfctlreq'... */
/* just forward-declare them for now: */
struct hfctlack;
struct hfctlreq;
struct hfintro;
# endif /* 1 */
#endif /* HAVE_SYS_HFT_H */
#include <sys/uio.h>
#include <sys/tty.h>
#ifdef HAVE_SYS_PTY_H
# include <sys/pty.h>
#else
# ifdef HAVE_PTY_H
#  include <pty.h>
# endif /* HAVE_PTY_H */
#endif /* HAVE_SYS_PTY_H */

#define REMOTE 0x01

#undef ioctl
static const char     SCCSid[] = "com/gnuemacs/src,3.1,9021-90/05/03-5/3/90";

/*************** LOCAL DEFINES **********************************/

#define QDEV   ((HFQPDEVCH << 8) | HFQPDEVCL)
#define QLOC   ((HFQLOCCH << 8) | HFQLOCCL)
#define QPS    ((HFQPRESCH << 8) | HFQPRESCL)

/* definition of TCGETA taken from "../mac/inc/sys/ioctl.h"; and TCSETA is
 * just that incremented by 1. */
#ifndef TCGETA
# define TCGETA 2
#endif /* !TCGETA */
#ifndef TCSETA
# define TCSETA 3
#endif /* !TCSETA */
#ifndef TCGETS
# define TCGETS TCGETA
#endif /* !TCGETS */
#ifndef TCSETS
# define TCSETS TCSETA
#endif /* !TCSETS */

/*************** EXTERNAL / GLOBAL DATA AREA ********************/

#if defined(PROTOTYPES) || defined(__PROTOTYPES)
static int              hfqry(int fd, int request, struct hfquery *arg);
static int              hfskbd(int fd, int request, struct hfbuf *arg);
       void             xfree(void *block);
       void            *xmalloc(size_t n);
       ptrdiff_t        emacs_read(int fildes, void *buf, ptrdiff_t nbyte);
       ptrdiff_t emacs_write(int fildes, void const *buf, ptrdiff_t nbyte);
#else
static int              hfqry();
static int              hfskbd();
       void             xfree();
       char            *xmalloc();
       ptrdiff_t        emacs_read();
       ptrdiff_t        emacs_write();
#endif /* PROTOTYPES || __PROTOTYPES */

#ifndef errno
extern int              errno;
#endif /* !errno */
static jmp_buf          hftenv;
static int              is_ack_vtd;

#ifndef RETSIGTYPE
# define RETSIGTYPE void
#endif /* !RETSIGTYPE */
#ifndef SIGTYPE
# define SIGTYPE RETSIGTYPE
#endif /* !SIGTYPE */
#if defined(PROTOTYPES) || defined(__PROTOTYPES)
static SIGTYPE             (*sav_alrm) (int);
#else
static SIGTYPE             (*sav_alrm) ();
#endif /* PROTOTYPES || __PROTOTYPES */
/* no idea what these are actually supposed to be: */
#ifndef HFCTLREQCH
# define HFCTLREQCH 0
#endif /* !HFCTLREQCH */
#ifndef HFCTLREQCL
# define HFCTLREQCL 0
#endif /* !HFCTLREQCL */
static struct hfctlreq  req =
                  { 0x1b, '[', 'x', 0, 0, 0, 21, HFCTLREQCH, HFCTLREQCL };
/* likewise: */
#ifndef HFCTLACKCH
# define HFCTLACKCH 0
#endif /* !HFCTLACKCH */
#ifndef HFCTLACKCL
# define HFCTLACKCL 0
#endif /* !HFCTLACKCL */
static struct hfctlack  ACK =
                  { 0x1b, '[', 'x', 0, 0, 0, 21, HFCTLACKCH, HFCTLACKCL };

       /* FUNC             signal(); */

/*************** LOCAL MACROS ***********************************/

#define HFTYPE(p)   ((p->hf_typehi<<8)|(p->hf_typelo))

#define BYTE4(p)    ((p)[0]<<24 | (p)[1]<<16 | (p)[2]<<8 | (p)[3])

					/* read a buffer:       */
#define RD_BUF(f,p,l) \
        while ((l)) \
          if ((j = emacs_read (f, p, l)) < 0) \
             if (errno != EINTR) return (-1); \
             else continue; \
          else { (l) -= j; (p) += j; }

/*************** function prototypes ***************************/
#if defined(PROTOTYPES) || defined(__PROTOTYPES)
static int GT_ACK(int fd, int req, char *buf);
static int WR_REQ(int fd, int request, int cmdlen, char *cmd, int resplen);
static void hft_alrm(int sig);
#else
static GT_ACK ();
static WR_REQ ();
static void hft_alrm ();
#endif /* PROTOTYPES || __PROTOTYPES */

/*************** HFTCTL FUNCTION *******************************/

int hftctl (fd, request, arg)
     int     fd;
     int     request;
     union {
       struct hfintro *intro;
       struct hfquery *query;
       char           *c;
     } arg;
{

  int             i;
  int             fd_flag;	/* fcntl flags          */
  register union {
    struct hfintro         *cmd; /* p.cmd - intro des.   */
    struct hfqphdevc       *ph;	/* p.ph  - physical dev.*/
    char            *c;		/* p.c   - char ptr     */
  }               p;		/* general pointer      */
  int             pty_new;	/* pty modes            */
  int             pty_old;
  int             retcode;
  struct termios   term_new;	/* terminal attributes  */
  struct termios   term_old;
  struct devinfo	devInfo; /* defined in sys/devinfo.h */

#ifndef IOCINFO
# define IOCINFO 0xff01
#endif /* !IOCINFO */
  if (ioctl (fd, IOCINFO, &devInfo) == -1) return(-1);

  if (devInfo.devtype != DD_PSEU) /* is it a pty? */
    return (ioctl(fd, request, arg)); /* no, do IOCTL */

  /******* START PTY **************/
  /**  Pty found, possible HFT    */
  /** set new file des as raw     */
  /** as you can.                 */
  /********************************/

  /* Get current state of file    */
  /* descriptor & save            */
  if ((fd_flag = fcntl (fd, F_GETFL, 0)) == -1) return (-1);
  if (ioctl (fd, TCGETS, &term_old) == -1) return (-1);
  /* set terminal attr to raw     */
  /* and to delay on read         */
  pty_new = pty_old | REMOTE;
  memcpy (&term_new, &term_old, sizeof (term_new));
  term_new.c_iflag = 0;
  term_new.c_oflag = 0;
  term_new.c_lflag = 0;
  /* term_new.c_line  = 0; */
  for (i = 1; i <= 5; i++)
    term_new.c_cc[i] = 0;
  term_new.c_cc[0] = -1;
  ioctl (fd, TCSETS, &term_new);
  if (fcntl (fd, F_SETFL, fd_flag & ~O_NDELAY) == -1)
    return(-1);
  /* This define is probably wrong: */
#ifndef HFSKBD
# define HFSKBD 0
#endif /* !HFSKBD */
  /* call spacific function       */
  if (request == HFSKBD)
    retcode = hfskbd (fd, request, arg.c);
  else				/* assume HFQUERY */
    retcode = hfqry (fd, request, arg.c);

  fcntl (fd, F_SETFL, fd_flag); /* reset terminal to original   */
  ioctl (fd, TCSETS, &term_old);


  return (retcode);             /* return error                 */
}

/*************** HFSKBD  FUNCTION ******************************/
static int
hfskbd (fd, request, arg)
        int     fd;
        int     request;
        struct hfbuf *arg;
{
  WR_REQ(fd, request, arg->hf_buflen, arg->hf_bufp, 0);
  return (GT_ACK(fd, request, arg->hf_bufp));
}

/*************** HFQUERY FUNCTION ******************************/
static int
hfqry (fd, request, arg)
        int     fd;
        int     request;
        struct hfquery *arg;
{
  WR_REQ(fd, request, arg->hf_cmdlen, arg->hf_cmd, arg->hf_resplen);
  return (GT_ACK(fd, request, arg->hf_resp));
}


/*************** GT_ACK FUNCTION ******************************/
static int
GT_ACK (fd, req, buf)
        int     fd;
        int     req;
        char   *buf;
{
  struct hfctlack ack;
  int             i = sizeof (ack);
  int             j = 0;
  union {
    char            *c;
    struct hfctlack *ack;
  }               p;

  is_ack_vtd = 0;		/* flag no ACT VTD yet         */

  if (setjmp (hftenv))		/* set environment in case     */
    {				/* of time out                 */
      errno = ENODEV;		/* if time out, set errno      */
      return (-1);		/* flag error                  */
    }

  alarm(3);			/* time out in 3 secs          */
  sav_alrm = signal (SIGALRM, hft_alrm); /* prepare to catch time out   */

  p.ack = &ack;
  while (! is_ack_vtd)		/* do until valid ACK VTD      */
    {
      RD_BUF(fd, p.c, i);	/* read until a ACK VTD is fill*/

      if (! memcmp (&ack, &ACK, sizeof (HFINTROSZ)) /* the ACK intro &  */
	  && (ack.hf_request == req)) /* is it the response we want ?*/
	{			/* yes, ACK VTD found          */
	  is_ack_vtd = 1;	/* quickly, flag it            */
	  break;		/* get the %$%#@ out of here   */
	}

      p.ack = &ack;		/* no, then skip 1st           */
      ++p.c;			/* char and start over         */
      i = (sizeof(ack) - 1);	/* one less ESC to cry over    */

      while ((*p.c != 0x1b) && i) /* scan for next ESC           */
	{ ++p.c; --i; }		/* if any                      */

      (i ? memcpy (&ack, p.c, i) : 0); /* if any left over, then move */
      p.ack = &ack;		/* ESC to front of ack struct  */
      p.c += i;			/* skip over whats been read   */
      i = sizeof (ack) - i;	/* set whats left to be read   */
    }				/***** TRY AGAIN               */

  alarm(0);			/* ACK VTD received, reset alrm*/
  signal (SIGALRM, sav_alrm);	/* reset signal                */

  if (i = ack.hf_arg_len)	/* any data following ?        */
    {				/* yes,                        */
      RD_BUF(fd,buf,i);		/* read until it is received   */
    }

  if (errno = ack.hf_retcode)	/* set errno based on returned */
    return (-1);		/* code, if 0, then no error   */
  else
    return (0);			/* if set, then error returned */
}

/*************** HFT_ALRM FUNCTION ******************************/
static void
hft_alrm (sig)                  /* Function hft_alrm - handle */
     int sig;			/* alarm signal		      */
{
  signal (SIGALRM, sav_alrm);	/* reset to previous          */

  if (is_ack_vtd)		/* has ack vtd arrived ?      */
    return;			/* yes, then continue         */
  else				/* no, then return with error */
    longjmp (hftenv, -1);

}

/*********************************************************************/
/***                                                               ***/
/***  NOTE: Both the HFCTLREQ and the arg structure should be      ***/
/***        sent in one io write operation.  If terminal           ***/
/***        emulators are in NODELAY mode then multiple writes     ***/
/***        may cause bogus information to be read by the emulator ***/
/***        depending on the timing.                               ***/
/***                                                               ***/
/*********************************************************************/

static int
WR_REQ (fd, request, cmdlen, cmd, resplen)
	int             fd;
	int             request;
	int             cmdlen;
	char           *cmd;
	int             resplen;
{
  struct {
    char            *c;
    struct hfctlreq *req;
  }              p;
  int            size;

  req.hf_request = request;
  req.hf_arg_len = cmdlen;
  req.hf_rsp_len = resplen;

  if (cmdlen)			/* if arg structure to pass    */
    {
      size = sizeof (struct hfctlreq) + cmdlen;
      if ((p.c = xmalloc(size)) == NULL) /* malloc one area            */
	return (-1);

      memcpy (p.c, &req, sizeof (req)); /* copy CTL REQ struct         */
      memcpy (p.c + sizeof (req), cmd, cmdlen); /* copy arg struct     */
    }
  else
    {
      p.req = &req;		/* otherwise use only CTL REQ  */
      size = sizeof (req);
    }

  /* write request to terminal: */
  if (emacs_write (fd, p.c, size) == -1) return (-1);
  if (p.req != &req)		/* free if allocated           */
    xfree (p.c);
  return (0);

}

/* arch-tag: cfd4f3bd-fd49-44e6-9f69-c8abdf367650
   (do not change this comment) */
