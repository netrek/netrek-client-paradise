/*
 * redraw.c
 */
#include "copyright.h"
#include "defines.h"

#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#ifdef STDC_HEADERS
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#include <math.h>
#ifdef HAVE_TIME_H
#include <time.h>
#if defined(HAVE_SYS_TIME_H) && defined(TIME_WITH_SYS_TIME)
#include <sys/time.h>
#endif
#else
#include <sys/time.h>
#endif
#include "config.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "packets.h"
#include "proto.h"
#include "gameconf.h"
#include "images.h"
#ifdef SOUND
#include "Slib.h"
#endif

void local P((void));

/* since WINSIDE and MAPSIDE are now variable, this function is used to
   initialize some constants that use them.  Most common computations
   involving WINSIDE and MAPSIDE are done once, here, and the result
   used over and over throughout the rest of the code.  [BDyess] */
void recalcWindowConstants() 
{
  if(blk_zoom) {
    gwidth = blk_gwidth / 2;
    offsetx = zoom_offset(me->p_x);
    offsety = zoom_offset(me->p_y);
  } else {
    gwidth = blk_gwidth;
    offsetx = 0;
    offsety = 0;
  }
  /* keeps the same amount of info on the tactical no matter how large
     it is. [BDyess] */
  scale = SCALE * WINSIDE / winside;
  /*scale = gwidth / winside;*/
  center = winside/2;
  /* 'view' is the number of server units that the window represents [BDyess] */
  fullview = scale * winside;
  view = fullview/2;
  mapscale = gwidth / mapside;
/*  printf("after recalc: gwidth = %d, offsetx = %d, offsety = %d, scale = %d, center = %d,\nfullview = %d, view = %d, mapscale = %d.\n",gwidth,offsetx,offsety,scale,center,fullview,view,mapscale);*/
}


struct _clearzone *
new_czone()
{
    if (clearzone == 0) {
	czsize = 10;
	clearzone = (struct _clearzone *) malloc(sizeof(*clearzone) * czsize);
	clearcount = 0;
    } else if (clearcount >= czsize) {
	clearzone = (struct _clearzone *) realloc(clearzone,
					sizeof(*clearzone) * (czsize *= 2));
    }
    return &clearzone[clearcount++];
}

void
intrupt()
{
#if 0
    static long lastread;
#endif /*0*/
    static struct timeval lastredraw = {0, 0};
    struct timeval t;
    static int needredraw=0;

/* Same thing.  Include time.h, sys/time.h at top of file. */
#if 0
#ifndef __osf__
    long    time();
#endif
#endif

    udcounter++;
    
    needredraw += readFromServer();

    gettimeofday(&t, NULL);
    if (needredraw && ((unsigned int) redrawDelay * 100000 <
                       (unsigned int) ((t.tv_sec - lastredraw.tv_sec) * 1000000
                                       + (t.tv_usec - lastredraw.tv_usec)))) {
	needredraw = 0;
        lastredraw=t;

#ifdef RECORDER
	if (!playback || (pb_update && 
			  ((!pb_scan) || !(udcounter % pb_advance))))
#endif
	{
	    redraw();
	    playerlist2();
	}
#ifdef RECORDER
	if (playback) {
	    if(!pb_scan) {
		if (pb_advance > 0) {
		    if ((pb_advance -= pb_update) <= 0) {
			pb_advance = 0;
			paused = 1;
		    }
		} else if (pb_advance < 0) {
		    switch (pb_advance) {
		    case PB_REDALERT:
			if (me->p_flags & PFRED) {
			    pb_advance = 0;
			    paused = 1;
			}
			break;
		    case PB_YELLOWALERT:
			if (me->p_flags & PFYELLOW) {
			    pb_advance = 0;
			    paused = 1;
			}
			break;
		    case PB_DEATH:
		    default:
			if (me->p_status != PALIVE) {
			    pb_advance = 0;
			paused = 1;
			}
			break;
		    }
		}
	    }
	    pb_update = 0;
	}
	if (recordGame)
	    writeUpdateMarker();
#endif
    }
#if 0
    if (lastread + 3 < time(NULL)) {
	/*
	   We haven't heard from server for awhile... Strategy:  send a
	   useless packet to "ping" server.
	*/
	sendWarReq(me->p_hostile);
    }
#endif
    if (me->p_status == POUTFIT) {
	death();
    }
}

void
redraw()
{

    /* erase warning line if necessary */
    if ((warntimer <= udcounter && hwarncount <= udcounter) &&
        (warncount || hwarncount)) {
      W_ClearWindow(warnw);
    }
    if (warntimer <= udcounter && warncount) {
       W_MaskText(w, center - (warncount / 2) * W_Textwidth, 
                  winside - W_Textheight - HUD_Y, backColor, warningbuf, 
		  warncount, W_RegularFont);
       warncount = 0;
    }
    if (hwarntimer <= udcounter && hwarncount) {
       W_MaskText(w, center - (hwarncount / 2) * W_Textwidth, HUD_Y, 
                  backColor, hwarningbuf, hwarncount, W_BoldFont);
       hwarncount = 0;
    }
      
#if 0
    if ((warntimer <= udcounter) && (warncount > 0)) {
#ifndef AMIGA
	/* XFIX */
	W_ClearArea(warnw, 5, 5, W_Textwidth * warncount, W_Textheight);
#else
	W_ClearWindow(warnw);
#endif
	warncount = 0;
    }
#endif /*0*/

#ifdef BUFFERING
    if(W_IsBuffered(w)) {   /* buffered, clear the entire buffer [BDyess] */
      W_ClearBuffer(w);
      clearcount = 0;
      clearlcount = 0;
      tractcurrent = tracthead;
    } else
#endif /*BUFFERING [BDyess]*/

    {
      if (W_FastClear) {
	  W_ClearWindow(w);
	  clearcount = 0;
	  clearlcount = 0;
	  tractcurrent = tracthead;
      } else {
#if 0
	  /* just to save a little time on all the function calls */
	  /* all the normal clear functions are implemented as well. */
	  W_FlushClearZones(w, clearzone, clearcount);
	  clearcount = 0;
#else
	  while (clearcount) {
	      clearcount--;
	      /* XFIX */
	      W_CacheClearArea(w, clearzone[clearcount].x,
			 clearzone[clearcount].y, clearzone[clearcount].width,
			       clearzone[clearcount].height);

	      /*
		 W_ClearArea(w, clearzone[clearcount].x,
		 clearzone[clearcount].y, clearzone[clearcount].width,
		 clearzone[clearcount].height);
	      */
	  }
#endif
	  while (clearlcount) {
	      clearlcount--;
	      /* XFIX */
	      W_CacheLine(w, clearline[0][clearlcount],
			  clearline[1][clearlcount], clearline[2][clearlcount],
			  clearline[3][clearlcount], backColor);
	      /*
		 W_MakeLine(w, clearline[0][clearlcount],
		 clearline[1][clearlcount], clearline[2][clearlcount],
		 clearline[3][clearlcount], backColor);
	      */
	  }
	  /* XFIX */
#ifndef AMIGA
	  W_FlushClearAreaCache(w);
	  W_FlushLineCaches(w);
#endif
	  /* erase the tractor lines [BDyess] */
	  for (tractrunner = tracthead; tractrunner != tractcurrent;
	       tractrunner = tractrunner->next) {
	      W_MakeTractLine(w, tractrunner->sx, tractrunner->sy,
			      tractrunner->d1x, tractrunner->d1y,
			      backColor);
	      W_MakeTractLine(w, tractrunner->sx, tractrunner->sy,
			      tractrunner->d2x, tractrunner->d2y,
			      backColor);
	  }
	  tractcurrent = tracthead;
      }
    }

    local();			/* redraw local window */

    /* XFIX */
    W_FlushLineCaches(w);

    stline(0);			/* draw dashboard [BDyess] */

#ifdef BUFFERING
    /* flush buffers [BDyess] */
    W_DisplayBuffer(tstatw);	/* dashboard [BDyess] */
    W_DisplayBuffer(w);		/* local [BDyess] */
#endif /*BUFFERING*/

				/* would do it in W_EventsPending, just have
				   it here so the display is updated sooner. */
    W_Flush();

    if (W_IsMapped(statwin)) {
	updateStats();
    }
    if (W_IsMapped(newstatwin)) {
	updateNewStats();
    }

    updateInform();		/* check and update info window [BDyess] */

    /* XFIX: last since its least accurate information */
    if (mapmode) {
      map();
#ifdef BUFFERING
      /* write the buffered data, if any [BDyess] */
      W_DisplayBuffer(mapw);
#endif /*BUFFERING [BDyess]*/
    }
}

#define DRAWGRID		4

int
zoom_offset(v)
    int     v;
{
    int ov;
    /* offset to bring us into new zoom area */
    ov = (v / (int) gwidth) * gwidth;
    if (blk_zoom > 1)
	return ov;
    else
	/* sector offset into new zoom area */
	return ov + (((v - ov) / GRIDSIZE) * GRIDSIZE) - GRIDSIZE;
}
