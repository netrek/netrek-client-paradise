/*
 * credits.c
 *
 * Bill Dyess
 *
 */
#include "copyright2.h"
#include "defines.h"

#include <stdio.h>
#ifdef STDC_HEADERS
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif
#include <errno.h>
#include <pwd.h>
#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#include <ctype.h>
#include "config.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

/* This is never used in this file.  Why is it here?  Someone tell me if
   this breaks the OSF/1 port */
#if 0
#ifndef __osf__
long    time();
#endif
#endif

static char *credits = "\
NetrekII (Paradise) is Copyright 1994-1999 by the Paradise Working Group\n\
\n\
NetrekII (Paradise) was created by:\n\
   Larry Denys         (a fungusamoungus)  <sl1yn@paradise.declab.usu.edu>\n\
   Kurt Olsen          (Bubbles)           <kurto@cc.usu.edu>\n\
   Brandon Gillespie   (Lynx)              <brandon@paradise.declab.usu.edu>\n\
\n\
Developers (alphabetical order):\n\
     Dave Ahn                             Larry Denys (a fungusamoungus)\n\
     Eric Dorman                          Bill Dyess (Thought)\n\
     Rob Forsman (Hammor)                 Brandon Gillespie (Lynx)\n\
     Bob Glamm (Brazilian)                Mike McGrath (Kaos)\n\
     Heath Kehoe (Key)                    Kurt Olsen (Bubbles)\n\
     Sujal Patel (LordZeus)               Joe Rumsey (Ogre)\n\
\n\
Contributors (alphabetical order):\n\
     Scott Drellishak                     Mike Lutz\n\
     Ted Hadley                           Heiji Horde\n\
\n\
NetrekII (Paradise) copyright 1993 by:\n\
            Larry Denys, Kurt Olsen, Brandon Gillespie and Rob Forsman\n\
Netrek (XtrekII) copyright 1989 by:\n\
            Scott Silvey and Kevin Smith\n\
Xtrek copyright 1986 by:\n\
            Chris Guthrie and Ed James\n\
Short packets by:                         Heiko Wengler and Ted Hadley\n\
UDP by:                                   Andy Mcfadden\n\
Bronco Release by:                        Terrence Chang\n\
Ted Turner Client by:                     Bill Dyess\n\
Artists (alphabetical order):\n\
     Brandon Gillespie (Lynx)             Mike McGrath (Kaos)\n\
     Joe Rumsey (Ogre)\n\
     Ola Andersson (Janice Rand)\n\
";

void 
showCredits(win)
  W_Window win;
{
  char *start = credits;
  char *end = credits;
  int  y = 30;
  
  if(!W_IsMapped(win)) {
    fprintf(stderr,"Why am I trying to write the credits into a window that doesn't exist?\n");
    return;
  }
  while(*start) {
    while(*end != '\n' && *end) end++;
    W_WriteText(win, 10, y, textColor, start, end - start, W_BoldFont);
    start = ++end;
    y += W_Textheight;
  }

#ifdef BUFFERING
  /* flush buffer if one exists [BDyess] */
  if(W_IsBuffered(win)) W_DisplayBuffer(win);	
#endif /*BUFFERING [BDyess]*/
}
