
/*
 * warning.c
 */
#include "copyright.h"
#include "defines.h"

#include <stdio.h>
#include <signal.h>
#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#ifdef STDC_HEADERS
#include <stdlib.h>
#endif
#include <ctype.h>
#include <math.h>
#include "config.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"
#ifdef HAVE_TIME_H
#include <time.h>
#if defined(HAVE_SYS_TIME_H) && defined(TIME_WITH_SYS_TIME)
#include <sys/time.h>
#endif
#else
#include <sys/time.h>
#endif

#if 0	/* time.h or sys/time.h will have this under AIX v3.2.5 or later */
#ifdef RS6K
struct tm {
    int     tm_sec;		/* seconds (0 - 59) */
    int     tm_min;		/* minutes (0 - 59) */
    int     tm_hour;		/* hours (0 - 23) */
    int     tm_mday;		/* day of month (1 - 31) */
    int     tm_mon;		/* month of year (0 - 11) */
    int     tm_year;		/* year - 1900 */
    int     tm_wday;		/* day of week (Sunday = 0) */
    int     tm_yday;		/* day of year (0 - 365) */
    int     tm_isdst;		/* flag: daylight savings time in effect */
    long    tm_gmtoff;		/* offset from GMT in seconds */
    char   *tm_zone;		/* abbreviation of timezone name */

};
#endif
#endif

static void warning2 P((char *text, char save_buffer[], int *count, int *timer, 
                        int y, W_Color color, W_Font font));

#define W_XOFF 5
#ifndef AMIGA
#define W_YOFF 5
#else
#define W_YOFF 1
#endif

/* returns a string of the form hour:minute:second [BDyess] */
char   *
timeString(t)
    time_t  t;
{
    static char *s = NULL;
    struct tm *tm;

    if (!s)
	s = (char *) malloc(9);
    if (t > 24 * 60 * 60) {
	tm = localtime(&t);
	sprintf(s, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
    } else
	sprintf(s, "%02d:%02d:%02d", (int) (t / (60 * 60)), (int) ((t % (60 * 60)) / 60),
		(int) (t % 60));
    return s;
}

/* hwarnings are printed at the top of the screen ("high") vice the bottom 
   [BDyess]*/
void
hwarning(text)
    char *text;
{
    warning2(text,hwarningbuf,&hwarncount,&hwarntimer,HUD_Y,W_Yellow,
             W_BoldFont);
}

void
warning(text)
    char *text;
{
    warning2(text,warningbuf,&warncount,&warntimer,winside - HUD_Y - W_Textheight,
            W_Green, W_RegularFont);
}


/*
   ** The warning in text will be printed in the warning window.
   ** The message will last WARNTIME/10 seconds unless another message
   ** comes through and overwrites it.
 */

/* warning2 is called only by hwarning and warning [BDyess] */
static void
warning2(text,save_buffer,count,timer,y,color,font)
    char   *text;
    char   save_buffer[];
    int    *count, *timer;
    int    y;
    W_Color color;
    W_Font font;
{
    /* long curtime; */
    /* char timebuf[10]; */
    /* struct tm *tm; */
    char    newtext[91];	/* infinite appending fix - jn */
    int     len;

    char   *d;
    int     dmg;
    float   phasRatio, avgDmg;
    int     thisHit = 0;

    newtext[0] = '\0';
    *timer = udcounter + WARNTIME;	/* set the line to be cleared */
    
    if(!W_IsMapped(warnw)) return;

    if (*count > 0) {
	/* XFIX */
	W_ClearWindow(warnw);
        /*W_ClearArea(warnw, W_XOFF, W_YOFF, W_Textwidth * *count, W_Textheight);*/
	if(hudwarning)
          W_DirectMaskText(w, center - (*count / 2) * W_Textwidth, y, 
	                   backColor, save_buffer, *count, font);
    }
    *count = strlen(text);
    W_WriteText(warnw, W_XOFF, W_YOFF, textColor, text, *count, 
                font);
    if(hudwarning) {
      strcpy(save_buffer, text);
      W_DirectMaskText(w, center - (*count / 2) * W_Textwidth, y, 
                       color, save_buffer, *count, font);
#ifdef BUFFERING
      /* flush the buffer if there is one and we're not playing.  Flushing is
         too slow to do when playing - wait for the next update [BDyess] */
      if(me)
      if((me->p_status == PFREE ||
          me->p_status == POUTFIT) &&
	 W_IsBuffered(w))
        W_DisplayBuffer(w);
#endif /*BUFFERING [BDyess]*/
    }

    if (strncmp(text, "Phaser", 6) == 0) {
	if (strncmp(text + 7, "missed", 6) == 0) {
	    phasFired++;
	    if (!logPhaserMissed)
		return;
	    thisHit = 0;
	} else if (strncmp(text + 7, "burst", 5) != 0 &&
		   strncmp(text + 7, "shot", 4) != 0)
	    return;
	else {			/* a hit! */
	    phasFired++;
	    phasHits++;
	    thisHit = 1;
	}

	if (phaserStats) {
	    if (thisHit) {
		d = &text[strlen(text)];

		while (!isdigit(*d) && d > text)	/* find the last number
							   in the string, should
							   be damage */
		    d--;
		while (d > text && isdigit(*d))
		    d--;

		if (d > text) {
		    dmg = atoi(d);
		    totalDmg += dmg;
		    avgDmg = (float) totalDmg / (float) phasHits;
		    phasRatio = (100 * phasHits) / (float) phasFired;
		    sprintf(newtext, "Av:%5.2f, Hit:%5.2f%%: ", avgDmg, phasRatio);
		}
	    } else {		/* a miss */
		sprintf(newtext, "Hit: %d, Miss: %d, Dmg: %d: ", phasHits, phasFired - phasHits, totalDmg);
	    }
	} else {		/* not keeping phaser stats right now */
	    phasFired--;
	    if (thisHit)
		phasHits--;
	    newtext[0] = '\0';
	}
	strncat(newtext, text, 80);
	len = strlen(newtext);
	newtext[len++] = ' ';
	strcpy(newtext + len, timeString(time(NULL)));

	dmessage(newtext, 0, 254, 0);

    } else if (strncmp(text, "Missile away", 12) == 0) {
	/* missile total kludge.  No value until one is shot :( */
	me->p_totmissiles = atoi(text + 13);
    } else if (strcmp(text, "Prepping for warp jump") == 0) {
	/* keep track of when in warp prep */
	me->p_flags |= PFWARPPREP;
    } else if (strcmp(text, "Warp drive aborted") == 0) {
	me->p_flags &= ~PFWARPPREP;
    }
}
