#include "config.h"
#ifdef XTREKRC_HELP
/*
 * taken from helpwin.c
 * (copyright 1991 ERic mehlhaff Free to use, hack, etc. Just keep
 *  these credits here. Use of this code may be dangerous to your health
 *  and/or system. Its use is at your own risk. I assume no responsibility for
 *  damages, real, potential, or imagined, resulting  from the use of it.)
 *
 */

#include "defines.h"
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <math.h>

#ifdef TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <sys/time.h>
#endif
#endif

#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "proto.h"
#include "data.h"
#if defined(HAVE_STRING_H)
#include <string.h>
#else
#include <string.h>
#endif

void def_write 
P((char *file));

/* this is the number of help messages there are */

#define INT_DEF		0
#define BOOL_DEF	1
#define STR_DEF		2
#define SINT_DEF	3

#define NAME_WIDTH	18
#define VAL_WIDTH	8
#define INDENT		3
#define MAX_VLINES	58

extern int updateSpeed;
#ifdef RECORD
extern char *recordFileName;
#endif

#define DEFMESSAGES	(sizeof(def_messages)/ sizeof(struct def))

char   *name = NULL, *cloak_chars = NULL, *bmap = NULL, *keymap = NULL, *plist = NULL, *log_file = NULL, *saveFileName = NULL;
int     galacticFrequent;

/* sure its a mess, but it gets the job done */

static
struct def {
    char   *name;
    int     type;
    char   *desc;
    void   *variable;

    struct {
	int     i_value;	/* if int or bool */
	char   *s_value;	/* if str */
	char   *desc;
    }       values[10];

    struct {			/* the area of the window this def takes up */
	int     x, y, rt, bot;
    }       loc;
}       def_messages[] = {

#ifdef AUTHORIZE
    {
	"useRSA", BOOL_DEF, "Use RSA checking",
	&RSA_Client,
        {
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
#endif
    {
	"showStats", BOOL_DEF, "Show stats window",
	&showStats,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"showShields", BOOL_DEF, "Show shields around ship",
	&showShields,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"keepPeace", BOOL_DEF, "Stay peaceful when reborn",
	&keeppeace,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"reportKills", BOOL_DEF, "Report kill messages",
	&reportKills,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
#if 0
    {
	"altBitmaps", BOOL_DEF, "Use alternate ship bitmaps",
	&blk_altbits,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
#endif
    {
	"showStars", BOOL_DEF, "Show star background on tactical",
	&blk_showStars,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"showMySpeed", BOOL_DEF, "Show speed next to ship",
	&showMySpeed,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"showTractorPressor", BOOL_DEF, "Show my tract/press",
	&showTractorPressor,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"showLock", INT_DEF, "Lock display for planets/players",
	&showLock,
	{
	    {
		0, NULL, "don't show lock"
	    },
	    {
		1, NULL, "show lock on galactic only"
	    },
	    {
		2, NULL, "show lock on tactical only"
	    },
	    {
		3, NULL, "show lock on both"
	    },
	    {
		0, NULL, NULL
	    },
	}
    },
    {
	"showGrid", BOOL_DEF, "Show grid on galactic",
	&drawgrid,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"Dashboard", INT_DEF, "Type of dashboard to use",
	&Dashboard,
	{
	    {
		0, NULL, "text based dashboard"
	    },
	    {
		1, NULL, "new dashboard"
	    },
	    {
		2, NULL, "color dashboard"
	    },
	    {
		3, NULL, "Rainbow Dashboard"
	    },
	    {
		0, NULL, NULL
	    },
	}
    },
    {
	"cloakChars", STR_DEF, "Cloak chars for map",
	&(cloak_chars),
	{
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"logging", BOOL_DEF, "Use message logging",
	&logmess,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"logFile", STR_DEF, "File to use for message logging",
	&(log_file),
	{
	    {
		0, NULL, NULL
	    },
	},
    },
#ifdef VARY_HULL
    {
	"warnHull", BOOL_DEF, "Warn hull state based on damage",
	&vary_hull,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
#endif
    {
	"warpStreaks", BOOL_DEF, "Streak stars when entering warp",
	&warpStreaks,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"useMsgw", BOOL_DEF, "Use message window",
	&use_msgw,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"showShieldDam", BOOL_DEF, "Vary shields based on damage",
	&show_shield_dam,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"updatesPerSec", SINT_DEF, "No. of updates from server per sec",
	&updateSpeed,
	{
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"redrawDelay", SINT_DEF, "Minimum time between redraws (x/10 sec)",
	&redrawDelay,
	{
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"extraAlertBorder", BOOL_DEF, "Show alert on local border",
	&extraBorder,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"galacticFrequent", BOOL_DEF, "Update galactic map frequently",
	&galacticFrequent,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
#ifdef CONTINUOUS_MOUSE
    {
	"continuousMouse", BOOL_DEF, "Continuous mouse input",
	&continuousMouse,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
#endif
    {
	"tryUdp", BOOL_DEF, "Try UDP automatically",
	&tryUdp,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"udpClientReceive", INT_DEF, "UDP receive mode",
	&udpClientRecv,
	{
	    {
		0, NULL, "TCP only"
	    },
	    {
		1, NULL, "simple UDP"
	    },
	    {
		2, NULL, "fat UDP"
	    },
	    {
		3, NULL, "double UDP (obsolete)"
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"udpClientSend", INT_DEF, "UDP send mode",
	&udpClientSend,
	{
	    {
		0, NULL, "TCP only"
	    },
	    {
		1, NULL, "simple UDP"
	    },
	    {
		2, NULL, "enforced UDP (state only)"
	    },
	    {
		3, NULL, "enforced UDP (state & weapon)"
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"udpSequenceCheck", BOOL_DEF, "UDP sequence checking",
	&udpSequenceChk,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"sortPlayers", BOOL_DEF, "Sort playerlist by teams",
	&sortPlayers,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"robsort", BOOL_DEF, "Put enemies on left in sorted playerlist",
	&robsort,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"hideNoKills", BOOL_DEF, "Replace 0.00 kills with spaces",
	&hideNoKills,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"showDead", BOOL_DEF, "Show dead in playerlist",
	&showDead,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"showPreLogins", BOOL_DEF, "Show pre-logins in playerlist",
	&showPreLogins,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"sortOutfitting", BOOL_DEF, "Sort outfitting ('--') to bottom",
	&sortOutfitting,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"timerType", INT_DEF, "Type of timer to use",
	&timerType,
	{
	    {
		0, NULL, "no timer"
	    },
	    {
		1, NULL, "time of day"
	    },
	    {
		2, NULL, "time on server"
	    },
	    {
		3, NULL, "time in ship"
	    },
	    {
		4, NULL, "user set timer"
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"showGalactic", INT_DEF, "Galactic planet bitmaps",
	&showgalactic,
	{
	    {
		0, NULL, "show nothing on galactic map"
	    },
	    {
		1, NULL, "show facilities on galactic map"
	    },
	    {
		2, NULL, "show owner on galactic map"
	    },
	    {
		3, NULL, "show surface properties on galactic map"
	    },
	    {
		4, NULL, "show scout info age on galactic map"
	    },
	    {
		5, NULL, "show MOO facilities on galactic map"
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"showLocal", INT_DEF, "Local planet bitmaps",
	&showlocal,
	{
	    {
		0, NULL, "show nothing on local map"
	    },
	    {
		1, NULL, "show facilities on local map"
	    },
	    {
		2, NULL, "show owner on local map"
	    },
	    {
		3, NULL, "show surface properties on local map"
	    },
	    {
		4, NULL, "show MOO facilities"
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"name", STR_DEF, "Default player name",
	&(name),
	{
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"keymap", STR_DEF, "Keyboard map",
	&(keymap),
	{
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"buttonmap", STR_DEF, "Mouse button map",
	&(bmap),
	{
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"nameMode", BOOL_DEF, "Show names on map/local",
	&namemode,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
#ifdef SHORT_PACKETS
    {
	"tryShort", BOOL_DEF, "Try SHORT-PACKETS at startup",
	&tryShort,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
#endif
#ifdef IGNORE_SIGNALS_SEGV_BUS
    {
	"ignoreSignals", BOOL_DEF, "Ignore SIGSEGV and SIGBUS",
	&ignore_signals,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
#endif
#ifdef SHIFTED_MOUSE
    {
	"shiftedMouse", BOOL_DEF, "More mouse buttons with shift",
	&extended_mouse,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
#endif
#ifdef BEEPLITE
    {
	"UseLite", BOOL_DEF, "Use message highliting",
	&UseLite,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
    {
	"DefLite", BOOL_DEF, "Use default lites",
	&DefLite,
	{
	    {
		0, NULL, ""
	    },
	    {
		0, NULL, NULL
	    },
	},
    },
#endif
    {
	"saveFileName", STR_DEF, "Name to save defaults as(click here)",
	&(saveFileName),
	{
	    {
		0, NULL, NULL
	    },
	},
    },
};

char   *
itos(v)
    int     v;
{
    static char value[10];
    sprintf(value, "%d", v);
    return value;
}

char   *
btoa(v)
    int     v;
{
    if (v)
	return "on";
    else
	return "off";
}

static void
def_redraw(d)
    struct def *d;
{
    int     xo = d->loc.x, yo = d->loc.y;
    int     x, y, j;
    char   *val;
    W_Color col;

    x = xo;
    y = yo;

    W_ClearArea(defWin, d->loc.x, d->loc.y, d->loc.rt - d->loc.x, d->loc.bot - d->loc.y);

    W_WriteText(defWin, x, y, W_Yellow, d->name, (int)strlen(d->name),
		W_RegularFont);
    x += NAME_WIDTH;

    W_WriteText(defWin, x, y, textColor, d->desc, (int)strlen(d->desc),
		W_RegularFont);
    y++;
    x = xo + INDENT;

    if (d->type != STR_DEF) {
	if (!d->values[0].desc && d->variable) {
	    if (d->type == SINT_DEF)
		val = itos(*(int *)d->variable);
	    else
		val = itos(d->values[0].i_value);

	    W_WriteText(defWin, x, y, W_Green, val, (int)strlen(val),
			W_RegularFont);
	    y++;
	}
	for (j = 0; d->values[j].desc; j++) {
	    switch (d->type) {
	    case INT_DEF:
		val = itos(d->values[j].i_value);
		if (d->values[j].i_value == *(int *)d->variable) {
		    col = W_Green;

		    W_WriteText(defWin, x, y, col, val, (int)strlen(val),
				W_RegularFont);
		    if (W_Mono()) {
			W_WriteText(defWin, x + 1, y, col, "*", 1,
				    W_RegularFont);
		    }
		} else {
		    col = textColor;

		    W_WriteText(defWin, x, y, col, val, (int)strlen(val),
				W_RegularFont);
		}
		x = xo + NAME_WIDTH;
		W_WriteText(defWin, x, y, col, d->values[j].desc,
			    (int)strlen(d->values[j].desc), W_RegularFont);
		y++;
		x = xo + INDENT;
		break;

	    case BOOL_DEF:
		val = btoa(*(int *)d->variable);
		W_WriteText(defWin, x, y, W_Green, val, (int)strlen(val),
			    W_RegularFont);
		y++;
		x = xo + INDENT;
		break;
	    default:
		fprintf(stderr, "Unknown type.\n");
		break;
	    }
	}
    } else if (d->variable && *(int *)d->variable) {
	W_WriteText(defWin, x, y, W_Green, (char *)*(int *)d->variable,
		    (int)strlen((char *)(*(int *)d->variable)),
		    W_RegularFont);
    }
}


void
showdef()
{
    register int i, j, x = 0, y = 0, xo = 0, yo = 0, max_desc = 0, height = 1, width = 1;
    register struct def *d;
    char   *val;
    W_Color col;

    name = stringDefault("name",NULL);
    keymap = stringDefault("keymap",NULL);
    cloak_chars = cloakchars;
    bmap = stringDefault("buttonmap",NULL);
    log_file = stringDefault("logfile",NULL);
    galacticFrequent = (mapmode == 2) ? 0 : 1;
    if (!saveFileName) {
	saveFileName = stringDefault("saveFileName", "~/.paradisesaverc");
	saveFileName = expandFilename(saveFileName);
    }
    if (!defWin)
	defWin = W_MakeTextWindow("xtrekrc_help", 1, 100, 174, 60, NULL, NULL, BORDER);

    for (i = 0, d = def_messages; i < DEFMESSAGES; i++, d++) {
	x = xo;
	y = yo;

	d->loc.x = x;
	d->loc.y = y;

	W_WriteText(defWin, x, y, W_Yellow, d->name, (int)strlen(d->name),
		    W_RegularFont);
	x += NAME_WIDTH;

	W_WriteText(defWin, x, y, textColor, d->desc, (int)strlen(d->desc),
		    W_RegularFont);
	if ((int)strlen(d->desc) > max_desc) {
	    max_desc = strlen(d->desc);
	    width = MAX(width, x + max_desc);
	}
	y++;
	x = xo + INDENT;

	if (d->type != STR_DEF) {
	    if (!d->values[0].desc && d->variable) {
		if (d->type == SINT_DEF)
		    val = itos(*(int *)d->variable);
		else
		    val = itos(d->values[0].i_value);

		W_WriteText(defWin, x, y, W_Green, val, (int)strlen(val),
			    W_RegularFont);
		y++;
	    }
	    for (j = 0; d->values[j].desc; j++) {
		switch (d->type) {
		case INT_DEF:
		    val = itos(d->values[j].i_value);
		    if (d->values[j].i_value == *(int *)d->variable) {
			col = W_Green;

			W_WriteText(defWin, x, y, col, val, (int)strlen(val),
				    W_RegularFont);
			if (W_Mono()) {
			    W_WriteText(defWin, x + 1, y, col, "*", 1,
					W_RegularFont);
			}
		    } else {
			col = textColor;

			W_WriteText(defWin, x, y, col, val, (int)strlen(val),
				    W_RegularFont);
		    }
		    x = xo + NAME_WIDTH;
		    W_WriteText(defWin, x, y, col, d->values[j].desc,
				(int)strlen(d->values[j].desc), W_RegularFont);
		    y++;
		    x = xo + INDENT;
		    break;

		case BOOL_DEF:
		    val = btoa(*(int *)d->variable);
		    W_WriteText(defWin, x, y, W_Green, val, (int)strlen(val),
				W_RegularFont);
		    y++;
		    x = xo + INDENT;
		    break;
		default:
		    fprintf(stderr, "Unknown type.\n");
		    break;
		}
	    }
	} else if (d->variable && *(int *)d->variable) {
	    W_WriteText(defWin, x, y, W_Green, (char *)*(int *)d->variable,
			(int)strlen((char *)(*(int *)d->variable)),
			W_RegularFont);
	    y++;
	}
	d->loc.rt = xo + max_desc;
	d->loc.bot = y + 1;

	height = MAX(height, y);
	if (y > MAX_VLINES) {
	    yo = 0;
	    xo += NAME_WIDTH + max_desc + 2;
	    max_desc = 0;
	} else {
	    yo = y + 1;
	}
    }

    if (!W_IsMapped(defWin)) {
	W_ResizeText(defWin, width, height);
	W_MapWindow(defWin);
    }
}

void
def_action(ev)
    W_Event *ev;
{
    int     i, j, x, y, line;
    register struct def *d;

    x = ev->x;
    y = ev->y;
    W_TranslatePoints(ev->Window, &x, &y);

    for (i = 0, d = def_messages; i < DEFMESSAGES; i++, d++) {
	if (y >= d->loc.y && y < d->loc.bot &&
	    x >= d->loc.x && x < d->loc.rt)
	    break;		/* found it! */
    }

    if (i >= DEFMESSAGES)
	return;

    line = y - d->loc.y;

    switch (ev->type) {
    case W_EV_BUTTON:
	switch (d->type) {
	case BOOL_DEF:
	    *(int *)d->variable = !(*(int *)d->variable);
	    def_redraw(d);
	    break;
	case INT_DEF:
	case SINT_DEF:
	    if (line == 0 || d->type == SINT_DEF) {
		switch (ev->key) {
		case W_LBUTTON:
		    (*(int *)d->variable)++;
		    if (!(*(int *)d->values[*(int *)d->variable].desc))
			*(int *)d->variable = 0;
		    break;
		case W_RBUTTON:
		    (*(int *)d->variable)--;
		    if (*(int *)d->variable < 0) {
			for (j = 0; d->values[j].desc; j++)
			     /* empty */ ;
			*(int *)d->variable = j - 1;
		    }
		    break;
		case W_MBUTTON:
		    *(int *)d->variable = 0;
		    break;
		default:
		    break;
		}
	    } else if (y < d->loc.bot)
		*(int *)d->variable = line - 1;
	    def_redraw(d);
	    break;
	case STR_DEF:
	    if (d->variable == &saveFileName)
		def_write(saveFileName);
	    break;
	default:
	    break;
	}
    }
}

void
def_write(file)
    char   *file;
{
    int     i;
    struct def *d;
    FILE   *f;

    f = fopen(file, "w");


    for (i = 0, d = def_messages; i < DEFMESSAGES; i++, d++) {
	switch (d->type) {
	case INT_DEF:
	case SINT_DEF:
	    fprintf(f, "%s: %d\n", d->name, *(int *)d->variable);
	    break;
	case BOOL_DEF:
	    fprintf(f, "%s: %s\n", d->name, (*(int *)d->variable ? "on" : "off"));
	    break;
	case STR_DEF:
	    fprintf(f, "%s: %s\n", d->name, (char *)*(int *)d->variable);
	    break;
	}
    }

    fclose(f);
}


#endif
