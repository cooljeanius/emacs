/* @(#)55	1.36  com/inc/sys/hft.h, sysxhft, bos325, 9409B325a 1/31/94 13:17:43 */
/*
 * COMPONENT_NAME: (SYSXHFT) High Function Terminal hft.h
 *
 * FUNCTIONS:
 *
 * ORIGINS: 10, 27
 *
 * (C) COPYRIGHT International Business Machines Corp. 1985, 1989, 1992
 * All Rights Reserved
 * Licensed Materials - Property of IBM
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */


/*
 * HFT  - high function terminal
 * KEP  - keyboard emulation program
 * KSR  - keyboard send/receive mode
 * LPFK - lighted programmable function keys
 * MOM  - Monitor mode
 * SM   - Screen Manager
 * VT   - virtual terminal
*/

#ifndef _H_HFT
#define _H_HFT

#include <sys/types.h>

#ifndef HFNUMVTS
#define HFNUMVTS        16      /* maximum virtual terminals */
#define HFNUMDISP       4       /* maximum displays */
#define HFNUMFONT       32      /* maximum fonts */
#define FONTPAL         8       /* maximum fonts in font palette */
#define COLORPAL        16      /* maximum colors in color map (palette) */
#endif

/*------------------------------------------------------------------------
  VTD (virtual terminal datastream) introducer.
  ------------------------------------------------------------------------*/
#define HFINTROESC      '\033'
#define HFINTROLBR      '['
#define HFINTROEX       'x'

struct hfintro {        /* VTD stream header */
    char hf_esc;        /* esc */
    char hf_lbr;        /* [ */
    char hf_ex;         /* x */
    char hf_len[4];     /* sizeof structure the hf_intro appears in minus 3 */
    char hf_typehi;     /* major type: 1=gen, 2=ksr, 3=mom */
    char hf_typelo;     /* minor type: 00-3F=output, 40-7F=query,
                           C0-FF=query response */
};
#define HFINTROSZ       sizeof(struct hfintro)

/*------------------------------------------------------------------------
  WRITE:  Several commands can be sent to the hft in a write.
  The  format  of  these  commands includes a VTD (virtual terminal
  datastream) header (hf_intro, hf_sublen, hf_subtype) followed by
  command data.  The possible commands are listed below:
  ------------------------------------------------------------------------*/

/*------------
  set/reset keyboard LED's
  ------------*/
#define HFKLEDCH        0x01    /* high type */
#define HFKLEDCL        0x06    /* low type */
struct hfkled {                 /* set/reset keyboard LED's */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfkled)-3
                                                HFKLEDCH HFKLEDCL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */
        char hf_ledselect;              /* LED 8-bit mask:0=no chg,1=chg */
        char hf_ledvalue;               /* LED 8-bit mask:0=reset,1=set */
};
#define HFNUMLOCK       0x02
#define HFCAPSLOCK      0x04
#define HFSCROLLOCK     0x01

/*------------
  set/reset locator thresholds
  ------------*/
#define HFLOTHCH        0x01    /* high type */
#define HFLOTHCL        0x07    /* low type */
struct hfloth {                 /* set locator thresholds */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfloth)-3
                                                 HFLOTHCH HFLOTHCL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */
        char hf_hthresh[2];             /* 0-32767 .25mm horiz units */
        char hf_vthresh[2];             /* 0-32767 .25mm vert units */
};

/*------------
  set/reset LPFK lights, dials granularity
  query LPFK lights, dials granularity
  ------------*/
#define HFLPFKSCH       0x01    /* LPFKs  set - high (major) type */
#define HFLPFKSCL       0x0C    /* low (minor) type               */
#define HFDIALSCH       0x01    /* Dials set - high (major) type  */
#define HFDIALSCL       0x0D    /* low (minor) type               */
struct hfdial_lpfk {            /* set LPFK lights on/off */
                                /* set dials granularity */
                                /* response to query LPFK lights on/off */
                                /* response to query dials granularity */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfdial_lpfk)-3
                                                0x01 low */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 1 */
        union {
                char rsvd[4];           /* not used in set commands */
                char numlpfks[4];       /* query LPFK response - number */
                                        /* of LPFKs on device */
                char numdials[4];       /* query dials response - number */
                                        /* of dials on device */
        } hf_data1;
        union {
                char rsvd[4];           /* not used in query responses */
                char keys[4];           /* LPFK 32-bit mask, 0=no chg, 1=chg */
                char dials[4];          /* dial 32-bit mask, 0=no chg, 1=chg */
        } hf_mask;
        union {
                char granularity[16];   /* gran of each dial (events per
                                           360-degree revolution in powers
                                           of 2 from 2 to 8) */
                struct {                /* 1=enabled (lit), 0=disabled (off) */
                        char flags[4];  /* 32-bit mask, 1 per LPFK */
                        char rsvd[12];  /* rsvd in LPFK queries, sets */
                } lpfk;
        } hf_data2;
        char hf_rsvd[4];                /* reserved for sets & queries */
};

/*------------
  write sound duration and frequency
  ------------*/
#define HFSOUNDCH       0x01    /* high type */
#define HFSOUNDCL       0x09    /* low type */
struct hfsound {                /* write sound duration and frequency */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfsound)-3
                                                 HFSOUNDCH HFSOUNDCL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */
        char hf_mode;                   /* modes */
#define HFSIGSOUND      0x80            /* signal when sound request has been
                                           processed (played) or discarded */
#define HFEXECALWAYS    0x01            /* keep sound writes playing even if
                                           active vt changes */
        char hf_rsvd;                   /* reserved */
        char hf_dur[2];                 /* duration in 1/128 seconds */
        char hf_freq[2];                /* frequency in hertz */
};

/*------------
  cancel 'play when active' sound
  ------------*/
#define HFCANSNDCH      0x01    /* high type */
#define HFCANSNDCL      0x0A    /* low type */
struct hfcansnd {               /* cancel 'play when active' sound */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfcansnd)-3
                                                 HFCANSNDCH HFCANSNDCL */
};

/*------------
  change physical display
  ------------*/
#define HFCHGDSPCH      0x01    /* high type */
#define HFCHGDSPCL      0x16    /* low type */
struct hfchgdsp {               /* change physical display */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfchgdsp)-3
                                                 HFCHGDSPCH HFCHGDSPCL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */
        char hf_mode[2];                /* 0 = use current display */
/* bit selector for hf_mode[0] */
#define HFNONDEF        0x20            /* nondefault, use phys disp id below */
        char hf_rsvd1[8];               /* reserved */
        char hf_devid[4];               /* physical display (device) id */
        char hf_rsvd2[10];              /* reserved */
};

/*----------
  set/reset MOM (monitor mode) protocol mode:
     XLATKBD HOSTPC LPFK DIALS TABLET MOUSE JKANA
  set/reset KSR (keyboard send/receive) protocol mode:
     XLATKBD HOSTPC LPFK DIALS TABLET MOUSE JKANA HOSTS WRAP
  ----------*/
/* bit selectors for hf_select[0] below */
#define HFHOSTS         0x10            /* report keyboard shift status chgs */
#define HFXLATKBD       0x01            /* translate key position,status,
                                           scan code to ascii code */
/* bit selectors for hf_select[1] below */
#define HFWRAP          0x40            /* wrap cursor  */
#define HFLPFKS         0x20            /* enable lighted pgm func key input */
#define HFDIALS         0x10            /* enable dials input */
#define HFTABLET        0x04            /* enable tablet input */
#define HFMOUSE         0x02            /* enable mouse input */
#define HFJKANA         0x01            /* enable kana input */


#define HFKSRPROH       0x02    /* KSR protocol, high type */
#define HFKSRPROL       0x01    /* KSR protocol, low type */
#define HFMOMPROH       0x05    /* MOM protocol, high type */
#define HFMOMPROL       0x01    /* MOM protocol, low type */
struct hfprotocol {             /* set/change MOM/KSR protocol mode */
        char hf_intro[HFINTROSZ];       /* esc [ x size-3 high low */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */
        char hf_select[2];              /* ksr/mom protocol definition word */
        char hf_value[2];               /* values */
};

/*------------
  set tablet dead zones (non-input borders)
  ------------*/
#define HFTDZCH         0x01    /* high type */
#define HFTDZCL         0x08    /* high type */
struct hftdzone {               /* set tablet dead zones (non-input
                                   borders) */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hftdzone)-3
                                                 HFTDZCH HFTDZCL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */
        char hf_horizontal[2];          /* 0-32767 horizontal units */
        char hf_vertical[2];            /* 0-32767 vertical units */
};

/*------------
  set KSR color map (palette)
  ------------*/
#define HFCOLORPALH     0x02    /* high type */
#define HFCOLORPALL     0x05    /* low type */
struct hfcolorpal {             /* set KSR color map (palette) */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfcolorpal)-3
                                           HFCOLORPALH HFCOLORPALL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */
        char hf_numcolor[2];            /* # of color map entries to chg */
        /*---(repeats for each entry of color map, hf_numcolor times)---*/
        char hf_palet[COLORPAL][4];     /* adapter-specific value settings
                                           for color map (palette) entry */
};

/*------------
  Change fonts.  The font palette must be completely filled in.
  ------------*/
#define HFFONTH         0x02    /* high type */
#define HFFONTL         0x06    /* low type */
struct hffont {                 /* change font palette (lose data in PS!!) */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hffont)-3
                                                 HFFONTH HFFONTL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */
        char hf_primary[2];             /* phys font id of primary font */
        char hf_alt1[2];                /* physical font id of 1st alt */
        char hf_alt2[2];
        char hf_alt3[2];
        char hf_alt4[2];
        char hf_alt5[2];
        char hf_alt6[2];
        char hf_alt7[2];
};

/*------------
  Change cursor representation
  ------------*/
#define HFCURSORH       0x02    /* high type */
#define HFCURSORL       0x08    /* low type */
struct hfcursor {                       /* cursor representation */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfcursor)-3
                                                 HFCURSORH HFCURSORL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */
        char hf_rsvd;                   /* reserved */
        char hf_shape;                  /* cursor shape */
#define HFNONE          0               /* no cursor */
#define HFSNGLUS        1               /* single underscore */
#define HFDBLUS         2               /* double underscore */
#define HFHALFBLOB      3               /* half blob */
#define HFMIDLINE       4               /* mid character line */
#define HFFULLBLOB      5               /* full blob */
};

/*------------
  Monitor mode screen request and input ring addressability
  ------------*/
#define HFMOMREQH       0x05    /* high type */
#define HFMOMREQL       0x02    /* low type */
struct hfmomscreq {                     /* monitor mode screen request
                                           and input ring addressability */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfmomscreq)-3
                                                 HFMOMREQH HFMOMREQL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */
        char hf_ringlen[2];             /* ring buffer length in bytes,
                                           including ptrs & stat fields */
        char hf_ringoffset[4];          /* offset to the input buffer ring
                                           from hf_ringlen field */
};

#ifndef HFRDATA                 /* this define may be overridden */
#define HFRDATA         1
#endif
struct hfmomring {                      /* monitor mode ring status, ptrs */
   char  hf_rsvd[2];                    /* reserved */
   char  hf_intreq;                     /* intr request, send a SIGMSG:
                                             0x00 = when buffer becomes empty
                                             0xFF = for each input event */
   char  hf_ovflow;                     /* overflow:  0x00=normal,
                                                      0xFF=overflow */
   ulong hf_source;                     /* current sys ptr (offset=32+) */
   ulong hf_sink;                       /* current user ptr (offset=32+) */
   long  hf_unused[5];                  /* reserved */
   char  hf_rdata[HFRDATA];             /* data goes here */
};

/*----------
  screen release acknowledgement.  Application sends to VT when
  application has received a screen release interrupt.
  ----------*/
#define HFMOMRELH       0x05    /* high type */
#define HFMOMRELL       0x03    /* low type */
struct hfmomscrel {                     /* monitor mode screen rel ack */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfmomscrel)-3
                                                 HFMOMRELH HFMOMRELL */
};

/*----------
  monitor mode process ping acknowledgement.
  ----------*/
#define HFMOMPINGH      0x05    /* high type */
#define HFMOMPINGL      0x04    /* low type */
struct hfmomping {                      /* monitor mode ping ack */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfmomscrel)-3*/
};

/*----------
  request an emulated (connected through a pty ==> all communication
  with it must be through the data stream) to perform an ioctl.
  First put emulated hft in raw mode, then issue a write with the
  following vtd structure.
  ----------*/
#define HFCTLREQCH      0x04    /* high */
#define HFCTLREQCL      0x00    /* low */
struct hfctlreq {               /* emulated ioctl request */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfctlreq)-3
                                                 HFCTLREQCH HFCTLREQCL */
                                        /* Next 3 fields always ignored */
        char hf_sublen;                 /* sub header length = 3 */
        char hf_subtype;                /* sub header type = 0 */
        char hf_align ;
        long hf_request ;               /* ioctl request type */
        long hf_arg_len ;               /* length of arg structure
                                           following vtd */
        long hf_rsp_len ;               /* max response length */
} ;



/*------------------------------------------------------------------------
  READ:  Data can be read from the keyboard, locator, tablet,
  valuators, lighted programmable function keys (lpfk's), etc.
  Set the hft so that ICANON is off, since the read structures
  below contain binary data.
  ------------------------------------------------------------------------*/
/*----------
  ioctl acknowledgement from an emulated hft.  See ioctl request in
  write above.
  ----------*/
#define HFCTLACKCH      0x04    /* high */
#define HFCTLACKCL      0x01    /* low */
struct hfctlack {               /* emulated ioctl acknowledgement */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfctlack)-3
                                                 HFCTLACKCH HFCTLACKCL */
                                        /* Next 3 fields always ignored */
        char hf_sublen;                 /* sub header length = 3 */
        char hf_subtype;                /* sub header type = 0 */
        char hf_align ;
        long hf_request ;               /* ioctl request type */
        long hf_retcode ;               /* return code */
        long hf_arg_len ;               /* length of data structure
                                           following vtd */
} ;

/*------------
  Read untranslated keyboard input
  ------------*/
#define HFINTROWW       'w'
struct hfunxlate                /* read untranslated keyboard input */
{
    char hf_esc;                        /* esc */
    char hf_lbr;                        /* [ */
    char hf_ww;                         /* w */
    char hf_keypos;                     /* key position on keyboard */
    char hf_scancode;                   /* scan code */
    char hf_status[2];                  /* key state */
/* hf_status[0] states */
#define HFUXSHIFT      0x80             /* shift */
#define HFUXCTRL       0x40             /* control */
#define HFUXALT        0x20             /* alternate */
#define HFUXCAPS       0x08             /* caps lock */
#define HFUXNUM        0x04             /* num lock  */
#define HFUXSCROLL     0x02             /* scroll lock */
#define HFUXMAKE       0x01             /* make (depressed) */
/* hf_status[1] states */
#define HFUXRPT        0x80             /* repeat (typamatic) */
#define HFUXLSH        0x40             /* left  shift */
#define HFUXRSH        0x20             /* right shift */
#define HFUXLALT       0x10             /* left  alternate */
#define HFUXRALT       0x08             /* right alternate */
    char hf_seconds[3];                 /* seconds since system startup */
    char hf_sixtyths;                   /* 1/60 seconds */
};

/*----------
  Read input from mouse, locator, tablet, LPFK or dials device

  deltax in hflocator below:
  mouse = relative X delta accumulations in counts of .25mm of
     locator movement in twos-complement signed int form.
  tablet - absolute X coordinate of the tablet sensor.
  LPFK - LPFK number
  dials - dial number

  deltay in hflocator below:
  mouse = relative Y delta accumulations in counts of .25mm of
     locator movement in twos-complement signed int form.
  tablet - absolute Y coordinate of the tablet sensor.
  LPFK - reserved
  dials - dial value delta, signed int (256 points in a 360-degree revolution)
  ----------*/
#define HFINTROWHY      'y'
struct hflocator                /* read input data from mouse, tablet,
                                   LPFK's, valuator dials. */
{
    char hf_esc;                        /* esc */
    char hf_lbr;                        /* [ */
    char hf_why;                        /* y */
    char hf_deltax[2];                  /* see note */
    char hf_deltay[2];                  /* see note */
    char hf_seconds[3];                 /* seconds since system startup */
    char hf_sixtyths;                   /* 1/60 seconds */
    char hf_buttons;                    /* state of buttons */
#define HFBUTTON1       0x80            /* mouse leftmost button */
#define HFBUTTON2       0x40            /* mouse next leftmost button */
#define HFBUTTON3       0x20            /* mouse next leftmost button */
#define HFBUTTON4       0x10            /* mouse next leftmost button */
#define HFBUTTON5       0x08            /* button five */
#define HFBUTTON6       0x02            /* button six */
#define HFBUTTON_STAT   0x04            /* puck/stylus presence (0=out, 1=in) */
    char hf_stype;                      /* mouse = 0,tablet = 1,
                                           LPFK =  2,dials  = 3   */
};

/*----------
  Adapter-generated input may be returned to Monitor Mode
  applications by VTA  multi-byte controls (esc [ r QID 20-bytes-
  of-data).
  ----------*/

/*------------------------------------------------------------------------
  IOCTL:  ioctl calls use the following structures:
  ------------------------------------------------------------------------*/
#define HFIOC           ('H'<<8)
/*      HFCSMGR deleted.  Use HFTCSMGR. */
#define HFTCSMGR        (HFIOC|26)      /* control Screen Manager */
/*      HFQSMGR deleted.  Use HFTQSMGR. */
#define HFTQSMGR        (HFIOC|20)      /* query Screen Manager */
#define HFESOUND        (HFIOC|11)      /* enable sound signals */
#define HFDSOUND        (HFIOC|12)      /* disable sound signals */
/*      HFGCHAN deleted.  Use HFTGETID. */
/*      HFGETMPX deleted. Use HFTGETID. */
/*      HFGETID deleted.  Use HFTGETID. */
#define HFTGETID        (HFIOC|1)       /* get VT identification info */
/*      HFMDMA deleted.   No longer supported. */
/*      HFQDEV deleted.   Use HFTQDEV. */
#define HFTQDEV         (HFIOC|27)      /* query Device Dependent data*/
/*      HFQEIO deleted.  Use HFQERROR. */
#define HFQERROR        (HFIOC|21)      /* query Terminal for i/o Error Value */
/*      HFQKBS deleted.  Use HFQUERY. */
#define HFQUERY         (HFIOC|2)       /* queries of physical devices */
#define HFRCONF         (HFIOC|10)      /* reconfigure HFT sys/default values */
/*      HFSECHO deleted.  Use HFTSECHO. */
#define HFTSECHO        (HFIOC|24)      /* set echo map */
#define HFTSBREAK       (HFIOC|25)      /* set break map */
#define HFSJKBD         (HFIOC|18)      /* set Japanese keyboard map */
#define HFSKBD          (HFIOC|3)       /* set keyboard map */
#define HFSKEP          (HFIOC|15)      /* set   "KEP" signal handling  */
#define HFCKEP          (HFIOC|16)      /* clear "KEP" signal handling  */
#define HFSMON          (HFIOC|5)       /* request to set monitor mode */
#define HFCMON          (HFIOC|6)       /* clear monitor mode */
/*      HFSRINGSELPTR deleted.  No longer supported. */
#define HFESWKBD        (HFIOC|23)      /* change swkb for this terminal */
#define HFCHGLOC        (HFIOC|22)      /* change mouse or tablet values */
#define HFHANDLE        (HFIOC|28)      /* get maj/min & mpx into int */

/*----------
  HFQERROR - get HFTSS error when an i/o operation occurs.  On return,
  error_code contains 0 (last operation was successful) or the error code
  for the last HFTSS operation.
        ioctl(fildes, HFQERROR, &error_code);
        long error_code;
  ----------*/

/*----------
  HFCHGLOC - change mouse or tablet values for this virtual terminal.
        ioctl(fildes, HFCHGLOC, &hfchgloc)
  ----------*/
struct hfchgloc {
        ulong hf_cmd;
#define HFMRATE         0x80    /* set mouse sample rate.  Valid loc_value1 =
                                   10,20,40,60,80,100,200 samples/second */
#define HFMRES          0x40    /* set mouse resolution.  Valid loc_value1 =
                                   1,2,4,8 counts/mm */
#define HFMTHRESH       0x20    /* set mouse horiz & vert thresholds.  Valid
                                   loc_value1 & loc_value2 values = 0 to 32767
                                   units of .25 mm; value1=horiz, value2=vert */
#define HFMSCALE        0x10    /* set mouse scale.  Valid loc_value1 =
                                   1 for 1:1, 2 for 2:1 */
#define HFTRATE         0x08    /* set tablet sample rate.  Valid loc_value1 =
                                   1 to 100 samples/second */
#define HFTRES          0x04    /* set tablet resolution.  Valid loc_value1 =
                                   0 to 1279 lines per inch */
#define HFTDZONES       0x02    /* set tablet horiz & vert deadzones.  Valid
                                   loc_value1 & loc_value2 values = 0 to 32767
                                   value1=horiz, value2=vert*/
#define HFTORIGIN       0x01    /* set tablet origin.  Valid loc_value1 =
                                   0 for lower left, 1 for center */
#define HFMIDDLE        0x01    /* center of tablet */
#define HFLOWERL        0x00    /* lower left corner of tablet */
#define HFTCONV         0x00    /* set tablet conversion.  Valid loc_value1 =
                                   0 for English, 1 for metric */
        ushort loc_value1;
        ushort loc_value2;
};


/*----------
  HFESWKBD - enable software keyboard. Changes the keyboard map a virtual
  terminal is using to the one specified by the keyboard id sent in as an
  argument to this command. The keyboard must have already been added by
  the HFADDSWKB command in a reconfigure operation.
        long ioctl(fildes, HFESWKBD, &kbdid)
        int *kbdid;                 * keyboard id returned from query *
  ----------*/

/*----------
  HFTQDEV- query device.  Detailed device information is returned in
  the hftqdresp structure.
        long ioctl(fildes, HFTQDEV, &hftqdresp)
  ----------*/
struct hftqdresp {               /* response */
        ushort hf_vtid;                 /* mpx of this virtual terminal   */
        dev_t  hf_dev;                  /* major/minor number of hft dd */
        ulong hf_numdisp;               /* # of displays actually installed */
        dev_t hf_kbddev;                /* major/minor number of keyboard dd */
        dev_t hf_mousedev;              /* major/minor number of mouse dd */
        dev_t hf_snddev;                /* major/minor number of sound dd */
        dev_t hf_tabletdev;             /* major/minor number of tablet dd */
        dev_t hf_dialsdev;              /* major/minor number of dials dd */
        dev_t hf_lpfkdev;               /* major/minor number of lpfk dd */
        dev_t hf_dispdev[HFNUMDISP];    /* major/minor number of displays
                                           attached to hft subsystem */
        dev_t hf_defdisp;               /* major/minor number of default disp */
};


/*----------
  HFTGETID - get major/minor number, process group, channel for current VT.
        long ioctl(fildes, HFTGETID, &hftgetid);
  ----------*/
struct hftgetid {
        dev_t hf_dev;                   /* major/minor number of hft dd */
        ulong hf_pgrp;                  /* process id of term. group leader */
        ulong hf_chan;                  /* channel number */
};

/*------------
  HFSMON - requests Monitor Mode, providing a program  with  direct
  control  of  the  screen  and keyboard.
        long ioctl(fildes, HFSMON, &hfsmon);
  HFESOUND - sound interrupts enabled for process or for entire
  process group.  Valid in both MOM and KSR modes.
        long ioctl(fildes, HFESOUND, &hfsmon);
  ------------*/
struct hfsmon {
        long hf_momflags;
#define HFSINGLE  1     /* single proc to rcv MOM or sound signals */
#define HFGROUP   2     /* tty group to rcv MOM or sound signals */
#define HFDSDEACT 4	/* disable deactivation of VT */
};


/*------------
  HFDSOUND - Disable sound interrupts for monitor mode process or entire
  process group.
        long ioctl(fildes, HFDSOUND, 0);
  ------------*/

/*------------
  HFCMON - clear Monitor Mode.  Disables Monitor Mode signals,
  restores  processes old nice value and clears the ring select pointer.
        long ioctl(fildes, HFCMON, 0);
  ------------*/

/*------------
  HFSKEP - enable Keyboard Emulation Program signals.
        long ioctl(fildes, HFSKEP, 0);
  ------------*/

/*------------
  HFCKEP - clear Keyboard Emulation Program signals.
        long ioctl(fildes, HFCKEP, 0);
  ------------*/

/*------------
  These are defined in the termio.h file.
  TCQSAK  - query secured attention key scanning in vt.  flag=0.
  TCSAK   - turn on Secure Attention Key processing.  flag=TCSAKON.
  TCTRUST - set trusted attribute of this vt.  flag=TCUNTRUSTED (default)
     or flag=TCTRUSTED.
  TCQTRUST - query trusted attribute of vt.  flag=0.
        long ioctl(fildes, ioctl_cmd, flag);
        int flag;
  ------------*/


/*----------
  HFRCONF - reconfigure.  Change physical terminal or VT defaults
  for subsequently opened terminals.
        long ioctl(fildes, HFRCONF, &hfrconf);
  ----------*/
/* hf_op - operations */
/*      HFADDLOC        0    add real locator deleted */
/*      HFADDSOUND      1    add sound deleted*/
/*      HFADDDISPLAY    2    add real display deleted */
/*      HFDELDISPLAY    3    delete real display deleted */
#define HFADDDIALS      15      /* add real dial device */
#define HFADDLPFK       16      /* add lighted pgmable func keys device */
#define HFDELDIALS      27
#define HFDELLPFK       28
/*      HFCHGLOCRATE    7    change mouse or tablet sample rate deleted.
                                 Use HFCHGLOC ioctl to set rate for a vt */
/*      HFCHNGDMA       18   deleted.  No longer supported. */
/*      HFADDFONT       4    deleted.  Use HFTADDFONT. */
#define HFTADDFONT      17      /* add a font */
/*      HFKEYMAP        10   deleted.  Use HFADDSWKBD, HFCHGSWKBD. */
#define HFADDSWKBD      23      /* add swkb to list of available kbd's */
#define HFCHGSWKBD      24      /* change to a new default software keyboard */
#define HFCHGKBDRATE    5       /* change keyboard typematic rate */
#define HFCHGKBDDEL     6       /* change keyboard typematic delay */
#define HFCHGCLICK      8       /* change keyboard click */
#define HFCHGVOLUME     9       /* change sound volume */
/*      HFDISPMAP       11    deleted.  */
/*      HFECHOMAP       12    deleted.  Use HFTECHOMAP, HFTBREAKMAP. */
#define HFTECHOMAP      20      /* replace echo map */
#define HFTBREAKMAP     21      /* replace break map */
/*      HFDEFAULT       13    deleted.  Use HFTDEFAULT. */
#define HFTDEFAULT      22      /* replace default values */
#define HFSETDD         14      /* set default real display */
#define HFKEPKEY        25      /* send simulated keystroke to keyboard */
#define HFKEPMOUSE      26      /* send simulated mouse event to mouse */
#define HFS106CAPSLOCK  29      /* set the 106 keyboard capslock key   */

/* hf_obj - object operation is to be performed on.
  Items followed by * take effect only for VT's opened after reconfiguration.

  for HFTADDFONT      file desc of font (use font struct in aixfont.h)
  for HFADDSWKBD      file desc of new software keyboard (use vtmdkey struct)
  for HFCHGSWKBD      keyboard id of swkb to use for default
  for HFCHGKBDRATE*   rate (2-30 chars/sec, in 1 char/sec units)
  for HFCHGKBDDEL*    rate (250-1000 msec by 250s)
  for HFCHGCLICK*     0=off, 1=on */
#define HFCLICKOFF      0       /* off */
#define HFCLICKON       1       /* on */
/*
  for HFCHGVOLUME*    volume: 0=off,1=low,2=med,3=hi */
#define HFVOLOFF        0       /*    volume off */
#define HFVOLLOW        1       /* low volume */
#define HFVOLMED        2       /* medium volume */
#define HFVOLHIGH       3       /* high volume */
/*
  for HFTECHOMAP       file desc of new echo map (use echo_map struct)
  for HFTBREAKMAP      file desc of new break map (use break_map struct)
  for HFTDEFAULT       file desc of new misc defaults (use vtmdef struct)
  for HFSETDD          physical display identifier */

struct hfrconf {
        ulong hf_op;                     /* requested operation */
        ulong hf_obj;                    /* object effected */
};


#define VTM_MAX_KEY_POS 134

/*-------------------------------------------------------------------
  The following structure holds information in the software keyboard
  table to map one keystroke (scan code, position code and status flag)
  to a code page/code point, escape control, control code, or user-
  defined string.  It is used in the vtmdkey struct below for software
  keyboard remapping.
  -------------------------------------------------------------------*/
struct keystroke {
     uchar flag;                 /* type of keystroke  */
#define FLAG_GRAPHIC        0x0  /* normal printable character */
#define FLAG_SINGLE_CONTROL 0x1  /* control code (0x00 to 0x20) */
#define FLAG_CHAR_STRING    0x4  /* char string mapped to key */
#define FLAG_ESC_FUNCTION   0x5  /* escape function */
#define FLAG_CNTL_FUNCTION  0x6  /* control function */
#define FLAG_APP_FUNCTION   0x7

     uchar stat;                 /* state of keystroke */
#define CODE_STAT_NONE    0x0    /* normal key */
#define CODE_STAT_ERROR   0x1    /* error in parsing control sequence */
#define CODE_STAT_DECODED 0x2    /* esc/ctl sequence successfully parsed */
#define CODE_STAT_CURSOR  0x3    /* key affects cursor */
#define CODE_STAT_PF_KEY  0x4    /* programmable function key */
#define CODE_STAT_DEAD    0x5    /* diacritic key */
#define CODE_STAT_STATE   0x6    /* used on japanese keyboards */

     uchar str_page;             /* position in char string key map */

     uchar code_page;            /* code page may be one of */
/* final character values specifying character sets in ANSI */
#define CHARSET_P0              0x3c    /* '<', page 0 */

     uchar code;                /* code point */
};

/*-------------------------------
  Software keyboard map structure
  -------------------------------*/
struct vtmdkey {
        char hf_disp_set[8];            /* display set this maps */
        char hf_kbdname[8];             /* descriptive name of keyboard map */
        struct keystroke keyb_pos_mappings[7][VTM_MAX_KEY_POS];
        ulong capslock[5];              /* bit map of keys affected by */
                                        /*    by caps lock key */
        struct vtmdkey *next;           /* link list pointer */
};



/*-------------------------------------------------------------------
  Default virtual terminal settings.
  See set protocol mode write above for hf_select[] values.
  -------------------------------------------------------------------*/
/* bit selectors for ds_modes:  SM,RM (set mode/reset mode) data stream
   (presentation space) modes (ANSI multi-byte control) */
#define _IRM    0x4000   /* insert/replace  mode           default = 0 */
#define _TSM    0x2000   /* tabulation stop mode           default = 0 */
#define _LNM    0x1000   /* line feed/new line mode        default = 0 */
#define _CNM    0x0800   /* carriage return/new line mode  default = 0 */
#define _AUTONL 0x0400   /* wrap to next line when ovflk   default = 1 */
#define _SRM    0x0020   /* send/receive mode - no echo    default = 1 */

#define DEFAULT_OS_MODES        ((HFXLATKBD<<8) | HFWRAP)
#define DEFAULT_DS_MODES        (_SRM | _AUTONL)

/* default bit maps for tab stops */
#define DEF_TAB_RACK1           0x80808080      /* 1st char, every 8th */
#define DEF_TAB_RACK2           0x80808080      /* every 8th */
#define DEF_TAB_RACK3           0x80808081      /* every 8th, last char */

struct vtmdef {
        ulong  vtmdtab[12];             /* bit mask to store tab settings */
        char hf_select[2];              /* protocol modes */
        ushort ds_modes;                /* data stream modes */
        uchar g0_id;                    /* G0 char set symbol (: ; < > = ? @) */
        uchar g1_id;                    /* G1 char set symbol (: ; < > = ? @) */
};

/*------------
  HFTCSMGR - control screen mgr.
        long ioctl(fildes, HFTCSMGR, &hftsmgrcmd);
  ------------*/
/* commands */
#define SMSCMD          0x00    /* set command virtual terminal */
#define SMACT           0x10    /* activate vt */
#define SMHIDE          0x20    /* hide vt */
#define SMUNHIDE        0x30    /* unhide vt */
#define SMCVTEN         0x40    /* activate cmd vt when locator chorded */
#define SMCVTDI         0x50    /* report input data when loc chorded */
#define SMNOHOTKEY      0x80    /* vt is not part of hot key sequence */

struct hftsmgrcmd {
        long  hf_cmd;                   /* command to SM */
        dev_t hf_vtid;                  /* virtual terminal mpx (channel) */
        long  hf_vsid;                  /* reserved.  Set to zero */
};


struct hfbuf {
        char *hf_bufp;          /* buffer pointer */
        long hf_buflen;          /* buffer length */
};

/*------------
  HFTQSMGR - query screen mgr for number of VT's, mpx number and state
  (active, hidden, command, trusted) of each.
        long ioctl(fildes, HFTQSMGR, &hfbuf);
        struct hfbuf {
            struct hftqstat *hf_bufp;     * ptr to query status structure *
            long   hf_buflen;             * length of hftqstat in bytes *
        };
  ------------*/
struct hftqstat {
        short hf_numvts;
        struct hfvtinfo {
                ushort hf_vtid;         /* multiplex number of virtual term */
                ushort hf_vtstate;      /* state of terminal */
#define HFVTHIDDEN  0x8000      /* hidden terminal */
#define HFVTACTIVE  0x4000      /* active terminal */
#define HFVTCOMMAND 0x2000      /* command terminal */
#define HFVTTRUSTED 0x1000      /* trusted terminal */
#define HFVTNOHOTKEY 0x0800     /* not part of hot key sequence */
        } hf_vtinfo[HFNUMVTS];
};

/*------------
  HFTSECHO - set echo and break maps.
        long ioctl(fildes, HFTSECHO, &hfbuf);
        struct hfbuf {
            struct echo_map *hf_bufp;    * pointer to echo/break structure *
            long   hf_buflen;            * 128 bytes *
        };

  echo_map represents 512 echo bits and is only valid in keyboard send/receive
     (KSR) mode.  If bit=1 (is ON), the character is echoed when the key
     is pressed.
  ------------*/
struct echo_map {               /* echo map */
        ulong vtmecho[16];      /* 512-bit echo mask */
};


/*------------
  HFTSBREAK - set echo and break maps.
        long ioctl(fildes, HFTSBREAK, &hfbuf);
        struct hfbuf {
            struct break_map *hf_bufp;    * pointer to echo/break structure *
            long   hf_buflen;            * 128 bytes *
        };

  break_map represents 512 break bits and is only valid in monitor mode (MOM).
     If bit=1, SIGMSG is sent to the process and the character is reported
     using read system call instead of being placed in the input ring buffer.
  ------------*/
struct break_map {              /* break map */
        ulong vtmbrk[16];       /* 512-bit break mask */
};


/*------------
  HFSKBD  - set keyboard map
  HFSJKBD - set Japanese (kanji) keyboard map
        long ioctl(fildes, HFSKBD, &hfbuf);
        struct hfbuf {
            struct hfkeymap *hf_bufp;    * ptr to keyboard map structure *
            long   hf_buflen;            * length of hfkeymap in bytes *
        };

  In the hfkeyasgn structure, the fields are used as follows:
  Single character or non-spacing character:
        hf_pagenum   = reserved
        hf_character = code point from display symbols
  Function id (see list of func id's in hft documentation):
        hf_pagenum   = high byte of id
        hf_character = low byte of id
  String (must be only key remapped in hfkeymap)
        hf_pagenum   = reserved
        hf_character = length of string in bytes minus 1, immediately
                       followed by the string

  Example.  Remap key position 1 (located below the ESC key) to the
  character string "hi":
        struct hfkeymap {
            char  hf_rsvd1;
            char  hf_nkeys;              * must be 1 for strings *
            char hf_kpos;                * Key position number *
            char hf_kstate;              * string map, base state *
            char hf_pagenum;             * '<' = P0 *
            char hf_character;           * string length *
            char hf_carray[2];           * not a string (no trailing '\0') *
        } hfkeymap = {0,1,1,HFMAPSTR+HFSHFNONE,'<',2,'h','i'};
  ------------*/
#ifndef HFNKEYS
#define HFNKEYS         1
#endif
/* map types */
#define HFMAPMASK       0x0f    /* mask type field */
#define HFMAPCHAR       0x00    /* single character */
#define HFMAPFUNC       0x01    /* predefined function ID */
#define HFMAPNONSP      0x02    /* non-spacing character */
#define HFMAPSTR        0x04    /* string */
/* state mask */
#define HFSHFMASK       0x70    /* mask for shift field */
#define HFSHFNONE       0x00    /* base state */
#define HFSHFALTGR      0x10    /* alt graphic state */
#define HFSHFSHFT       0x20    /* shift state */
#define HFSHFCTRL       0x40    /* control state */
#define HFSHFALT        0x60    /* alt state */
#define HFCAPSL         0x80    /* caps lock applies */
/* Japanese 106-key keyboard state masks */
#define HFROMBASE       0x00    /* Roma-ji base state */
#define HFKANBASE       0x10    /* Kana base state */
#define HFROMSHFT       0x20    /* Roma-ji shift state */
#define HFKANSHFT       0x30    /* Kana shift state */

struct hfkeymap {
        char  hf_rsvd1;
        char  hf_nkeys;                 /* # keys being mapped */
        struct hfkey {
                char hf_kpos;           /* Key position #, from keyboard map */
                char hf_kstate;         /* map type, state mask */
                struct hfkeyasgn  {             /* assignment structure */
                        char hf_pagenum;        /* code page  */
                        char hf_character;      /* character to map  */
#define hf_char                 hf_keyasn.hf_character
#define hf_keyidh               hf_keyasn.hf_pagenum
#define hf_keyidl               hf_keyasn.hf_character
#define hf_kstrl                hf_keyasn.hf_character
                } hf_keyasn;
        } hfkey[HFNKEYS];               /* if HFNKEYS is not equal to 1,
                                           user should set in a #define stmt */
};


/*------------
  HFQUERY - Get information on current VT.
        ioctl(filedes, HFQUERY, &hfquery);
  ------------*/
struct hfquery {
        char *hf_cmd;           /* buffer containing command */
        long  hf_cmdlen;        /* length of buffer containing command */
        char *hf_resp;          /* buffer for response */
        long  hf_resplen;       /* length of expected response */
};

/*------------
  query device retract id.
  ------------*/
struct hfqretract {
    struct hfqretractc *hf_cmd;   /* buffer containing command */
    long   hf_cmdlen;             /* sizeof(struct hfqretractc) */
    struct hfqretractr *hf_resp;  /* buffer for response */
    long   hf_resplen;            /* sizeof(struct hfqretractr) */
};

#define HFQRETRACTCH      0x01    /* high type */
#define HFQRETRACTCL      0x4D    /* low type */
struct hfqretractc {              /* query device ids */
        char hf_intro[HFINTROSZ];       /* esc [ x  sizeof(struct hfqretractc)-3
                                                 HFQRETRACTCH HFQRETRACTCL */
};

#define HFQRETRACTRH      0x01    /* high type */
#define HFQRETRACTRL      0xCD    /* low type */
struct hfqretractr                /* query device id response */
{
        char hf_intro[HFINTROSZ];       /* esc [ x size-3
                                                HFQRETRACTRH HFQRETRACTRL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */
        char hf_vtid[2];                /* id of vt to release display */
        char hf_devid[4];               /* physical display (device) id */
};

#define HFQPINGCH         0x01    /* high type */
#define HFQPINGCL         0x49    /* low type */
struct hfqpingc {              	  /* query device ids */
        char hf_intro[HFINTROSZ];       /* esc [ x  sizeof(struct hfqpingc)-3
                                                 HFQPINGCH HFQPINGCL */
};

#define HFQPINGRH         0x01    /* high type */
#define HFQPINGRL         0xC9    /* low type */
struct hfqpingr                   /* query device id response */
{
        char hf_intro[HFINTROSZ];       /* esc [ x size-3
                                                HFQPINGRH HFQPINGRL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */
        char hf_vtid[2];                /* id of vt to release display */
        char hf_devid[4];               /* physical display (device) id */
};


#define HFQGRANTCH      0x01    /* high type */
#define HFQGRANTCL      0x4E    /* low type */
struct hfqgrantc {              /* query device ids */
        char hf_intro[HFINTROSZ];       /* esc [ x  sizeof(struct hfqgrantc)-3
                                                 HFQGRANTCH HFQGRANTCL */
};

#define HFQGRANTRH      0x01    /* high type */
#define HFQGRANTRL      0xCE    /* low type */
struct hfqgrantr                /* query device id response */
{
        char hf_intro[HFINTROSZ];       /* esc [ x size-3
                                                HFQGRANTRH HFQGRANTRL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */
        char hf_vtid[2];                /* id of vt to release display */
        char hf_devid[4];               /* physical display (device) id */
};


/*------------
  query display ids.
  ------------*/
struct hfqdevid {
    struct hfqdevidc *hf_cmd;   /* buffer containing command */
    long   hf_cmdlen;           /* sizeof(struct hfqdevidc) */
    struct hfqdevidr *hf_resp;  /* buffer for response */
    long   hf_resplen;          /* sizeof(struct hfqdevidr) */
};

#define HFQDEVIDCH      0x01    /* high type */
#define HFQDEVIDCL      0x40    /* low type */
struct hfqdevidc {              /* query device ids */
        char hf_intro[HFINTROSZ];       /* esc [ x  sizeof(struct hfqdevidc)-3
                                                 HFQDEVIDCH HFQDEVIDCL */
};

#define HFQDEVIDRH      0x01    /* high type */
#define HFQDEVIDRL      0xC0    /* low type */
struct hfqdevidr                /* query device id response */
{
        char hf_intro[HFINTROSZ];       /* esc [ x size-3
                                                HFQDEVIDRH HFQDEVIDRL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */
        char hf_numdev[2];              /* number of devices */
        /*------(repeats for each device, hf_numdev times)------*/
        char hf_devid[4];               /* physical display (device) id */
        char hf_class[4];               /* display class = 0x44 */
};

/*------------
  query physical device for locator, display, font and color info.
  ------------*/
struct hfqphdev {
    struct hfqphdevc *hf_cmd;   /* buffer containing command */
    long   hf_cmdlen;           /* sizeof(struct hfqphdevc) */
    struct hfqphdevr *hf_resp;  /* buffer for response */
    long   hf_resplen;          /* sizeof(struct hfqphdevr) */
};

#define HFQPDEVCH       0x01    /* high type */
#define HFQPDEVCL       0x41    /* low type */
struct hfqphdevc {              /* query physical device for locator,
                                   display, font and color info */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfqphdevc)-3
                                                 HFQPDEVCH HFQPDEVCL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */
        char hf_phdevid[4];             /* physical display (device) id */
        char hf_rsvd[4];                /* reserved */
};

struct  hfqfont {
        char hf_fontid[4];              /* physical font id */
        char hf_fontname[16];           /* font name (filename etc.) */
        char hf_fontweight[8];          /* Bold, semibold, book etc. */
        char hf_fontstyle[8];           /* physical font style (Roman,
                                            italic etc.) */
        char hf_fontencode[8];          /* code page this font renders */
        char hf_fontwidth[4];           /* font charbox width  (pels) */
        char hf_fontheight[4];          /* font charbox height (pels) */
};

struct hfcolor {
        char hf_numcolor[4];            /* # of colors possible */
        char hf_numactive[4];           /* # of colors active at once */
        char hf_numfgrnd[4];            /* # of foreground colors */
        char hf_numbgrnd[4];            /* # of background colors */
        /*(variable size:  repeats for each active color, hf_numactive times)*/
        /* the color table will be filled in to the end of the buffer provided
           by the user or to the end of the color table, whichever comes 1st */
        char hf_actcolor[4];            /* active color value = 0 to
                                           (numactive-1) */
};

#define HFQPDEVRH       0x01    /* high type */
#define HFQPDEVRL       0xC1    /* low type */
struct hfqphdevr {              /* query physical dev response (ack) */
        char hf_intro[HFINTROSZ];       /* esc [ x size-3 HFQPDEVRH HFQPDEVRL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */
                                /* LOCATOR DEVICE */
        char hf_scale[4];               /* scale or conversion */
/* bit indicators for hf_scale[0] */
#define HFMSCALE1       0x01            /* Mouse scale is 1 to 1 */
#define HFMSCALE2       0x02            /* Mouse scale is 2 to 1 */
/* bit indicators for hf_scale[1] */
#define HFTCONVE        0x00            /* Tablet conversion is English units */
#define HFTCONVM        0x01            /* Tablet conversion is metric units */
        char hf_locattr[1];             /* mouse and/or tablet attached */
#define HFLOCREL        0x40            /* relative = mouse */
#define HFLOCABS        0x80            /* absolute = tablet */
        char hf_rsvd[3];                /* reserved */
                                /* DISPLAY DEVICE */
        char hf_attrib[4];              /* attributes */

/* bit selectors for hf_attrib[0] */
#define HFISAPA         0x80    /* off(0)=character, on(1)=apa */
#define HFHASBLINK      0x40    /* off(0)=no blink,  on(1)=blink */

/* bit selectors for hf_attrib[2] */
#define HFHASCOLOR      0x01    /* on(1)=has color */

/* bit selectors for hf_attrib[3] */
#define HFCHGPALET      0x80    /* on(1)=1 can chg color map (palette) */

        char hf_pwidth[4];              /* display screen width  (pels) */
        char hf_pheight[4];             /* display screen height (pels) */
        char hf_mwidth[4];              /* display screen width  (mm) */
        char hf_mheight[4];             /* display screen height (mm) */
        char hf_bperpel[4];             /* bits per pel (1, 2 or 4) */
        char hf_phdevid[4];             /* physical display device id */
                                /* FONT */
        char hf_numfont[4];             /* # of fonts avail to display */
        char hf_fontstart;              /* the hfqfont structure is
                                           repeated N number of times, where
                                           N is the value in hf_numfonts.
                                           Following this is one color
                                           response (struct hfcolor).    */
};


/*------------
  query font palette info.
  ------------*/
struct hfqpalet {
    struct hfqpaletc *hf_cmd;   /* buffer containing command */
    long   hf_cmdlen;           /* sizeof(struct hfqpaletc) */
    struct hfqpaletr *hf_resp;  /* buffer for response */
    long   hf_resplen;          /* sizeof(struct hfqpaletr) */
};

#define HFQPALETCH       0x01    /* high type */
#define HFQPALETCL       0x4F    /* low type */
struct hfqpaletc {              /* query for font palette info */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfqpaletc)-3
                                                 HFQPALETCH HFQPALETCL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */
};


#define HFQPALETRH       0x01    /* high type */
#define HFQPALETRL       0xCF    /* low type */
struct hfqpaletr {               /* query font response (ack) */
        char hf_intro[HFINTROSZ];   /* esc [ x size-3 HFQPALETRH HFQPALETRL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */

        char hf_pwidth[4];              /* display screen width  (pels) */
        char hf_pheight[4];             /* display screen height (pels) */
                                /* FONT */
        char hf_numfont[4];             /* # of fonts avail to display */
        char hf_fontstart;              /* the hfqfont structure is
                                           repeated N number of times, where
                                           N is the value in hf_numfonts.*/
};


/*------------
  Query locator device:

  mouse -  resolution, horiz/vert thresholds, sample rate
  tablet - resolution, horiz/vert dead zone,  sample rate, horiz/vert
        max counts, origin
  ------------*/

struct hfqloc {
    struct hfqgraphdev *hf_cmd; /* buffer containing command */
    long   hf_cmdlen;           /* sizeof(struct hfqgraphdev) */
    struct hfqlocr *hf_resp;    /* buffer for response */
    long   hf_resplen;          /* sizeof(struct hfqlocr) */
};

/*------------
  Query graphics input/output (GIO) device:

  lpfks - how many keys, if keys are on or off.
  dials - number, granularity.
  ------------*/

struct hfqdial_lpfk {
    struct hfqgraphdev *hf_cmd;         /* buffer containing command */
    long   hf_cmdlen;                   /* sizeof(struct hfqgraphdev) */
    struct hfdial_lpfk *hf_resp;        /* buffer for response */
    long   hf_resplen;                  /* sizeof(struct hfdial_lpfk) */
};

#define HFQLOCCH        0x01    /* locator, high type */
#define HFQLOCCL        0x42    /* locator, low type */
#define HFQMOUSECH      0x01    /* mouse, high type */
#define HFQMOUSECL      0x4A    /* mouse, low type */
#define HFQTABLETCH     0x01    /* tablet, high type */
#define HFQTABLETCL     0x4B    /* tablet, low type */
#define HFQLPFKSCH      0x01    /* lpfk, high type */
#define HFQLPFKSCL      0x43    /* lpfk, low type */
#define HFQDIALSCH      0x01    /* dials, high type */
#define HFQDIALSCL      0x44    /* dials, low type */
struct hfqgraphdev {            /* query graphics device cmd */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfqgraphdev)-3
                                                 0x01 low */
};

#define HFQLOCRH        0x01    /* locator, high type */
#define HFQLOCRL        0xC2    /* locator, low type */
#define HFQMOUSERH      0x01    /* mouse, high type */
#define HFQMOUSERL      0xCA    /* mouse, low type */
#define HFQTABLETRH     0x01    /* tablet, high type */
#define HFQTABLETRL     0xCB    /* tablet, low type */
struct hfqlocr {                /* query locator response */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfqlocr)-3
                                                 HFQLOCRH HFQLOCRL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */
        char hf_resolution[4];          /* locator resolution */
        char hf_devinfo[1];             /* mouse/tablet, input device */
/*
#define HFLOCABS        0x80       absolute=tablet
#define HFLOCREL        0x40       relative = mouse */
#define HFLOCUNKNOWN    0x00    /* unknown input device */
#define HFLOCSTYLUS     0x02    /* stylus */
#define HFLOCPUCK       0x04    /* puck */
        char hf_rsvd[3];                /* reserved */
        char hf_horzmax_cnt[2];         /* tablet horizontal max count */
        char hf_vertmax_cnt[2];         /* tablet vertical max count   */
        char hf_horzdead_zone[2];       /* tablet horiz dead zone or mouse
                                              threshold */
        char hf_vertdead_zone[2];       /* tablet vert dead zone or mouse
                                              threshold */
        char hf_sample_rate[2];         /* sample rate */
        char hf_origin[4];              /* tablet origin */
/* bit indicators for hf_origin[0]:
#define HFMIDDLE        0x01     * middle of tablet *
#define HFLOWERL        0x00     * lower left corner of tablet */
        char hf_scale[4];
/* bit indicators for hf_scale[0]:
#define HFMSCALE1       0x01             * Mouse scale is 1 to 1 *
#define HFMSCALE2       0x02             * Mouse scale is 2 to 1 *
 * bit indicators for hf_scale[1]:
#define HFTCONVE        0x00             * Tablet conversion is English units
#define HFTCONVM        0x01             * Tablet conversion is metric units */
};

#define HFQLPFKSRH      0x01    /* LPFKs response - high (major) type */
#define HFQLPFKSRL      0xC3    /* low (minor) type                   */
#define HFQDIALSRH      0x01    /* Dials response - high (major) type */
#define HFQDIALSRL      0xC4    /* low  (minor) type                  */
/* query lpfk and dials response is in an hfdial_lpfk structure
   defined in the write section above (used in set/reset lpfks/dials) */


/*------------
  query presentation space - user defines block of characters in
  presentation space to query.  Receives attribute and character set
  information on characters within the block.
  ------------*/

struct hfqpres {
    struct hfqpresc *hf_cmd;    /* buffer containing command */
    long   hf_cmdlen;           /* sizeof(struct hfqpresc) */
    struct hfqpresr *hf_resp;   /* buffer for response */
    long   hf_resplen;          /* sizeof(struct hfqpresr) */
};

#define HFQPRESCH       0x02    /* high type */
#define HFQPRESCL       0x40    /* low type */
struct hfqpresc {               /* query presentation space */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfqpresc)-3
                                                 HFQPRESCH HFQPRESCL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 0 */
        char hf_xuleft;                 /* x upper left coordinate */
        char hf_yuleft;                 /* y upper left coordinate  */
        char hf_xlright;                /* x lower right coordinate  */
        char hf_ylright;                /* y lower right coordinate  */
};

/* The response is returned in an hfqpresr structure.  The  response
   is  an  ascii  data stream that contains character codes from the
   queried blocks.  It includes all ascii data currently  associated
   with  the  input  buffer. Character set and attribute changes are
   indicated with SGR  and  SG0  control  sequences.   A  line  feed
   control is returned after the last character code in each line of
   the queried block. */
#define HFQPRESRH       0x02    /* high type */
#define HFQPRESRL       0xC0    /* low type */
struct hfqpresr {               /* query presentation space response */
        char hf_intro[HFINTROSZ];       /* esc [ x size-3 HFQPRESRH HFQPRESRL */
};

/*------------
  query swkbd command
  ------------*/
struct hfqswkb {
    struct hfqswkbc *hf_cmd;    /* buffer containing command */
    long   hf_cmdlen;           /* sizeof(struct hfqswkbc) */
    struct hfqswkbr *hf_resp;   /* buffer for response */
    long   hf_resplen;          /* sizeof(struct hfqswkbr) */
};

#define HFQSWKBCH        0x01    /* high type */
#define HFQSWKBCL        0x4C    /* low  type */
struct hfqswkbc {                /* query swkb command */
        char hf_intro[HFINTROSZ];          /* esc [ x sizeof(struct hfqswkbc)-3
                                                 HFQSWKBCH HFQSWKBCL */
};

#define HFQSWKBRH        0x01    /* high type */
#define HFQSWKBRL        0xCC    /* low  type */
struct hfqswkbr {                /* query swkb response */
        char hf_intro[HFINTROSZ];       /* esc [ x  sizeof(struct hfqswkbr)-3
                                                 HFQSWKBRH HFQSWKBRL */
                                        /* Next three fields ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub type = 1 */
        char hf_numkbds[4];             /* number of swkb's loaded */
        struct hfkbds {
             char hf_kbdid[4];          /* this keyboard's id */
             char hf_disp_set[8];       /* display set of this keyboard map */
             char hf_kbdname[8];        /* descriptive name of this keyboard */
        } hfkbds[1];                    /* repeated hf_numkbd times */
};

/*------------
  query hft command
  ------------*/
struct hfqhft {
    struct hfqhftc *hf_cmd;     /* buffer containing command */
    long   hf_cmdlen;           /* sizeof(struct hfqhftc) */
    struct hfqhftr *hf_resp;    /* buffer for response */
    long   hf_resplen;          /* sizeof(struct hfqhftr) */
};

#define HFQHFTCH        0x01    /* high type */
#define HFQHFTCL        0x45    /* low  type */
struct hfqhftc {                /* query hft command */
        char hf_intro[HFINTROSZ];          /* esc [ x sizeof(struct hfqhftc)-3
                                                 HFQHFTCH HFQHFTCL */
};

#define HFQHFTRH        0x01    /* high type */
#define HFQHFTRL        0xC5    /* low  type */
struct hfqhftr {                /* query hft response */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfqhftr)-3
                                                 HFQHFTRH HFQHFTRL */
                                        /* Next three fields ignored */
        char hf_sublen;                 /* sub header length = 3 */
        char hf_subtype;                /* sub header type = 1 */
        char hf_align ;                 /* ALIGNMENT BYTE !!!*/
        long hf_phdevid ;               /* physical display (device) id */
        long hf_phrow ;                 /* number of rows */
        long hf_phcol ;                 /* number of columns */
        long hf_phcolor ;               /* number of colors */
        long hf_phfont ;                /* number of fonts */
        long hf_phkbdid;                /* physical keyboard id */
#define HF101KBD   0xBFB0               /* 101-key keyboard */
#define HF102KBD   0xBFB1               /* 102-key keyboard */
#define HF106KBD   0xBFB2               /* 106-key keyboard */
#define HF122KBD   0xAB86               /* 122-key keyboard */
        long hf_reserve[2];             /* reserved */
};

/*------------
  query keyboard status
  ------------*/
struct hfqkbs {
    struct hfqkbsc *hf_cmd;     /* buffer containing command */
    long   hf_cmdlen;           /* sizeof(struct hfqkbsc) */
    struct hfqkbsr *hf_resp;    /* buffer for response */
    long   hf_resplen;          /* sizeof(struct hfqkbsr) */
};

#define HFQKBSCH        0x01    /* high */
#define HFQKBSCL        0x47    /* low */
struct hfqkbsc {                /* query keyboard status */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfqkbsc)-3
                                                 HFQKBSCH HFQKBSCL */
};

#define HFQKBSRH        0x01    /* high */
#define HFQKBSRL        0xC7    /* low */
struct hfqkbsr {                /* query keyboard status response */
        char hf_intro[HFINTROSZ];       /* esc [ x  sizeof(struct hfqkbsr)-3
                                                 HFQKBSRH HFQKBSRL */
                                        /* Next 3 fields ignored */
        char hf_sublen;                 /* sub header length = 3 */
        char hf_subtype;                /* sub header type = 1 */
        char hf_align;                  /* alignment byte */
        char hf_kbstat[4];              /* keyboard status info */
                                /* for hf_kbstat[3]     */
#define HFALPHAKBD    0x04              /* Alpha keyboard       */
#define HFKATAKBD     0x02              /* Katakana keyboard    */
#define HFHIRAKBD     0x01              /* Hiragana keyboard    */
};

/*------------
  query diagnostics command
  ------------*/
struct hfqdiag {
    struct hfqdiagc *hf_cmd;    /* buffer containing command */
    long   hf_cmdlen;           /* sizeof(struct hfqdiagc) */
    struct hfqdiagr *hf_resp;   /* buffer for response */
    long   hf_resplen;          /* sizeof(struct hfqdiagr) */
};

#define HFQDIAGCH     0x01      /* high type */
#define HFQDIAGCL     0x48      /* low type */
struct hfqdiagc {               /* query diagnostics command */
        char hf_intro[HFINTROSZ];       /* esc [ x sizeof(struct hfqdiagc)-3
                                                 HFQDIAGCH HFQDIAGCL */
                                        /* Next 2 fields always ignored */
        char hf_sublen;                 /* sub header length = 2 */
        char hf_subtype;                /* sub header type = 1 */
        char hf_device;                 /* device */
        char hf_testnum;                /* test number */
        char hf_testdata[1];            /* test data */
};
/* device to test   */
#define HF_KYBD         0x01    /* keyboard */
#define HF_MOUSE        0x02    /* mouse */
#define HF_TABLET       0x03    /* tablet */
#define HF_DISPLAY      0x04    /* display */
#define HF_LPFK         0x05    /* lighted programmable function keys */
#define HF_DIALS        0x06    /* dials */
#define HF_SOUND        0x07    /* sound */

#define HFQDIAGRH       0x01    /* high type */
#define HFQDIAGRL       0xC8    /* low type */
struct hfqdiagr {               /* query diagnostics response */
        char hf_intro[HFINTROSZ];       /* esc [ x  sizeof(struct hfqdiagr)-3
                                                 HFQDIAGRH HFQDIAGRL */
                                        /* Next 3 fields ignored */
        char hf_sublen;                 /* sub header length = 3 */
        char hf_subtype;                /* sub header type = 1 */
        char hf_align;                  /* alignment byte */
        char hf_result[1];              /* test results */
};

#endif /* _H_HFT */
