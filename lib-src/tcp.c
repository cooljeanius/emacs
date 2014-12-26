/* tcp.c
 * TCP/IP stream emulation for GNU Emacs.
 * Copyright (C) 1988, 1989, 1992, 1993 Free Software Foundation, Inc.
 *
 * Author: Masanobu Umeda
 * Maintainer: umerin@mse.kyutech.ac.jp
 */
/*
This file is part of GNU Emacs.

GNU Emacs is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Emacs is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Emacs; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.
*/
/*
 *
 * Yasunari, Itoh at PFU limited contributed for Fujitsu UTS and SX/A.
 *
 * Thu Apr  6 13:47:37 JST 1989
 * USG fixes by Sakaeda <saka@mickey.trad.pf.fujitsu.junet>
 *
 * For Fujitsu UTS compile with:
 *	cc -O -o tcp tcp.c -DFUJITSU_UTS -lu -lsocket
 */

#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>

#ifdef FUJITSU_UTS
# define USG
# include <sys/ucbtypes.h>
# include <sys/tisp/socket.h>
# include <netdb.h>
# include <sys/tisp/in.h>
#else
# include <stdlib.h>
# include <string.h>
# include <sys/socket.h>
# include <netdb.h>
# include <netinet/in.h>
# include <unistd.h>
# if !defined(USG) && defined(DEBUG)
#  define USG 1
# endif /* !USG && DEBUG */
#endif /* FUJITSU_UTS */

#ifdef USG
# include <sys/stat.h>
# include <signal.h>
#endif /* USG */

#ifdef FUJITSU_UTS
# define bcopy(f, t, n)    memcpy(t, f, n)
# define bcmp(b1, b2, n)   (memcmp(b1, b2, n) != 0)
# define bzero(b, n)       memset(b, 0, n)
#endif /* FUJITSU_UTS */

#ifdef USG
/* prototype: */
extern void sigout(int dummy_param);
#endif /* USG */

/* in case the autoconf test result never made it here: */
#ifndef SELECT_TYPE_ARG234
/* not sure what results we get on other systems, so just go with what I
 * know: */
# ifdef __APPLE__
#  define SELECT_TYPE_ARG234 fd_set *
# else
#  define SELECT_TYPE_ARG234 int
# endif /* __APPLE__ */
#endif /* !SELECT_TYPE_ARG234 */

/* no autoconf test for this one afaik, but do likewise: */
#ifndef ARG2_FOR_BIND_AND_CONNECT
# ifdef __APPLE__
#  define ARG2_FOR_BIND_AND_CONNECT const struct sockaddr *
# else
#  define ARG2_FOR_BIND_AND_CONNECT struct sockaddr_in *
# endif /* __APPLE__ */
#endif /* ARG2_FOR_BIND_AND_CONNECT */

#ifdef USG
int selectable = 1;

/* this is taken from "../src/config.h", which we cannot include the
 * entirety of: */
/* The _Noreturn keyword of C11: */
#if !(defined(_Noreturn) \
    || (defined(__STDC_VERSION__) && (201112 <= __STDC_VERSION__)))
# if ((3 <= __GNUC__) || ((__GNUC__ == 2) && (8 <= __GNUC_MINOR__)) \
     || (0x5110 <= __SUNPRO_C))
#  define _Noreturn __attribute__ ((__noreturn__))
# elif defined(_MSC_VER) && (1200 <= _MSC_VER)
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn
# endif /* compatible compiler check */
#endif /* have _Noreturn */

_Noreturn void sigout(int dummy_param)
{
# if defined(__APPLE__) && defined(__APPLE_CC__) && (__APPLE_CC__ > 1)
#  pragma unused (dummy_param)
# endif /* __APPLE__ && (__APPLE_CC__ > 1) */
  fcntl(fileno(stdin), F_SETFL, 0);
  exit(-1);
}
#endif /* USG */

int main(int argc, char *argv[])
{
  struct hostent *host;
  struct sockaddr_in sockin, sockme;
  struct servent *serv;
  char *hostname = NULL;
  const char *service = "nntp";
  int port;
  int readfds;
  int writefds;
  int server; /* NNTP Server */
  int emacsIn = fileno(stdin); /* Emacs intput */
  int emacsOut = fileno(stdout); /* Emacs output */
  char buffer[1024];
  int nbuffer; /* Number of bytes in buffer */
  int wret;
  char *retry; /* retry bufferp */
  int false = 0; /* FALSE flag for setsockopt() */

  if (argc < 2)
    {
      fprintf(stderr, "Usage: %s HOST [SERVICE]\n", argv[0]);
      exit(1);
    }
  if (argc >= 2)
    hostname = argv[1];
  if (argc >= 3)
    service = argv[2];

  if ((host = gethostbyname(hostname)) == NULL)
    {
      perror("gethostbyname");
      exit(1);
    }
  if (isdigit(service[0]))
    port = atoi(service);
  else
    {
      serv = getservbyname(service, "tcp");
      if (serv == NULL)
	{
	  perror("getservbyname");
	  exit(1);
	}
      port = serv->s_port;
    }

  bzero(&sockin, sizeof(sockin));
  sockin.sin_family = host->h_addrtype;
  bcopy(host->h_addr, &sockin.sin_addr, host->h_length);
  sockin.sin_port = htons(port);
  if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
      perror("socket");
      exit(1);
    }
  if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &false, sizeof(false)))
    {
      perror("setsockopt");
      exit(1);
    }
  bzero(&sockme, sizeof(sockme));
  sockme.sin_family = sockin.sin_family;
  sockme.sin_addr.s_addr = INADDR_ANY;
  if (bind(server, (ARG2_FOR_BIND_AND_CONNECT)&sockme, sizeof(sockme)) < 0)
    {
      perror("bind");
      exit(1);
    }
  if (connect(server, (ARG2_FOR_BIND_AND_CONNECT)&sockin, sizeof(sockin)) < 0)
    {
      perror("connect");
      close(server);
      exit(1);
    }

#ifdef O_NDELAY
  fcntl(server, F_SETFL, O_NDELAY);

# ifdef USG
  /* USG pipe cannot not select emacsIn: */
  {
    struct stat statbuf;
    fstat(emacsIn, &statbuf);
    if (statbuf.st_mode & 010000)
      selectable = 0;
    if (!selectable)
      {
	signal(SIGINT, sigout);
	fcntl(emacsIn, F_SETFL, O_NDELAY);
      }
  }
# endif /* USG */
#endif /* O_NDELAY */

  /* Connection established. */
  while (1)
    {
      readfds = ((1 << server) | (1 << emacsIn));
      if (select(32, (SELECT_TYPE_ARG234)&readfds, (SELECT_TYPE_ARG234)NULL,
                 (SELECT_TYPE_ARG234)NULL, (struct timeval *)NULL) == -1)
	{
	  perror("select");
	  exit(1);
	}
      if (readfds & (1 << emacsIn))
	{
	  /* From Emacs: */
	  nbuffer = read(emacsIn, buffer, sizeof buffer -1);

#ifdef USG
	  if (selectable && (nbuffer == 0))
	    {
	      goto finish;
	    }
	  else if (!(readfds & (1 << server)) && (nbuffer == 0))
	    {
	      sleep (1);
	    }
	  else
#else
	    if (nbuffer == 0)
	      goto finish;
#endif /* USG */
	  for (retry = buffer; nbuffer > 0; nbuffer -= wret, retry += wret)
	    {
	      writefds = (1 << server);
	      if (select((server + 1), (SELECT_TYPE_ARG234)NULL,
                         (SELECT_TYPE_ARG234)&writefds,
                         (SELECT_TYPE_ARG234)NULL, (struct timeval*)NULL) == -1)
		{
		  perror("select");
		  exit(1);
		}
	      wret = write(server, retry, nbuffer);
	      if (wret < 0) goto finish;
	    }
	}
      if (readfds & (1 << server))
	{
	  /* From NNTP server: */
	  nbuffer = read(server, buffer, sizeof buffer -1);
	  if (nbuffer == 0)
	    goto finish;
	  for (retry = buffer; nbuffer > 0; nbuffer -= wret, retry += wret)
	    {
	      writefds = (1 << emacsOut);
#ifdef USG
	      if (selectable)
#endif /* USG */
		if (select((emacsOut + 1), (SELECT_TYPE_ARG234)NULL,
                           (SELECT_TYPE_ARG234)&writefds,
                           (SELECT_TYPE_ARG234)NULL, (struct timeval*)NULL) == -1)
		  {
		    perror("select");
		    exit(1);
		  }
	      wret = write(emacsOut, retry, nbuffer);
	      if (wret < 0) goto finish;
	    }
	}
    }

  /* End of communication: */
 finish:
  close(server);
#ifdef USG
  if (!selectable) {
    fcntl(emacsIn, F_SETFL, 0);
  }
#endif /* USG */
  close(emacsIn);
  close(emacsOut);
  exit(0);
}

/* EOF */
