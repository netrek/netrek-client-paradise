/*
 * getname.c
 *
 * Kevin P. Smith 09/28/88
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
#include "proto.h"

/* This should be in one of the above header files. */
#if 0
#ifndef __osf__
long    time();
#endif
#endif

static char tempname[16];
static char password1[16];
static char password2[16];
static int state, autolog;
static char username[32] = "****";
#define ST_GETNAME 0
#define ST_GETPASS 1
#define ST_MAKEPASS1 2
#define ST_MAKEPASS2 3
#define ST_DONE 4

/* Prototypes */
static void adjustString P((int ch, char *str, char *defname));
static void checkpassword P((void));
static void displayStartup P((char *defname));
static void loaddude P((void));
static void makeNewGuy P((void));
static void loginproced P((int ch, char *defname));

void
noautologin()
{
    char   *tempstr;

    autolog = 0;
    *defpasswd = *password1 = *password2 = '\0';
    tempstr = "Automatic login failed";
    W_WriteText(w, 100, 100, textColor, tempstr, (int)strlen(tempstr),
		W_BoldFont);
}

/* Let person identify themselves from w */
void
getname(defname, def_passwd)
    char   *defname, *def_passwd;
{
    W_Event event;
    register int ch = 0;
    int     secondsLeft = 99, laststate;
    char    tempstr[40];
    long    lasttime;
    register int j = 0;

#ifdef RECORDER
    if (playback)
	return;
#endif

    /* shows the credits in the map window [BDyess] */
    showCredits(mapw);

    autolog = (*def_passwd && *defname) ? 1 : 0;

    {
	struct passwd *passwd;

	passwd = getpwuid(getuid());
	if (passwd)		/* believe it or not, getpwuid failed on me -
				   RF */
	    strcpy(username, passwd->pw_name);
    }

    bzero(mystats, sizeof(struct stats));
    mystats->st_tticks = 1;
    mystats->st_flags =
	(ST_NOBITMAPS * (!sendmotdbitmaps) +
	 ST_KEEPPEACE * keeppeace +
	 0);
    lasttime = time(NULL);

    if (ghoststart)
	return;

    tempname[0] = '\0';
    password1[0] = '\0';
    password2[0] = '\0';
    laststate = state = ST_GETNAME;
    displayStartup(defname);
    for (;;) {
	if (isServerDead()) {
	    printf("Ack!  We've been ghostbusted!\n");
#ifdef AUTOKEY
	    if (autoKey)
		W_AutoRepeatOn();
#endif
	    EXIT(0);
	}
	if (lasttime != time(NULL)) {
	    lasttime++;
	    secondsLeft--;
	    if (!autolog) {
		sprintf(tempstr, "Seconds to go: %d ", secondsLeft);
		W_WriteText(w, 150, 400, textColor, tempstr, 
			    (int)strlen(tempstr), W_BoldFont);
#ifdef BUFFERING
		/* flush buffer if one exists [BDyess] */
		if(W_IsBuffered(w)) W_DisplayBuffer(w);	
#endif /*BUFFERING [BDyess]*/
	    }
	    if (secondsLeft == 0) {
		me->p_status = PFREE;
		printf("Auto-Quit\n");
#ifdef BUFFERING
		/* flush buffer if one exists [BDyess] */
		if(W_IsBuffered(w)) W_DisplayBuffer(w);	
#endif /*BUFFERING [BDyess]*/
#ifdef AUTOKEY
		if (autoKey)
		    W_AutoRepeatOn();
#endif
		EXIT(0);
	    }
	}
	if (state == ST_DONE) {
	    W_ClearWindow(w);
	    return;
	}
	readFromServer();	/* Just in case it wants to say something */

	if (autolog) {
	    switch (state) {
	    case ST_GETNAME:
		tempname[0] = '\0';
		ch = 13;
		j = 0;
		break;

	    case ST_GETPASS:
	    case ST_MAKEPASS1:
	    case ST_MAKEPASS2:
		ch = def_passwd[j++];
		if (ch == '\0') {
		    j = 0;
		    ch = 13;
		}
		break;

	    default:
		break;
	    }

	    loginproced(ch, defname);

	}
	laststate = state;

	if (!W_EventsPending())
	    continue;
	W_NextEvent(&event);
	if (event.Window == mapw && (int)event.type == W_EV_EXPOSE)
	    showCredits(mapw);
	if (event.Window != w)
	    continue;
	switch ((int) event.type) {
	case W_EV_EXPOSE:
            displayStartup(defname);
	    break;
	case W_EV_KEY:
	    ch = event.key;
	    if (!autolog)
		loginproced(ch, defname);
	}
    }
}


static
void
loginproced(ch, defname)
    int     ch;
    char   *defname;
{
    if (ch > 255)
	ch -= 256;		/* was alt key, ignore it */
    if (ch == 10)
	ch = 13;
    if ((ch == ('d' + 128) || ch == ('D' + 128)) && state == ST_GETNAME && *tempname == '\0') {
#ifdef AUTOKEY
	if (autoKey)
	    W_AutoRepeatOn();
#endif
	EXIT(0);
    }
    if ((ch < 32 || ch > 127) && ch != 21 && ch != 13 && ch != 8)
	return;
    switch (state) {
    case ST_GETNAME:
	if (ch == 13) {
	    if (*tempname == '\0') {
		strcpy(tempname, defname);
	    }
	    loaddude();
	    displayStartup(defname);
	} else {
	    adjustString(ch, tempname, defname);
	}
	break;
    case ST_GETPASS:
	if (ch == 13) {
	    checkpassword();
	    displayStartup(defname);
	} else {
	    adjustString(ch, password1, defname);
	}
	break;
    case ST_MAKEPASS1:
	if (ch == 13) {
	    state = ST_MAKEPASS2;
	    displayStartup(defname);
	} else {
	    adjustString(ch, password1, defname);
	}
	break;
    case ST_MAKEPASS2:
	if (ch == 13) {
	    makeNewGuy();
	    displayStartup(defname);
	} else {
	    adjustString(ch, password2, defname);
	}
	break;
    }
}

static void
loaddude()
/* Query dude.
 */
{
    if (strcmp(tempname, "Guest") == 0 || strcmp(tempname, "guest") == 0) {
	loginAccept = -1;
	sendLoginReq(tempname, "", username, 0);
	state = ST_DONE;
	me->p_pos = -1;
	me->p_stats.st_tticks = 1;	/* prevent overflow */
	strcpy(me->p_name, tempname);
	while (loginAccept == -1) {
	    socketPause(1, 0);
	    readFromServer();
	    if (isServerDead()) {
		printf("Server is dead!\n");
#ifdef AUTOKEY
		if (autoKey)
		    W_AutoRepeatOn();
#endif

		EXIT(0);
	    }
	}
	if (loginAccept == 0) {
	    printf("Hmmm... The SOB server won't let me log in as guest!\n");
#ifdef AUTOKEY
	    if (autoKey)
		W_AutoRepeatOn();
#endif

	    EXIT(0);
	}
	return;
    }
    /* Ask about the user */
    loginAccept = -1;
    sendLoginReq(tempname, "", username, 1);
    while (loginAccept == -1) {
	socketPause(1, 0);
	readFromServer();
	if (isServerDead()) {
	    printf("Server is dead!\n");
#ifdef AUTOKEY
	    if (autoKey)
		W_AutoRepeatOn();
#endif

	    EXIT(0);
	}
    }
    *password1 = *password2 = 0;
    if (loginAccept == 0) {
	state = ST_MAKEPASS1;
    } else {
	state = ST_GETPASS;
    }
}

static void
checkpassword()
/* Check dude's password.
 * If he is ok, move to state ST_DONE.
 */
{
    char   *s;

    sendLoginReq(tempname, password1, username, 0);
    loginAccept = -1;
    while (loginAccept == -1) {
	socketPause(1, 0);
	readFromServer();
	if (isServerDead()) {
	    printf("Server is dead!\n");
#ifdef AUTOKEY
	    if (autoKey)
		W_AutoRepeatOn();
#endif

	    EXIT(0);
	}
    }
    if (loginAccept == 0) {
	if (!autolog) {
	    s = "Bad password!";
	    W_WriteText(w, 100, 100, textColor, s, (int)strlen(s), W_BoldFont);
#ifdef BUFFERING
	    /* flush buffer if one exists [BDyess] */
	    if(W_IsBuffered(w)) W_DisplayBuffer(w);	
#endif /*BUFFERING [BDyess]*/
	    (void) W_EventsPending();
	    sleep(3);
	    W_ClearWindow(w);
	} else
	    noautologin();
	*tempname = 0;
	state = ST_GETNAME;
	return;
    }
    strcpy(me->p_name, tempname);
    sendmotdbitmaps = !((me->p_stats.st_flags / ST_NOBITMAPS) & 1);
    keeppeace = (me->p_stats.st_flags / ST_KEEPPEACE) & 1;
    state = ST_DONE;
}

static void
makeNewGuy()
/* Make the dude with name tempname and password password1.
 * Move to state ST_DONE.
 */
{
    char   *s;

    if (strcmp(password1, password2) != 0) {
	if (!autolog) {
	    s = "Passwords do not match";
	    W_WriteText(w, 100, 120, textColor, s, (int)strlen(s), W_BoldFont);
#ifdef BUFFERING
	    /* flush buffer if one exists [BDyess] */
	    if(W_IsBuffered(w)) W_DisplayBuffer(w);	
#endif /*BUFFERING [BDyess]*/
	    (void) W_EventsPending();
	    sleep(3);
	    W_ClearWindow(w);
	} else
	    noautologin();
	*tempname = 0;
	state = ST_GETNAME;
	return;
    }
    /* same routine! */
    checkpassword();
}

static void
adjustString(ch, str, defname)
    char    ch, *str;
    char   *defname;
{
    if (ch == 21) {
	*str = '\0';
	if (state == ST_GETNAME)
	    displayStartup(defname);
    } else if (ch == 8 || ch == '\177') {
	if ((int) strlen(str) > 0) {
	    str[strlen(str) - 1] = '\0';
	    if (state == ST_GETNAME)
		displayStartup(defname);
	}
    } else {
	if (strlen(str) == 15)
	    return;
	str[strlen(str) + 1] = '\0';
	str[strlen(str)] = ch;
	if (state == ST_GETNAME)
	    displayStartup(defname);
    }
}

static void
displayStartup(defname)
    char   *defname;
/* Draws entry screen based upon state. */
{
    char    s[100];
    char   *t;

    if (state == ST_DONE || autolog)
	return;
    t = "Enter your name.  Use the name 'guest' to remain anonymous.";
    W_WriteText(w, 100, 30, textColor, t, (int)strlen(t), W_BoldFont);
    t = "Type ^D (Ctrl - D) to quit.";
    W_WriteText(w, 100, 40, textColor, t, (int)strlen(t), W_BoldFont);
    sprintf(s, "Your name (default = %s): %s               ", defname, tempname);
    W_WriteText(w, 100, 50, textColor, s, (int)strlen(s), W_BoldFont);
    if (state == ST_GETPASS) {
	t = "Enter password: ";
	W_WriteText(w, 100, 60, textColor, t, (int)strlen(t), W_BoldFont);
    }
    if (state > ST_GETPASS) {
	t = "You need to make a password.";
	W_WriteText(w, 100, 70, textColor, t, (int)strlen(t), W_BoldFont);
	t = "So think of a password you can remember, and enter it.";
	W_WriteText(w, 100, 80, textColor, t, (int)strlen(t), W_BoldFont);
	t = "What is your password? :";
	W_WriteText(w, 100, 90, textColor, t, (int)strlen(t), W_BoldFont);
    }
    if (state == ST_MAKEPASS2) {
	t = "Enter it again to make sure you typed it right.";
	W_WriteText(w, 100, 100, textColor, t, (int)strlen(t), W_BoldFont);
	t = "Your password? :";
	W_WriteText(w, 100, 110, textColor, t, (int)strlen(t), W_BoldFont);
    }
#ifdef BUFFERING
    /* flush buffer if one exists [BDyess] */
    if(W_IsBuffered(w)) W_DisplayBuffer(w);	
#endif /*BUFFERING [BDyess]*/
}
