/*
 * main.c
 */
#include "copyright.h"

#include "defines.h"
#include <stdio.h>
#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#include <sys/types.h>
#include <signal.h>
#include <setjmp.h>
#include <pwd.h>
#ifdef STDC_HEADERS
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_WAIT_H
#include <wait.h>
#endif
#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif
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
#ifdef SOUND
#include "Slib.h"
#endif
#include "sound.h"
#include "version.h"

jmp_buf env;

#ifdef AMIGA
/* needed for metafork... implemented by running a whole new copy
   don't honestly know why I bothered ;-) */
char **command_line;
int global_argc;
#endif

#ifdef GATEWAY
#define DEFAULT_GATEWAY		"atlgw"	/* for Quar */

static char *get_gw P((void));
static unsigned long mkaddr P((char *m));
static void getUdpPort P((void));
#endif

/* Prototypes */
static void printUsage P((char *prog));
void checkExpire P((int verbose));

extern int xpmORplanes;	/* from x11window.c */

int
main(argc, argv)
    int     argc;
    char  **argv;
{
    int     team, s_type;
    char   *dpyname = NULL;
    int     usage = 0;
    int     err = 0;
    char   *name, *ptr, *cp;
/* 
  Bogus.  Function prototypes within function decl's. are GONE.
  This should be fixed by STRING_H and STRINGS_H anyway.
  Besides, if strchr and strrchr exist, use them instead of index()
  and rindex().
#if 0
#if !defined(NeXT) && !defined(RS6K) && !defined(S-VR4)
    char   *rindex();
#endif
#endif
*/
    struct passwd *pwent;
    int     passive = 0;
#ifdef XPM
    int     xpmopt = 0;
    int     useORopt = 0;
    int     useCookieOpt = 0;
    int     dontUseCookieOpt = 0;
#endif /*XPM [BDyess]*/
/*    char *defaultsFile=NULL;*/

    pseudo[0] = defpasswd[0] = '\0';

#ifdef AMIGA
    command_line = argv;
    global_argc=argc;
#endif

    name = *argv++;
    argc--;
    if ((ptr = strrchr(name, '/')) != NULL)
	name = ptr + 1;
#ifdef GATEWAY
    netaddr = -1;		/* special NULL address */
    serverName = get_gw();	/* default machine is gw */
#endif
    while (*argv) {
	if (**argv != '-') {
	    serverName = *argv;	/* don't abort argument processing */
	    argv++;
	    argc--;
	} else {
	    ++*argv;

	    argc--;
	    ptr = *argv++;
	    while (*ptr) {
		switch (*ptr) {
		case 'C':	/* character name */
		    (void) strncpy(pseudo, *argv, sizeof(pseudo));
		    argv++;
		    argc--;
		    break;

		case 'P':	/* authorization password */
		    (void) strncpy(defpasswd, *argv, sizeof(defpasswd));
		    {
			int     i;
			for (i = 0; (*argv)[i]; i++)
			    (*argv)[i] = 0;
		    }
		    argv++;
		    argc--;
		    break;

		case 'u':
		    usage++;
		    break;
		case 's':
		    if (*argv) {
			xtrekPort = atoi(*argv);
			passive = 1;
			argv++;
			argc--;
		    }
		    break;
		case 'p':
		    if (*argv) {
			xtrekPort = atoi(*argv);
			argv++;
			argc--;
		    }
		    break;
		case 'd':
		    dpyname = *argv;
		    argc--;
		    argv++;
		    break;
#ifdef METASERVER
		case 'm':
		    usemeta = 1;
		    break;
#endif				/* METASERVER */
		case 'h':
		    serverName = *argv;
#ifdef GATEWAY
		    gw_mach = *argv;
#endif
		    argc--;
		    argv++;
		    break;
#ifdef GATEWAY
		case 'H':
		    netaddr = mkaddr(*argv);
		    argc--;
		    argv++;
		    break;
#endif

		case 't':
		    title = *argv;
		    argc--;
		    argv++;
		    break;
		case 'r':
		    defaultsFile = *argv;
		    argv++;
		    argc--;
		    break;
#ifdef AUTHORIZE
		case 'o':
		    RSA_Client = -1;
		    break;
		case 'R':
		    RSA_Client = -2;
		    break;
#else
		case 'o':
		case 'R':
		    printf("This client does not have binary authorization.\n");
		    break;
#endif
		case 'e':
#ifdef AUTHORIZE
		    checkExpire(1);
#else
		    printf("This client does not RSA verify and will not expire.\n");
#endif
		    exit(0);
		    break;
		case 'f':	/* list ftp sites */
		    fprintf(stderr, "\n\
The newest version of the Paradise client can be found at:\n\
      ftp.cs.umn.edu  in /users/glamm/paradise/bin/\n\
or    ftp.reed.edu    in /mirrors/netrek.paradise/\n\
\n\
Quick ftp instructions:\n\
This assumes you are bob@school.edu and are using a Sun workstation\n\
('sparc' architecture).  Modify with your mailing address and architecture.\n\
Type:\n\
   ftp ftp.cs.umn.edu\n\
   (at name prompt) anonymous\n\
   (at password prompt) bob@school.edu\n\
   (at ftp> prompt) cd /users/glamm/paradise/bin\n\
   (at ftp> prompt) binary\n\
   (at ftp> prompt) dir\n\
   (lots of files printed - pick out the latest release for your architecture.\n\
    all the client binaries begin with 'ntparadise'.)\n\
   (at ftp> prompt) get netrek.Sparc-SunOS-static\n\
   (note: static and dynamic are functionally the same.)\n\
  (at ftp> prompt) bye\n\
\n\
That's it!\n");
		    exit(0);
		case 'G':
		    if (*argv) {
			ghoststart++;
			ghost_pno = atoi(*argv);
			printf("Emergency restart being attempted...\n");
			argv++;
			argc--;
		    }
		    break;
		case '2':	/* force paradise */
		    paradise = 1;
		    break;
#ifdef RECORDER
		case 'F':	/* File playback */
		    if (*argv) {
			playFile = strdup(*argv);
			playback = 1;
			argv++;
			argc--;
		    }
		    break;
#endif
#ifdef XPM
		case 'x':
		    xpmopt = 1;
		    break;
		case 'k':		/* cookie mode [BDyess] */
		    useCookieOpt = 1;
		    break;
		case 'K':		/* no-cookies :( [BDyess] */
		    dontUseCookieOpt = 1;
		    break;
		case 'v':
		    verbose_image_loading = 1;
		    break;
		case 'O':		/* turn on GXor image drawing [BDyess]*/
		    useORopt = 1;	
		    break;
#ifdef HACKED_XPMLIB
		case 'b':		/*specify number of bitplanes [BDyess]*/
		    if(*argv) {
		      xpmORplanes = atoi(*argv);
		      argv++;
		      argc--;
		    }
		    break;
#endif /*HACKED_XPMLIB*/
#endif /*XPM [BDyess]*/
                case 'c': 	/* dump .paradiserc defaults [BDyess] */
		    dump_defaults = 1;
		    break;
		default:
		    fprintf(stderr, "%s: unknown option '%c'\n", name, *ptr);
		    err++;
		    break;
		}
		ptr++;
	    }
	}
    }
#ifdef GATEWAY
    if (netaddr == -1) {
	fprintf(stderr,
		"netrek: no remote address set (-H).  Restricted server will not work.\n");
    }
#endif



    inittrigtables();

    initStars();		/* moved from redraw.c at KAO\'s suggestion */

    if (usage || err) {
	printUsage(name);
#ifdef AUTHORIZE
	checkExpire(1);
#endif
	exit(0);
	/* exit(err); Exits from checkExpire */
    }
    defaultsFile = initDefaults(defaultsFile);

#ifdef XPM
    if(xpmopt) xpm = 1;
    else xpm = booleanDefault("xpm",xpm);
    if(xpm) printf("XPM mode enabled.\n");
    /* command line option overrides .paradiserc value [BDyess] */
    if(useORopt) useOR = 1;
    else useOR = booleanDefault("useOR",useOR);
    if(useOR) printf("OR mode enabled.\n");
    if(useOR || !xpm) cookie = 0;	/* default no-cookies unless in XPM
    					   mode w/out OR [BDyess] */
					/* note: need a milk mode :) */
    if(useCookieOpt) cookie = 1;
    else if(dontUseCookieOpt) cookie = 0;
    else cookie = booleanDefault("cookie",cookie);
    if(cookie) printf("Cookie mode enabled.\n");
#endif /*XPM [BDyess]*/

#ifdef AUTHORIZE
    if (RSA_Client != -1)
	checkExpire(0);
#endif

    /* compatability */
    if (argc > 0)
	serverName = argv[0];

    srandom((unsigned)(getpid() + time((long *) 0)));

#ifdef RECORDER
    if(playback || booleanDefault("playback",0)) {
        defNickName = "playback";
#ifdef METASERVER
	usemeta=0;
#endif /*METASERVER*/
        serverName = "playback";
    } else
#endif
    {
        if (serverName) {
	    char    temp[80], *s;
	    sprintf(temp, "server.%s", serverName);
	    if ((s = stringDefault(temp,NULL))) {
		printf("Using nickname \"%s\" for server %s\n", serverName, s);
		defNickName = serverName;
		serverName = s;
		defFlavor = stringDefault("flavor",NULL);
	    }
	}
	if (!serverName) {
	    serverName = stringDefault("server",NULL);
        }
	if (!serverName && !passive) {
	    serverName = DEFAULT_SERVER;
#ifdef METASERVER
	    usemeta = 1;		/* no server specified, show the menu */
#endif
	}
	if (passive)
	    serverName = "passive";	/* newwin gets a wrong title otherwise */

	if (xtrekPort < 0)
	    xtrekPort = intDefault("port", -1);
	if (xtrekPort < 0)
	    xtrekPort = DEFAULT_PORT;
#if 0
#ifdef AUTHORIZE
	if (RSA_Client >= 0)
	    RSA_Client = booleanDefault("useRSA", RSA_Client);
	else
	    RSA_Client = (RSA_Client == -2);
#endif
#endif				/* 0 */

    } /* playback */
    build_default_configuration();

#ifdef METASERVER
    metaserverAddress = stringDefault("metaserver",
				      "metaserver.netrek.org");
    if (usemeta)
	openmeta();
#endif				/* METASERVER */

    W_Initialize(dpyname);
#ifdef SOUND
    S_Initialize();
#endif


#ifdef METASERVER
    metaFork = booleanDefault("metaFork", metaFork);
    /* do the metawindow thang */
    if (usemeta) {
	metawindow();
	metainput();
	if (metaFork)
	    W_Initialize(dpyname);
	newwin(dpyname, name);
    } else
#endif				/* METASERVER */

	/* this creates the necessary x windows for the game */
	newwin(dpyname, name);

#ifdef TIMELORD
    start_timelord();
#endif

    /* open memory...? */
    openmem();
#ifdef RECORDER
    if (!startPlayback())
#endif
    {
	if (!passive) {
	    serverName = callServer(xtrekPort, serverName);
	} else {
	    connectToServer(xtrekPort);
	}
    }
#ifdef FEATURE
    sendFeature("FEATURE_PACKETS", 'S', 1, 0, 0);
#endif

    timeStart = time(NULL);
    findslot();

    /* sets all the settings from defaults file (.xtrekrc probably) */
    resetDefaults();

#ifdef UNIX_SOUND
    init_sound();
    play_sound(SND_PARADISE);
#endif

    mapAll();
/*    signal(SIGINT, SIG_IGN);*/
    signal(SIGCHLD, (void (*) ()) reaper);

    /* Get login name */
    if ((pwent = getpwuid(getuid())) != NULL)
	(void) strncpy(login, pwent->pw_name, sizeof(login));
    else
	(void) strncpy(login, "Bozo", sizeof(login));
    login[sizeof(login) - 1] = '\0';

    if (pseudo[0] == '\0') {
	char *freeme;
	strncpy(pseudo, freeme = stringDefault("name",login), sizeof(pseudo));
	free(freeme);
    }
    pseudo[sizeof(pseudo) - 1] = '\0';

    if (defpasswd[0] == '\0') {
	char buf[100];  /* added password by character name -JR */
	sprintf(buf,"password.%s",pseudo);
	if((cp = stringDefault(buf,NULL)) || (cp = stringDefault("password",NULL)))
	    (void) strncpy(defpasswd, cp, sizeof(defpasswd));
    }
    defpasswd[sizeof(defpasswd) - 1] = '\0';

    /*
       sendLoginReq("Gray Lensman", "hh", "sfd", 0); loginAccept = -1; while
       (loginAccept == -1) { socketPause(1,0); readFromServer(); }
    */
    getname(pseudo, defpasswd);
    loggedIn = 1;
#ifdef TIMER
    timeBank[T_SERVER] = timeStart;
    timeBank[T_DAY] = 0;
#endif				/* TIMER */


#ifdef AUTOKEY
    /* autokey.c */
    autoKeyDefaults();
#endif				/* AUTOKEY */

    /*
       Set p_hostile to hostile, so if keeppeace is on, the guy starts off
       hating everyone (like a good fighter should)
    */
    me->p_hostile = (1 << number_of_teams) - 1;

    redrawTstats();

    me->p_planets = 0;
    me->p_genoplanets = 0;
    me->p_armsbomb = 0;
    me->p_genoarmsbomb = 0;
    /* Set up a reasonable default */
    me->p_whydead = KNOREASON;
    me->p_teami = -1;
    s_type = defaultShip(CRUISER);	/* from rlb7h 11/15/91 TC */

    if (booleanDefault("netStats", 1))
	startPing();		/* tell the server that we support pings */

#ifdef AUTOKEY
    if (autoKey) {
	/* XX: changes entire state of display */
	W_AutoRepeatOff();
    }
#endif
    /*
       hack to make galaxy class ships work.  This could be more elegant, but
       the configuration code would have to be modified quite a bit, since
       the client doesn't know if it's on a paradise server until after it
       connects, and it needs the configuration info before it connects.
    */
    init_galaxy_class();

    initkeymap(-1);		/* needs to have ship types initialized -JR */

    setjmp(env);		/* Reentry point of game */

    if (ghoststart) {
	int     i;

	ghoststart = 0;

	for (i = -1; i < 5; i++)
	    if (teaminfo[i].letter == me->p_mapchars[0])
		break;

	me->p_teami = i;

	if (me->p_damage > me->p_ship->s_maxdamage) {
	    me->p_status = POUTFIT;
	} else
	    me->p_status = PALIVE;
    } else
	me->p_status = POUTFIT;

    while (1) {
	switch (me->p_status) {
	case POUTFIT:
	case PTQUEUE:
	    /* give the player the motd and find out which team he wants */
#if 1
	    new_entrywindow(&team, &s_type);
#else
	    entrywindow(&team, &s_type);
#endif
	    allowPlayerlist = 1;
	    if (W_IsMapped(playerw))
		playerlist();

#ifdef RECORDER
	    if (!playback)
#endif
		if (team == -1) {
		    W_DestroyWindow(w);
#ifdef AUTOKEY
		    if (autoKey)
			W_AutoRepeatOn();
#endif
		    sendByeReq();
		    sleep(1);
		    printf("OK, bye!\n");
		    EXIT(0);
		}
	    sendVersion();
	    myship = getship(myship->s_type);

	    currentship = myship->s_type;

	    /*
	       sendOptionsPacket(); this would totally blast any flags you
	       had on the server
	    */

	    redrawall = 1;
	    enter();
	    calibrate_stats();
	    W_ClearWindow(w);
	    /*
	       for (i = 0; i < NSIG; i++) { signal(i, SIG_IGN); }
	    */

	    me->p_status = PALIVE;	/* Put player in game */

#ifdef UNIX_SOUND
            kill_sound ();
#endif
            
#ifdef HOCKEY
	    hockeyInit();
#endif /*HOCKEY*/

#ifdef TIMER
	    timeBank[T_SHIP] = time(NULL);
#endif				/* TIMER */

	    if (showStats)	/* Default showstats are on. */
		W_MapWindow(statwin);
	    if (showNewStats)	/* default showNewStats are off. [BDyess] */
	        W_MapWindow(newstatwin);

#ifdef GATEWAY
	    /* pick a nice set of UDP ports */
	    getUdpPort();
#endif

	    if (tryUdp && commMode != COMM_UDP) {
		sendUdpReq(COMM_UDP);
	    }

#ifdef SHORT_PACKETS
	    if (tryShort) {
		sendShortReq(SPK_VON);
		tryShort = 0;	/* only try it once */
	    }
#endif	/* SHORT_PACKETS */
	    /* Send request for a full update */
	    if (askforUpdate) {
		if(recv_short)
		    sendShortReq(SPK_SALL);
		else
		    sendUdpReq(COMM_UPDATE);
	    }
	    sendUpdatePacket(1000000 / updateSpeed);

	    W_Deiconify(baseWin);

	    break;
	case PALIVE:
	case PEXPLODE:
	case PDEAD:
	case POBSERVE:

#ifdef TIMELORD
	    /* reading the MOTD doesn't count against your playing time */
	    update_timelord_notcount();
#endif

	    /* Get input until the player quits or dies */
	    input();
	    W_ClearWindow(mapw);
#ifdef TIMELORD
	    /* get any fractional minutes we missed */
	    update_timelord(1);
#endif
	    break;
	default:
	    printf("client has p_status=%d.  how strange\n", me->p_status);
	    me->p_status = POUTFIT;
	}
    }

    /* NOTREACHED */
}

static void
printUsage(prog)
    char   *prog;
{
    fprintf(stderr, "Usage:\n  %s [ options ] [ ntserv-host ]\n\
Where options are\n\
    [-h] host          server host name\n\
    [-p] port          server port number\n\
    [-r] xtrekrc       defaults file to replace ~/.xtrekrc\n\
    [-t] title         window manager title\n\
    [-d] display       set Xwindows display\n\
    [-C] name          netrek pseudonym\n\
    [-P] passwd        passwd to use to attempt autologin\n\
    [-R]               use RSA authorization (default)\n\
    [-o]               use old (non-RSA) authorization\n\
    [-s] port          wait for connection from ntserv on a port (debugging)\n\
%s\
%s\
    [-e]               check the expire time on the client\n\
    [-f]               how to get the newest client\n\
    [-u]               print usage\n\
    [-c]               dump .paradiserc defaults\n\
%s\
%s\
  For emergency restart:\n\
    [-2]               force paradise - use if you were on a paradise server\n\
    [-G] playernum     specify player number to use\n\
    [-s] port          specify socket number to use\n\
Paradise/TedTurner Client %s\n\
For more information on how to play Paradise, use Mosaic or Lynx and connect\n\
to:\n\
    http://www.netrek.org and http://paradise.netrek.org/\n\n", prog,
#ifdef METASERVER
	    "    [-m]               check metaserver for active servers\n",
#else
	    "",
#endif				/* METASERVER */
#ifdef RECORDER
	    "    [-F] file          Replay from file instead of connecting\n",
#else
	    "",
#endif
#ifdef XPM
	    "\
    [-x]               enable XPM mode.\n\
    [-v]               verbose image loading.\n\
    [-O]               turn on OR display mode when in XPM mode.\n\
    [-k]               turn on Cookie mode.\n\
    [-K]               turn off Cookie mode.\n",
#ifdef HACKED_XPMLIB
            "\
    [-b] planes        set how many bitplanes to use in OR mode.\n",
#else  /*HACKED_XPMLIB*/
            "",
#endif /*HACKED_XPMLIB*/
#else
            "",
#endif /*XPM [BDyess]*/
	    CLIENTVERS);
}

void
reaper()
{
#ifndef HAVE_WAIT3
    wait((int *) 0);
#else
/*    while (wait3((union wait *) 0, WNOHANG, NULL) > 0);*/
    while (wait3(NULL, WNOHANG, NULL) > 0);
#endif
}

#ifdef GATEWAY
static struct udpmap_t {
    int     uid;
    int     serv_port;
    int     port;
    int     local_port;
}       udpmap[] = {
    /* 5000, 5001, 5000 *//* generic */
    {
	1290, 5010, 5011, 5010
    },				/* fadden */
    {
	757, 5020, 5021, 5020
    },				/* user2 */
};
#define MAPSIZE (sizeof(udpmap) / sizeof(struct udpmap_t))

static void
getUdpPort()
{
    int     i;
    uid_t   uid;
    char   *gw_m, *gw_p, *gw_lp, *gw_sp, *err, *getenv();

    /* should always be set prior, but in case not .. */
    if (!gw_mach) {
	gw_m = getenv("GW_MACH");
	if (gw_m)
	    gw_mach = gw_m;
	else
	    gw_mach = DEFAULT_GATEWAY;
    }
    uid = getuid();

    for (i = 0; i < MAPSIZE; i++) {
	if (uid == udpmap[i].uid) {
	    gw_serv_port = udpmap[i].serv_port;
	    gw_port = udpmap[i].port;
	    gw_local_port = udpmap[i].local_port;
	    return;
	}
    }

    gw_p = getenv("GW_PORT");
    gw_sp = getenv("GW_SPORT");
    gw_lp = getenv("GW_LPORT");

    if (gw_p) {
	gw_port = strtol(gw_p, &err, 10);
	if (err == gw_p) {
	    fprintf(stderr, "netrek: malformed integer for GW_PORT: %s\n",
		    gw_p);
	    /* let something else complain about port 0 */
	}
    } else
	gw_port = 5001;
    if (gw_sp) {
	gw_serv_port = strtol(gw_sp, &err, 10);
	if (err == gw_sp) {
	    fprintf(stderr, "netrek: malformed integer for GW_SPORT: %s\n",
		    gw_sp);
	    /* let something else complain about port 0 */
	}
    } else
	gw_serv_port = 5000;

    if (gw_lp) {
	gw_local_port = strtol(gw_lp, &err, 10);
	if (err == gw_lp) {
	    fprintf(stderr, "netrek: malformed integer for GW_LPORT: %s\n",
		    gw_lp);
	    /* let something else complain about port 0 */
	}
    } else
	gw_local_port = 5000;

    /*
       printf("gw_mach: \'%s\'\n", gw_mach); printf("gw_local_port: %d\n",
       gw_local_port); printf("gw_serv_port: %d\n", gw_serv_port);
       printf("gw_port: %d\n", gw_port);
    */
}

static char *
get_gw()
{
    char   *gw_m;

    gw_m = getenv("GW_MACH");
    if (gw_m)
	gw_mach = gw_m;
    else
	gw_mach = DEFAULT_GATEWAY;

    return gw_mach;
}

#endif

#ifdef GATEWAY
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

/*
 * In the event of problems assiocated with the above include files the
 * following routine can be alternately used to convert a string
 * ("xxx.xxx.xxx.xxx") to an internet address number.
 *
 * (author: Andy McFadden)
 */

#ifdef notneeded
unsigned long
dotAddrToNetAddr(str)
    char   *str;
{
    char   *t;
    unsigned long answer = 0;
    t = str;
    for (i = 0; i < 4; i++) {
	answer = (answer << 8) | atoi(t);
	while (*t && *t != '.')
	    t++;
	if (*t)
	    t++;
    }
    return answer;
}
#endif

/*
 * More network "correct" routine
 */

unsigned long
mkaddr(m)
    char   *m;
{
    struct in_addr ad;
    struct hostent *hp;

    hp = gethostbyname(m);
    if (!hp) {
	ad.s_addr = inet_addr(m);
	if (ad.s_addr == -1) {
	    fprintf(stderr, "netrek: unknown host \'%s\'\n", m);
	    exit(1);
	}
    } else
	bcopy(hp->h_addr, (char *) &ad, hp->h_length);

    return ad.s_addr;
}

#endif /*gateway*/
