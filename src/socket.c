/*
 * Socket.c
 *
 * Kevin P. Smith 1/29/89
 * UDP stuff v1.0 by Andy McFadden  Feb-Apr 1992
 *
 * UDP protocol v1.0
 *
 * Routines to allow connection to the xtrek server.
 */
#include "copyright2.h"
#include "defines.h"

/* to see the packets sent/received: [BDyess] */
#if 0
#define SHOW_SEND
#define SHOW_RECEIVED
#endif				/* 0 */

#include "config.h"

#ifndef GATEWAY
#define USE_PORTSWAP		/* instead of using recvfrom() */
#endif

#undef USE_PORTSWAP		/* recvfrom is a better scheme */

#include <stdio.h>
#ifdef STDC_HEADERS
#include <stdlib.h>
#endif
#include <sys/types.h>
#include <sys/time.h>
#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#ifndef DNET
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#else
#include <devices/timer.h>
#include <dos/dos.h>
#endif				/* DNET */
#include <errno.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#ifdef HAVE_VALUES_H
#include <values.h>
#endif
#include <math.h>
#ifdef HAVE_LIMITS_H
#include <limits.h>
#ifndef MAXINT
#define MAXINT INT_MAX
#endif
#endif
#ifdef ASTEROIDS
#include <zlib.h>
#endif /* ASTEROIDS */
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "packets.h"
#include "proto.h"
#include "gameconf.h"
#ifdef SOUND
#include "Slib.h"
#endif				/* SOUND */
#include "sound.h"

#if 0
#ifndef MAXINT
#define MAXINT ~(unsigned int)0;
#endif /*MAXINT*/
#endif /*0*/

#define BIGINT 2000000000

#define INCLUDE_SCAN		/* include Amdahl scanning beams */
#define INCLUDE_VISTRACT	/* include visible tractor beams */

#ifdef GATEWAY
/*
 * (these values are now defined in "main.c":)
 * char *gw_mach        = "charon";     |client gateway; strcmp(serverName)
 * int   gw_serv_port   = 5000;         |what to tell the server to use
 * int   gw_port        = 5001;         |where we will contact gw
 * int   gw_local_port  = 5100;         |where we expect gw to contact us
 *
 * The client binds to "5100" and sends "5000" to the server (TCP).  The
 * server sees that and sends a UDP packet to gw on port udp5000, which passes
 * it through to port udp5100 on the client.  The client-gw gets the server's
 * host and port from recvfrom.  (The client can't use the same method since
 * these sockets are one-way only, so it connect()s to gw_port (udp5001)
 * on the gateway machine regardless of what the server sends.)
 *
 * So all we need in .gwrc is:
 *      udp 5000 5001 tde.uts 5100
 *
 * assuming the client is on tde.uts.  Note that a UDP declaration will
 * work for ANY server, but you need one per player, and the client has to
 * have the port numbers in advance.
 *
 * If we're using a standard server, we're set.  If we're running through a
 * gatewayed server, we have to do some unpleasant work on the server side...
 */
#endif

#ifdef SIZE_LOGGING
int     send_total = 0;
int     receive_total = 0;
#endif				/* SIZE_LOGGING */

#ifdef TERM
u_short serv_port;
#endif /*TERM*/

/* Prototypes */
static void resetForce P((void));
static void checkForce P((void));
#ifdef nodef
static void set_tcp_opts P((int s));
static void set_udp_opts P((int s));
#endif				/* nodef */
static int doRead P((int asock));
static void handleTorp P((struct torp_spacket * packet));
static void handleTorpInfo P((struct torp_info_spacket * packet));
static void handleStatus P((struct status_spacket * packet));
static void handleSelf P((struct you_spacket * packet));
static void handlePlayer P((struct player_spacket * packet));
static void handleWarning P((struct warning_spacket * packet));
void sendServerPacket P((struct player_spacket * packet));
static void handlePlanet P((struct planet_spacket * packet));
static void handlePhaser P((struct phaser_spacket * packet));
void handleMessage P((struct mesg_spacket * packet));
static void handleQueue P((struct queue_spacket * packet));
static void handlePickok P((struct pickok_spacket * packet));
static void handleLogin P((struct login_spacket * packet));
static void handlePlasmaInfo P((struct plasma_info_spacket * packet));
static void handlePlasma P((struct plasma_spacket * packet));
static void handleFlags P((struct flags_spacket * packet));
static void handleKills P((struct kills_spacket * packet));
static void handlePStatus P((struct pstatus_spacket * packet));
static void handleMotd P((struct motd_spacket * packet));
static void handleMask P((struct mask_spacket * packet));
static void pickSocket P((int old));
static void handleBadVersion P((struct badversion_spacket * packet));
int gwrite P((int fd, char *buf, int bytes));
static void handleHostile P((struct hostile_spacket * packet));
static void handlePlyrLogin P((struct plyr_login_spacket * packet));
static void handleStats P((struct stats_spacket * packet));
static void handlePlyrInfo P((struct plyr_info_spacket * packet));
static void handlePlanetLoc P((struct planet_loc_spacket * packet));
static void handleReserved P((struct reserved_spacket * packet));

static void handleScan P((struct scan_spacket * packet));
static void handleSequence P((struct sequence_spacket * packet));
static void handleUdpReply P((struct udp_reply_spacket * packet));
static void informScan P((int p));
static int openUdpConn P((void));
#ifdef USE_PORTSWAP
static int connUdpConn P((void));
#endif
static int recvUdpConn P((void));
static void printUdpInfo P((void));
/*static void dumpShip P((struct ship *shipp ));*/
/*static int swapl P((int in ));*/
static void handleShipCap P((struct ship_cap_spacket * packet));
static void handleMotdPic P((struct motd_pic_spacket * packet));
static void handleStats2 P((struct stats_spacket2 * packet));
static void handleStatus2 P((struct status_spacket2 * packet));
static void handlePlanet2 P((struct planet_spacket2 * packet));
#ifdef ASTEROIDS
static void handleTerrain2 P((struct terrain_packet2 * pkt));
static void handleTerrainInfo2 P((struct terrain_info_packet2 *pkt));
#endif /* ASTEROIDS */
static void handleTempPack P((struct obvious_packet * packet));
static void handleThingy P((struct thingy_spacket * packet));
static void handleThingyInfo P((struct thingy_info_spacket * packet));
static void handleRSAKey P((struct rsa_key_spacket * packet));
void    handlePing();
static void handleExtension1 P((struct paradiseext1_spacket *));

static void handleEmpty();


#ifdef SHORT_PACKETS
void    handleShortReply(), handleVPlayer(), handleVTorp(),
        handleSelfShort(), handleSelfShip(), handleVPlanet(), handleSWarning();
void    handleVTorpInfo(), handleSMessage();
#endif

#ifdef FEATURE
void    handleFeature();	/* feature.c */
#endif

struct packet_handler handlers[] = {
    {NULL},			/* record 0 */
    {handleMessage},		/* SP_MESSAGE */
    {handlePlyrInfo},		/* SP_PLAYER_INFO */
    {handleKills},		/* SP_KILLS */
    {handlePlayer},		/* SP_PLAYER */
    {handleTorpInfo},		/* SP_TORP_INFO */
    {handleTorp},		/* SP_TORP */
    {handlePhaser},		/* SP_PHASER */
    {handlePlasmaInfo},		/* SP_PLASMA_INFO */
    {handlePlasma},		/* SP_PLASMA */
    {handleWarning},		/* SP_WARNING */
    {handleMotd},		/* SP_MOTD */
    {handleSelf},		/* SP_YOU */
    {handleQueue},		/* SP_QUEUE */
    {handleStatus},		/* SP_STATUS */
    {handlePlanet},		/* SP_PLANET */
    {handlePickok},		/* SP_PICKOK */
    {handleLogin},		/* SP_LOGIN */
    {handleFlags},		/* SP_FLAGS */
    {handleMask},		/* SP_MASK */
    {handlePStatus},		/* SP_PSTATUS */
    {handleBadVersion},		/* SP_BADVERSION */
    {handleHostile},		/* SP_HOSTILE */
    {handleStats},		/* SP_STATS */
    {handlePlyrLogin},		/* SP_PL_LOGIN */
    {handleReserved},		/* SP_RESERVED */
    {handlePlanetLoc},		/* SP_PLANET_LOC */
    {handleScan},		/* SP_SCAN (ATM) */
    {handleUdpReply},		/* SP_UDP_STAT */
    {handleSequence},		/* SP_SEQUENCE */
    {handleSequence},		/* SP_SC_SEQUENCE */
    {handleRSAKey},		/* SP_RSA_KEY */
    {handleMotdPic},		/* SP_MOTD_PIC */
    {handleStats2},		/* SP_STATS2 */
    {handleStatus2},		/* SP_STATUS2 */
    {handlePlanet2},		/* SP_PLANET2 */
    {handleTempPack},		/* SP_TEMP_5 */
    {handleThingy},		/* SP_THINGY */
    {handleThingyInfo},		/* SP_THINGY_INFO */
    {handleShipCap},		/* SP_SHIP_CAP */

#ifdef SHORT_PACKETS
    {handleShortReply},		/* SP_S_REPLY */
    {handleSMessage},		/* SP_S_MESSAGE */
    {handleSWarning},		/* SP_S_WARNING */
    {handleSelfShort},		/* SP_S_YOU */
    {handleSelfShip},		/* SP_S_YOU_SS */
    {handleVPlayer},		/* SP_S_PLAYER */
#else
    {handleEmpty},		/* 40 */
    {handleEmpty},		/* 41 */
    {handleEmpty},		/* 42 */
    {handleEmpty},		/* 43 */
    {handleEmpty},		/* 44 */
    {handleEmpty},		/* 45 */
#endif
    {handlePing},		/* SP_PING */
#ifdef SHORT_PACKETS
    {handleVTorp},		/* SP_S_TORP */
    {handleVTorpInfo},		/* SP_S_TORP_INFO */
    {handleVTorp},		/* SP_S_8_TORP */
    {handleVPlanet},		/* SP_S_PLANET */
#else
    {handleEmpty},		/* 47 */
    {handleEmpty},
    {handleEmpty},
    {handleEmpty},		/* 50 */
#endif
    {handleGameparams},
    {handleExtension1},
#ifndef ASTEROIDS
    {handleEmpty},
    {handleEmpty},
#else
    {handleTerrain2},		/* 53 */
    {handleTerrainInfo2},
#endif /* ASTEROIDS */
    {handleEmpty},
    {handleEmpty},
    {handleEmpty},
    {handleEmpty},
    {handleEmpty},		/* 59 */
#ifdef FEATURE
    {handleFeature},		/* SP_FEATURE */
#else
    {handleEmpty},		/* 60 */
#endif
};

#define NUM_HANDLERS	(sizeof(handlers)/sizeof(*handlers))

#define NUM_PACKETS (sizeof(handlers) / sizeof(handlers[0]) - 1)

int     serverDead = 0;

#ifdef SIZE_LOGGING
/* prints the total number of bytes sent/received.  Called when exiting the
   client [BDyess] */
void
print_totals()
{
    time_t  timeSpent = time(NULL) - timeStart;
    
    if(timeStart == 0 || timeSpent == 0) {	/* never connected [BDyess] */
      printf("%8d bytes sent.\n%8d bytes received.\n",send_total,receive_total);
      return;
    }

    /*
       printf("Total bytes sent:     %d\nTotal bytes received: %d\n",
       send_total, receive_total);
    */
    timeSpent = timeSpent ? timeSpent : 1;
    /* ftp format [BDyess] */
    if (timeSpent < 600 /* 10 minutes */ ) {
	printf("%8d bytes sent     in %d seconds (%.3f Kbytes/s)\n",
	       send_total, (int)timeSpent,
	       (float) send_total / (1024.0 * timeSpent));
	printf("%8d bytes received in %d seconds (%.3f Kbytes/s)\n",
	       receive_total, (int)timeSpent,
	       (float) receive_total / (1024.0 * timeSpent));
    } else {			/* number too big for seconds, use minutes */
	printf("%8d bytes sent     in %.1f minutes (%.3f Kbytes/s)\n",
	       send_total, timeSpent / 60.0,
	       (float) send_total / (1024.0 * timeSpent));
	printf("%8d bytes received in %.1f minutes (%.3f Kbytes/s)\n",
	       receive_total, timeSpent / 60.0,
	       (float) receive_total / (1024.0 * timeSpent));
    }
}
#else
#define EXIT exit
#endif				/* SIZE_LOGGING */

int     udpLocalPort = 0;
static int udpServerPort = 0;
static u_long serveraddr = 0;
static u_short serverport = 0;
static long sequence = 0;
static int drop_flag = 0;
static int chan = -1;		/* tells sequence checker where packet is
				   from */
static short fSpeed, fDirection, fShield, fOrbit, fRepair, fBeamup, fBeamdown, fCloak,
        fBomb, fDockperm, fPhaser, fPlasma, fPlayLock, fPlanLock, fTractor,
        fRepress;

/* reset all the "force command" variables */
static void
resetForce()
{
    fSpeed = fDirection = fShield = fOrbit = fRepair = fBeamup = fBeamdown =
    fCloak = fBomb = fDockperm = fPhaser = fPlasma = fPlayLock = fPlanLock =
    fTractor = fRepress = -1;
}

/*
 * If something we want to happen hasn't yet, send it again.
 *
 * The low byte is the request, the high byte is a max count.  When the max
 * count reaches zero, the client stops trying.  Checking is done with a
 * macro for speed & clarity.
 */
#define FCHECK_FLAGS(flag, force, const) {                      \
        if (force > 0) {                                        \
            if (((me->p_flags & flag) && 1) ^ ((force & 0xff) && 1)) {  \
                speedReq.type = const;                          \
                speedReq.speed = (force & 0xff);                \
                sendServerPacket((struct player_spacket *)&speedReq);   \
                V_UDPDIAG(("Forced %d:%d\n", const, force & 0xff));     \
                force -= 0x100;                                 \
                if (force < 0x100) force = -1;  /* give up */   \
            } else                                              \
                force = -1;                                     \
        }                                                       \
}
#define FCHECK_VAL(value, force, const) {                       \
        if (force > 0) {                                        \
            if ((value) != (force & 0xff)) {                    \
                speedReq.type = const;                          \
                speedReq.speed = (force & 0xff);                \
                sendServerPacket((struct player_spacket *)&speedReq);   \
                V_UDPDIAG(("Forced %d:%d\n", const, force & 0xff));     \
                force -= 0x100;                                 \
                if (force < 0x100) force = -1;  /* give up */   \
            } else                                              \
                force = -1;                                     \
        }                                                       \
}
#define FCHECK_TRACT(flag, force, const) {                      \
        if (force > 0) {                                        \
            if (((me->p_flags & flag) && 1) ^ ((force & 0xff) && 1)) {  \
                tractorReq.type = const;                        \
                tractorReq.state = ((force & 0xff) >= 0x40);    \
                tractorReq.pnum = (force & 0xff) & (~0x40);     \
                sendServerPacket((struct player_spacket *)&tractorReq); \
                V_UDPDIAG(("Forced %d:%d/%d\n", const,          \
                        tractorReq.state, tractorReq.pnum));    \
                force -= 0x100;                                 \
                if (force < 0x100) force = -1;  /* give up */   \
            } else                                              \
                force = -1;                                     \
        }                                                       \
}

static void
checkForce()
{
    struct speed_cpacket speedReq;
    struct tractor_cpacket tractorReq;

    /* upgrading kludge [BDyess] */
    if (!upgrading)
	FCHECK_VAL(me->p_speed, fSpeed, CP_SPEED);	/* almost always repeats */
    FCHECK_VAL(me->p_dir, fDirection, CP_DIRECTION);	/* (ditto) */
    FCHECK_FLAGS(PFSHIELD, fShield, CP_SHIELD);
    FCHECK_FLAGS(PFORBIT, fOrbit, CP_ORBIT);
    FCHECK_FLAGS(PFREPAIR, fRepair, CP_REPAIR);
    FCHECK_FLAGS(PFBEAMUP, fBeamup, CP_BEAM);
    FCHECK_FLAGS(PFBEAMDOWN, fBeamdown, CP_BEAM);
    FCHECK_FLAGS(PFCLOAK, fCloak, CP_CLOAK);
    FCHECK_FLAGS(PFBOMB, fBomb, CP_BOMB);
    FCHECK_FLAGS(PFDOCKOK, fDockperm, CP_DOCKPERM);
    FCHECK_VAL(phasers[me->p_no].ph_status, fPhaser, CP_PHASER);	/* bug: dir 0 */
    FCHECK_VAL(plasmatorps[me->p_no].pt_status, fPlasma, CP_PLASMA);	/* (ditto) */
    FCHECK_FLAGS(PFPLOCK, fPlayLock, CP_PLAYLOCK);
    FCHECK_FLAGS(PFPLLOCK, fPlanLock, CP_PLANLOCK);

    /* kludge to help prevent self-deflects */
    if(! (hockey && me->p_tractor == 'g'-'a'+10 /*puck*/)) {
      FCHECK_TRACT(PFTRACT, fTractor, CP_TRACTOR);
      FCHECK_TRACT(PFPRESS, fRepress, CP_REPRESS);
    }
}


int
idx_to_mask(i)
    int     i;
{
    if (i == number_of_teams)
	return ALLTEAM;
    return 1 << i;
}

int
mask_to_idx(m)
    int     m;
{
    switch(m) {
      case NOBODY:
        return INDi;
      case FEDm:
        return FEDi;
      case ROMm:
        return ROMi;
      case KLIm:
        return KLIi;
      case ORIm:
        return ORIi;
      default:
        return number_of_teams;
    }
}

void
connectToServer(port)
    int     port;
{
#ifndef DNET
    int     s;
    struct sockaddr_in addr;
    struct sockaddr_in naddr;
    int     len;
    fd_set  readfds;
    struct timeval timeout;
    struct hostent *hp;

    serverDead = 0;
    if (sock != -1) {
	shutdown(sock, 2);
	sock = -1;
    }
    sleep(3);			/* I think this is necessary for some unknown
				   reason */

#ifdef RWATCH
    printf("rwatch: Waiting for connection. \n");
#else
    printf("Waiting for connection (port %d). \n", port);
#endif				/* RWATCH */

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
#ifdef RWATCH
	printf("rwatch: I can't create a socket\n");
#else
	printf("I can't create a socket\n");
#endif				/* RWATCH */
#ifdef AUTOKEY
	if (autoKey)
	    W_AutoRepeatOn();
#endif

	EXIT(2);
    }
#ifndef RWATCH
#ifdef nodef			/* don't use for now */
    set_tcp_opts(s);
#endif				/* nodef */
#endif				/* RWATCH */

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(s, (struct sockaddr *) & addr, sizeof(addr)) < 0) {
	sleep(10);
	if (bind(s, (struct sockaddr *) & addr, sizeof(addr)) < 0) {
	    sleep(10);
	    if (bind(s, (struct sockaddr *) & addr, sizeof(addr)) < 0) {
#ifdef RWATCH
		printf("rwatch: I can't bind to port!\n");
#else
		printf("I can't bind to port!\n");
#endif				/* RWATCH */
#ifdef AUTOKEY
		if (autoKey)
		    W_AutoRepeatOn();
#endif

		EXIT(3);
	    }
	}
    }
    listen(s, 1);

    len = sizeof(naddr);

tryagain:
    timeout.tv_sec = 240;	/* four minutes */
    timeout.tv_usec = 0;
    FD_ZERO(&readfds);
    FD_SET(s, &readfds);
    if (select(32, &readfds, NULL, NULL, &timeout) == 0) {
#ifdef RWATCH
	printf("rwatch: server died.\n");
#else
	printf("Well, I think the server died!\n");
#endif				/* RWATCH */
#ifdef AUTOKEY
	if (autoKey)
	    W_AutoRepeatOn();
#endif

	EXIT(0);
    }
    sock = accept(s, (struct sockaddr *) & naddr, &len);

    if (sock == -1) {
#ifdef RWATCH
	perror("rwatch: accept");
#else
	perror("accept");
#endif				/* RWATCH */
	goto tryagain;
    }

    close(s);
    pickSocket(port);		/* new socket != port */


    /*
       This is strictly necessary; it tries to determine who the caller is,
       and set "serverName" and "serveraddr" appropriately.
    */
    len = sizeof(struct sockaddr_in);
    if (getpeername(sock, (struct sockaddr *) & addr, &len) < 0) {
	perror("unable to get peername");
	serverName = "nowhere";
    } else {
	hp = gethostbyaddr((char *) &addr.sin_addr.s_addr, sizeof(long), AF_INET);
	serveraddr = addr.sin_addr.s_addr;
	serverport = addr.sin_port;
	if (hp != NULL) {
	    serverName = (char *) malloc(strlen(hp->h_name) + 1);
	    strcpy(serverName, hp->h_name);
	} else {
	    serverName = (char *) malloc(strlen((char *) inet_ntoa(addr.sin_addr)) + 1);
	    strcpy(serverName, (char *) inet_ntoa(addr.sin_addr));
	}
    }
    printf("Connection from server %s (0x%lx)\n", serverName, serveraddr);

#else				/* DNET */
    /*
       unix end DNet server process connects to the server, on specified *
       port
    */
    ConnectToDNetServer(port);
#endif				/* DNET */

}


#ifndef DNET
#ifdef nodef
static void
set_tcp_opts(s)
    int     s;
{
    int     optval = 1;
    struct protoent *ent;

    ent = getprotobyname("TCP");
    if (!ent) {
	fprintf(stderr, "TCP protocol not found.\n");
	return;
    }
    if (setsockopt(s, ent->p_proto, TCP_NODELAY, &optval, sizeof(int)) < 0)
	perror("setsockopt");
}

static void
set_udp_opts(s)
    int     s;
{
    int     optval = BUFSIZ;
    struct protoent *ent;
    ent = getprotobyname("UDP");
    if (!ent) {
	fprintf(stderr, "UDP protocol not found.\n");
	return;
    }
    if (setsockopt(s, ent->p_proto, SO_RCVBUF, &optval, sizeof(int)) < 0)
	perror("setsockopt");
}
#endif				/* nodef */

#endif				/* DNET */

char *
callServer(port, server)
    int     port;
    char   *server;
{
#ifndef DNET
    int     s;
    struct sockaddr_in addr;
    struct hostent *hp;
#endif
    serverDead = 0;

    printf("Calling %s on port %d.\n", server, port);
#ifdef DNET
    CallDNetServer(server, port);
#else
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	printf("I can't create a socket\n");
	EXIT(0);
    }
#ifndef RWATCH
#ifdef nodef
    set_tcp_opts(s);
#endif				/* nodef */
#endif				/* RWATCH */

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
#ifdef TERM
    serv_port = addr.sin_port;
#endif

    if ((addr.sin_addr.s_addr = inet_addr(server)) == -1) {
	if ((hp = gethostbyname(server)) == NULL) {
	    /* netrek.org alias lookup [BDyess] */
	    char *buf = (char*)malloc(BUFSIZ);

	    strcpy(buf,server);
	    strcat(buf,".netrek.org");
	    if((hp = gethostbyname(buf)) == NULL) {
	      printf("Who is %s?\n", server);
	      EXIT(0);
	    }
	    server = buf;
	}
	addr.sin_addr.s_addr = *(long *) hp->h_addr;
    }
    serveraddr = addr.sin_addr.s_addr;

    if (connect(s, (struct sockaddr *) & addr, sizeof(addr)) < 0) {
	printf("Server not listening!\n");
	EXIT(0);
    }
    printf("Got connection.\n");

    sock = s;
#endif				/* DNET */
/* pickSocket is utterly useless with DNet, but the server needs the
   packet to tell it the client is ready to start. */

#ifdef RECORDER
    startRecorder();
#endif
    pickSocket(port);		/* new socket != port */

    return server;
}

int
isServerDead()
{
    return (serverDead);
}

void
socketPause(sec, usec)
    int     sec, usec;
{
    struct timeval timeout;
    fd_set  readfds;

#ifdef RECORDER
    if (playback)
	return;
#endif
#ifdef DNET
    (void) DNetServerPause(sec, usec, 0);
#else

    timeout.tv_sec = sec;
    timeout.tv_usec = usec;
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);
    if (udpSock >= 0)		/* new */
	FD_SET(udpSock, &readfds);
    select(32, &readfds, 0, 0, &timeout);
#endif				/* DNET */
}

int
readFromServer()
{
    struct timeval timeout;
    fd_set  readfds;
    int     retval = 0, rs;

#ifdef RECORDER
    if (playback) {
	while (!pb_update)
	    doRead(sock);
	return 1;
    }
#endif

    if (serverDead)
	return (0);
    if (commMode == COMM_TCP)
	drop_flag = 0;		/* just in case */

#ifndef DNET
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);
    if (udpSock >= 0)
	FD_SET(udpSock, &readfds);
    if ((rs = select(32, &readfds, 0, 0, &timeout)) != 0) {
	if (rs < 0) {
	    /* NEW */
	    perror("select");
	    return 0;
	}
#else
	/* this should have been done before calling the function.. not sure about this, should maybe be doing
	   DNetServerPause here? -JR*/
	/*	sigsPending=Wait(sockMask | udpSockMask | SIGBREAKF_CTRL_C);
	 */
	if (sigsPending & SIGBREAKF_CTRL_C) {
	    printf("readFromServer: Ctrl-C break, exiting\n");
	    exit(0);
	}
	/* note for DNet FD_ISSET is  redefined. */
#endif				/* DNET */
	/* Read info from the xtrek server */
	if (FD_ISSET(sock, &readfds)) {
	    chan = sock;
	    retval += doRead(sock);
	}
	if (udpSock >= 0 && FD_ISSET(udpSock, &readfds)) {
	    /* WAS V_ *//* should be! */
	    V_UDPDIAG(("Activity on UDP socket\n"));
	    chan = udpSock;
	    if (commStatus == STAT_VERIFY_UDP) {
		warning("UDP connection established");
		sequence = 0;	/* reset sequence #s */
		resetForce();
		
		if (udpDebug)
		    printUdpInfo();
		UDPDIAG(("UDP connection established.\n"));
		
		commMode = COMM_UDP;
		commStatus = STAT_CONNECTED;
		commSwitchTimeout = 0;
		if (udpClientRecv != MODE_SIMPLE)
		    sendUdpReq(COMM_MODE + udpClientRecv);
		if (udpWin) {
		    udprefresh(UDP_CURRENT);
		    udprefresh(UDP_STATUS);
		}
	    }
	    retval += doRead(udpSock);
	}
#ifndef DNET
    }
#endif

    /* if switching comm mode, decrement timeout counter */
    if (commSwitchTimeout > 0) {
	if (!(--commSwitchTimeout)) {
	    /*
	      timed out; could be initial request to non-UDP server (which won't
	      be answered), or the verify packet got lost en route to the
	      server.  Could also be a request for TCP which timed out (weird),
	      in which case we just reset anyway.
	      */
	    commModeReq = commMode = COMM_TCP;
	    commStatus = STAT_CONNECTED;
	    if (udpSock >= 0)
		closeUdpConn();
	    if (udpWin) {
		udprefresh(UDP_CURRENT);
		udprefresh(UDP_STATUS);
	    }
	    warning("Timed out waiting for UDP response from server");
	    UDPDIAG(("Timed out waiting for UDP response from server\n"));
	}
    }
    /* if we're in a UDP "force" mode, check to see if we need to do something */
    if (commMode == COMM_UDP && udpClientSend > 1)
	checkForce();
    
    return (retval != 0);		/* convert to 1/0 */
}


/* this used to be part of the routine above */
char    buf[BUFSIZ * 2 + 16];

static int
doRead(asock)
    int     asock;
{
    char   *bufptr;
    int     size;
    int     count;
    int     temp;
#ifdef HANDLER_TIMES
    struct timeval htpre, htpost;
    extern void log_time(int, struct timeval *, struct timeval *);
#endif

#ifndef DNET
    struct timeval timeout;
    fd_set  readfds;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
#endif				/* DNET */

    count = sock_read(asock, buf, 2 * BUFSIZ);
#ifdef DNET
    if (count == 0) {		/* yuck. */
	return 0;
    }
#endif
/* TMP */
#ifdef nodef
    if (asock == udpSock)
	printf("read %d bytes\n", count);
#endif				/* nodef */

    if (count <= 0) {
	if (asock == udpSock) {
#ifndef DNET
	    if (errno == ECONNREFUSED) {
		struct sockaddr_in addr;

		UDPDIAG(("asock=%d, sock=%d, udpSock=%d, errno=%d\n",
			 asock, sock, udpSock, errno));
		UDPDIAG(("count=%d\n", count));
		UDPDIAG(("Hiccup(%d)!  Reconnecting\n", errno));
		addr.sin_addr.s_addr = serveraddr;
		addr.sin_port = htons(udpServerPort);
		addr.sin_family = AF_INET;
		if (connect(udpSock, (struct sockaddr *) & addr, sizeof(addr)) < 0) {
		    perror("connect");
		    UDPDIAG(("Unable to reconnect\n"));
		    /* and fall through to disconnect */
		} else {
		    UDPDIAG(("Reconnect successful\n"));
		    return (0);
		}
	    }
#endif				/* DNET */
	    UDPDIAG(("*** UDP disconnected (res=%d, err=%d)\n",
		     count, errno));
	    warning("UDP link severed");
	    printUdpInfo();
	    closeUdpConn();
	    commMode = commModeReq = COMM_TCP;
	    commStatus = STAT_CONNECTED;
	    if (udpWin) {
		udprefresh(UDP_STATUS);
		udprefresh(UDP_CURRENT);
	    }
	    return (0);
	}
#ifndef RWATCH
	printf("1) Got read() of %d. Server dead\n", count);
	perror("");
#endif				/* RWATCH */
	serverDead = 1;
	return (0);
    }
    bufptr = buf;
    while (bufptr < buf + count) {
#ifdef SHORT_PACKETS
computesize:
	if ((*bufptr == SP_S_MESSAGE && (buf + count - bufptr <= 4))
	    || (buf + count - bufptr < 4)) {	/* last part may only be
						   needed for DNet...I'm not
						   so sure any more.
						   certainly doesn't hurt to
						   have it.-JR */
	    /*
	       printf("buf+count-bufptr=%d, *bufptr=%d\n",buf + count -
	       bufptr,*bufptr);
	    */
	    size = 0;		/* problem with reads breaking before size
				   byte for SP_S_MESSAGE has been read. Only
				   a problem for messages because reads break
				   on 4 byte boundaries,  other size bytes
				   are always in the first 4. -JR */
	} else
#endif
	{
	    size = size_of_spacket((unsigned char *)bufptr);
	    if (size < 1) {
#if 0
		fprintf(stderr, "Unknown packet %d.  Faking it.\n",
			*bufptr);
		size = 4;
#else
		fprintf(stderr, "Unknown packet %d.  Aborting.\n",
			*bufptr);
		return (0);
#endif
	    }
#ifdef SHOW_RECEIVED
	    printf("recieved packet type %d, size %d\n", *bufptr, size);
#endif				/* SHOW_RECEIVED */
#ifdef PACKET_LIGHTS
	    light_receive();
#endif				/* PACKET_LIGHTS */
#ifdef SIZE_LOGGING
	    receive_total += size;
#endif				/* SIZE_LOGGING */
	}
	while (size > count + (buf - bufptr) || size == 0) {
	    /*
	       We wait for up to ten seconds for rest of packet. If we don't
	       get it, we assume the server died.
	    */
	    /*
	       printf("er, possible packet fragment, waiting for the
	       rest...\n");
	    */
#ifdef RECORDER
	    if (!playback)
#endif
	    {
#ifdef DNET
		temp = DNetServerPause(20, 0, asock);
#else				/* DNET */
		timeout.tv_sec = 20;
		timeout.tv_usec = 0;
		FD_ZERO(&readfds);
		FD_SET(asock, &readfds);
		/* readfds=1<<asock; */
		temp = select(32, &readfds, 0, 0, &timeout);
#endif
		if (temp == 0) {
		    printf("Packet fragment.  Server must be dead\n");
		    serverDead = 1;
		    return (0);
		}
	    }
#ifdef SHORT_PACKETS
	    if (size == 0)
		/* 84=largest short packet message - the 4 we have */
		temp = sock_read(asock, buf + count, 84);
	    else
#endif
		temp = sock_read(asock, buf + count, size - (count + (buf - bufptr)));
	    count += temp;
#ifdef DNET
	    if (temp < 0)
#else
	    if (temp <= 0)
#endif				/* DNET */
	    {
#ifndef RWATCH
		printf("2) Got read() of %d.  Server is dead\n", temp);
#endif				/* RWATCH */
		serverDead = 1;
		return (0);
	    }
#ifdef SHORT_PACKETS
	    if (size == 0)	/* for the SP_S_MESSAGE problem */
		goto computesize;
#endif
	}
#ifdef UPDATE_SIZES
	totalbytes += size;
	packetbytes[*bufptr] += size;
#endif
#ifdef RECORDER
	if (playback && (*bufptr == REC_UPDATE)) {
	    pb_update++;
	    me->p_tractor = bufptr[1];
	    if (me->p_flags & PFPLOCK)
		me->p_playerl = bufptr[2];
	    else
		me->p_planet = bufptr[2];
/*	    printf("Read REC_UPDATE pseudo-packet!\n");*/
	} else
#endif
	    if (*bufptr >= 1 &&
		*bufptr < NUM_HANDLERS &&
		handlers[(int) *bufptr].handler != NULL) {
	    if (asock != udpSock ||
		(!drop_flag || *bufptr == SP_SEQUENCE || *bufptr == SP_SC_SEQUENCE)) {
		if (asock == udpSock)
		    packets_received++;	/* ping stuff */
#ifdef RECORDER
		if (recordGame)
		    recordPacket(bufptr, size);
#endif
#ifdef HANDLER_TIMES
		gettimeofday(&htpre,0);
#endif
		(*(handlers[(unsigned char)*bufptr].handler)) (bufptr);
#ifdef HANDLER_TIMES
		gettimeofday(&htpost,0);
		log_time(*bufptr, &htpre, &htpost);
#endif
		/* printf("handled packet %d\n", (unsigned char)*bufptr); */
	    } else
		UDPDIAG(("Ignored type %d\n", *bufptr));
	} else {
	    printf("Handler for packet %d not installed...\n", *bufptr);
	}

	bufptr += size;
	if (bufptr > buf + BUFSIZ) {
	    bcopy(buf + BUFSIZ, buf, BUFSIZ);
	    if (count == BUFSIZ * 2) {
#ifdef RECORDER
		if (playback)
		    temp = 0;
		else
#endif
		{
#ifdef DNET
		    temp = DNetServerPause(3, 0, asock);
#else
		    FD_ZERO(&readfds);
		    FD_SET(asock, &readfds);
		    /* readfds = 1<<asock; */
		    temp = select(32, &readfds, 0, 0, &timeout);
#endif				/* DNET */
		}
		if (temp != 0) {
		    temp = sock_read(asock, buf + BUFSIZ, BUFSIZ);
		    count = BUFSIZ + temp;
#ifdef DNET
		    if (temp < 0)
#else
		    if (temp <= 0)
#endif				/* DNET */
		    {
#ifndef RWATCH
			printf("3) Got read() of %d.  Server is dead\n", temp);
#endif				/* RWATCH */
			serverDead = 1;
			return (0);
		    }
		} else {
		    count = BUFSIZ;
		}
	    } else {
		count -= BUFSIZ;
	    }
	    bufptr -= BUFSIZ;
	}
    }
    return (1);
}

#define SANITY_TORPNUM(idx) \
	if ( (unsigned)(idx) >= ntorps*nplayers) { \
	    fprintf(stderr, "torp index %d out of bounds\n", (idx)); \
	    return; \
	}

#define SANITY_PNUM(idx) \
	if ( (unsigned)(idx) >= nplayers) { \
	    fprintf(stderr, "player number %d out of bounds\n", (idx)); \
	    return; \
	}

#define SANITY_PHASNUM(idx) \
	if ( (unsigned)(idx) >= nplayers*nphasers) { \
	    fprintf(stderr, "phaser number %d out of bounds\n", (idx)); \
	    return; \
	}

#define SANITY_PLASNUM(idx) \
	if ( (unsigned)(idx) >= nplayers*nplasmas) { \
	    fprintf(stderr, "plasma number %d out of bounds\n", (idx)); \
	    return; \
	}

#define SANITY_THINGYNUM(idx) \
	if ( (unsigned)(idx) >= npthingies*nplayers + ngthingies) { \
	    fprintf(stderr, "thingy index %x out of bounds\n", (idx)); \
	    return; \
	}

#define SANITY_PLANNUM(idx) \
	if ( (unsigned)(idx) >= nplanets) { \
	    fprintf(stderr, "planet index %d out of bounds\n", (idx)); \
	    return; \
	}

#define SANITY_SHIPNUM(idx) \
	if ( (unsigned)(idx) >= nshiptypes) { \
	    fprintf(stderr, "ship type %d out of bounds\n", (idx)); \
	    return; \
	}


static void
handleTorp(packet)
    struct torp_spacket *packet;
{
    struct torp *thetorp;

    SANITY_TORPNUM(ntohs(packet->tnum));

    thetorp = &torps[ntohs(packet->tnum)];
    thetorp->t_x = ntohl((unsigned)packet->x);
    thetorp->t_y = ntohl((unsigned)packet->y);
    thetorp->t_dir = packet->dir;

#ifdef ROTATERACE
    if (rotate) {
	rotate_gcenter(&thetorp->t_x, &thetorp->t_y);
	rotate_dir(&thetorp->t_dir, rotate_deg);
#endif
#ifdef BORGTEST
	if (bd)
	    bd_test_torp(ntohs(packet->tnum), thetorp);
#endif
    }
}


static void
handleTorpInfo(packet)
    struct torp_info_spacket *packet;
{
    struct torp *thetorp;

    SANITY_TORPNUM(ntohs(packet->tnum));

    thetorp = &torps[ntohs(packet->tnum)];

    if (packet->status == TEXPLODE && thetorp->t_status == TFREE) {
	/* FAT: redundant explosion; don't update p_ntorp */
	/*
	   printf("texplode ignored\n");
	*/
	return;
    }

    if (thetorp->t_status == TFREE && packet->status) {
	players[thetorp->t_owner].p_ntorp++;
	thetorp->frame = 0;
#ifdef BORGTEST
	if (bd)
	    bd_new_torp(ntohs(packet->tnum), thetorp);
#endif

    }
    if (thetorp->t_status && packet->status == TFREE) {
	players[thetorp->t_owner].p_ntorp--;
    }
    if (packet->status != thetorp->t_status) {
	/* FAT: prevent explosion reset */
	thetorp->t_status = packet->status;
	if (thetorp->t_status == TEXPLODE) {
	    thetorp->t_fuse = BIGINT;
	} else {
	    thetorp->t_war = packet->war;
	}
    } else {
        thetorp->t_war = packet->war;
    }
    thetorp->t_team = idx_to_mask(players[thetorp->t_owner].p_teami);
}

static void
handleStatus(packet)
    struct status_spacket *packet;
{
    status->tourn = packet->tourn;
    status->armsbomb = ntohl((unsigned)packet->armsbomb);
    status->planets = ntohl((unsigned)packet->planets);
    status->kills = ntohl((unsigned)packet->kills);
    status->losses = ntohl((unsigned)packet->losses);
    status->time = ntohl((unsigned)packet->time);
    status->timeprod = ntohl((unsigned)packet->timeprod);
}

static void
handleSelf(packet)
    struct you_spacket *packet;
{
    SANITY_PNUM(packet->pnum);
    me = (ghoststart ? &players[ghost_pno] : &players[packet->pnum]);
    myship = (me->p_ship);
    mystats = &(me->p_stats);
    me->p_hostile = packet->hostile;
    me->p_swar = packet->swar;
    me->p_armies = packet->armies;
    me->p_flags = ntohl((unsigned)packet->flags);
    me->p_damage = ntohl((unsigned)packet->damage);
    me->p_shield = ntohl((unsigned)packet->shield);
    me->p_fuel = ntohl((unsigned)packet->fuel);
    me->p_etemp = ntohs(packet->etemp);
    me->p_wtemp = ntohs(packet->wtemp);
    me->p_whydead = ntohs(packet->whydead);
    me->p_whodead = ntohs(packet->whodead);
    status2->clock = (unsigned long) packet->pad2;
    status2->clock += ((unsigned long) packet->pad3) << 8;
#ifdef INCLUDE_VISTRACT
    if (packet->tractor & 0x40)
	me->p_tractor = (short) packet->tractor & (~0x40);	/* ATM - visible trac
								   tors */
#ifdef nodef			/* tmp */
    else
	me->p_tractor = -1;
#endif				/* nodef */
#endif
#ifdef SOUND
    S_HandlePFlags();
#endif

}

static void
handlePlayer(packet)
    struct player_spacket *packet;
{
    register struct player *pl;
    unsigned char newdir;

    SANITY_PNUM(packet->pnum);


    pl = &players[packet->pnum];
    newdir = packet->dir;
#ifdef ROTATERACE
    if (rotate)
	rotate_dir(&newdir, rotate_deg);
#endif
#ifdef CHECK_DROPPED
    /* Kludge to fix lost uncloak packets! [3/94] -JR */
    if (pl->p_flags & PFCLOAK && pl->p_cloakphase >= (CLOAK_PHASES - 1)) {
	if (pl->p_dir != newdir) {	/* always sends the same direction
					   when cloaked! */
	    int     i, plocked = 0;

	    /*
	       nplayers is for paradise, probably want MAX_PLAYERS for other
	       clients
	    */
	    for (i = 0; i < nplayers; i++) {	/* except when someone has
						   this person phasered :( */
		if ((phasers[i].ph_status & PHHIT) && (phasers[i].ph_target == packet->pnum)) {
		    plocked = 1;
		    break;
		}
	    }
	    if (!plocked) {
		pl->p_flags &= ~(PFCLOAK);
		if (reportDroppedPackets)
		    printf("Uncloak kludge, player %d\n", pl->p_no);
	    }
	}
    }
#endif
    pl->p_dir = newdir;
    pl->p_speed = packet->speed;
    pl->p_x = ntohl((unsigned)packet->x);
    pl->p_y = ntohl((unsigned)packet->y);
    if (pl == me) {
	extern int my_x, my_y;	/* from shortcomm.c */
	my_x = me->p_x;
	my_y = me->p_y;
    }
    redrawPlayer[packet->pnum] = 1;

    if (me == pl) {
	extern int my_x, my_y;	/* short packets need unrotated co-ords! */
	my_x = pl->p_x;
	my_y = pl->p_y;
    }
#ifdef ROTATERACE
    if (rotate) {
	rotate_gcenter(&pl->p_x, &pl->p_y);
    }
#endif

}


static void
handleWarning(packet)
    struct warning_spacket *packet;
{
    warning((char *) packet->mesg);
}

static void
handleThingy(packet)
    struct thingy_spacket *packet;
{
    struct thingy *thetorp;

    SANITY_THINGYNUM(ntohs(packet->tnum));

    thetorp = &thingies[ntohs(packet->tnum)];
    thetorp->t_x = ntohl((unsigned)packet->x);
    thetorp->t_y = ntohl((unsigned)packet->y);
    /* printf("drone at %d, %d\n", thetorp->t_x, thetorp->t_y); */
    thetorp->t_dir = packet->dir;


#ifdef ROTATERACE
    if (rotate) {
	rotate_gcenter(&thetorp->t_x, &thetorp->t_y);
	rotate_dir(&thetorp->t_dir, rotate_deg);
    }
#endif

    if (thetorp->t_shape == SHP_WARP_BEACON)
	redrawall = 1;		/* shoot, route has changed */

}

static void
handleThingyInfo(packet)
    struct thingy_info_spacket *packet;
{
    struct thingy *thetorp;

    SANITY_THINGYNUM(ntohs(packet->tnum));

    thetorp = &thingies[ntohs(packet->tnum)];

#if 1
    thetorp->t_owner = ntohs(packet->owner);
#else
    /* we have the gameparam packet now */
#endif

    if (thetorp->t_shape == SHP_WARP_BEACON)
	redrawall = 1;		/* redraw the lines, I guess */

    if (ntohs(packet->shape) == SHP_BOOM && thetorp->t_shape == SHP_BLANK) {
	/* FAT: redundant explosion; don't update p_ntorp */
	/*
	   printf("texplode ignored\n");
	*/
	return;
    }

    if (thetorp->t_shape == SHP_BLANK && ntohs(packet->shape) != SHP_BLANK) {
	players[thetorp->t_owner].p_ndrone++;	/* TSH */
    }
    if (thetorp->t_shape != SHP_BLANK && ntohs(packet->shape) == SHP_BLANK) {
	players[thetorp->t_owner].p_ndrone--;	/* TSH */
    }
    thetorp->t_war = packet->war;

    if (ntohs(packet->shape) != thetorp->t_shape) {
	/* FAT: prevent explosion reset */
	int shape = ntohs(packet->shape);

        if(shape == SHP_BOOM || shape == SHP_PBOOM) {
	  if(thetorp->t_shape == SHP_FIGHTER)
	    shape = SHP_FBOOM;
	  if(thetorp->t_shape == SHP_MISSILE)
	    shape = SHP_DBOOM;
	  thetorp->t_fuse = BIGINT;
	}
	thetorp->t_shape = shape;
    }
}

void
sendShortPacket(type, state)
    int    type, state;
{
    struct speed_cpacket speedReq;

    speedReq.type = type;
    speedReq.speed = state;
#ifdef UNIX_SOUND
    if (type == CP_SHIELD) play_sound (SND_SHIELD); /* Shields */
#endif
    sendServerPacket((struct player_spacket *) & speedReq);
    /* printf("Sending packet #%d\n",type); */

    /* if we're sending in UDP mode, be prepared to force it */
    if (commMode == COMM_UDP && udpClientSend >= 2) {
	switch (type) {
	case CP_SPEED:
	    fSpeed = state | 0x100;
	    break;
	case CP_DIRECTION:
	    fDirection = state | 0x100;
	    break;
	case CP_SHIELD:
	    fShield = state | 0xa00;
	    break;
	case CP_ORBIT:
	    fOrbit = state | 0xa00;
	    break;
	case CP_REPAIR:
	    fRepair = state | 0xa00;
	    break;
	case CP_CLOAK:
	    fCloak = state | 0xa00;
	    break;
	case CP_BOMB:
	    fBomb = state | 0xa00;
	    break;
	case CP_DOCKPERM:
	    fDockperm = state | 0xa00;
	    break;
	case CP_PLAYLOCK:
	    fPlayLock = state | 0xa00;
	    break;
	case CP_PLANLOCK:
	    fPlanLock = state | 0xa00;
	    break;
	case CP_BEAM:
	    if (state == 1)
		fBeamup = 1 | 0x500;
	    else
		fBeamdown = 2 | 0x500;
	    break;
	}

	/* force weapons too? */
	if (udpClientSend >= 3) {
	    switch (type) {
	    case CP_PHASER:
		fPhaser = state | 0x100;
		break;
	    case CP_PLASMA:
		fPlasma = state | 0x100;
		break;
	    }
	}
    }
}

#ifndef __CEXTRACT__
/* Pick a random type for the packet */
void
sendServerPacket(packet)
    struct player_spacket *packet;
{
    int     size;

#ifdef RWATCH
    return;
#endif				/* RWATCH */

    if (serverDead)
	return;
    size = size_of_cpacket(packet);
    if (size < 1) {
	printf("Attempt to send strange packet %d!\n", packet->type);
	return;
    }
#ifdef SHOW_SEND
    printf("sending packet type %d, size %d\n", packet->type,
	   size);
#endif				/* SHOW_SEND */
#ifdef PACKET_LIGHTS
    light_send();
#endif				/* PACKET_LIGHTS */
#ifdef SIZE_LOGGING
    send_total += size;
#endif				/* SIZE_LOGGING */
    if (commMode == COMM_UDP) {
	/* for now, just sent everything via TCP */
    }
    if (commMode == COMM_TCP || !udpClientSend) {
	/* special case for verify packet */
	if (packet->type == CP_UDP_REQ) {
	    if (((struct udp_req_cpacket *) packet)->request == COMM_VERIFY)
		goto send_udp;
	}
	/*
	   business as usual (or player has turned off UDP transmission)
	*/
	if (gwrite(sock, (char *) packet, size) != size) {
	    printf("gwrite failed.  Server must be dead\n");
	    serverDead = 1;
	}
    } else {
	/*
	   UDP stuff
	*/
	switch (packet->type) {
	case CP_SPEED:
	case CP_DIRECTION:
	case CP_PHASER:
	case CP_PLASMA:
	case CP_TORP:
	case CP_QUIT:
	case CP_PRACTR:
	case CP_REPAIR:
	case CP_ORBIT:
	case CP_BOMB:
	case CP_BEAM:
	case CP_DET_TORPS:
	case CP_DET_MYTORP:
	case CP_TRACTOR:
	case CP_REPRESS:
	case CP_COUP:
	case CP_DOCKPERM:
	case CP_SCAN:
	case CP_PING_RESPONSE:
	case CP_CLOAK:
	case CP_SHIELD:
	case CP_PLANLOCK:
	    /*
	       these are non-critical but don't expire, send with TCP
	       [BDyess]
	    */
	    /* case CP_REFIT: */
	    /* case CP_PLAYLOCK: */
	    /* non-critical stuff, use UDP */
    send_udp:
	    packets_sent++;	/* ping stuff */

	    V_UDPDIAG(("Sent %d on UDP port\n", packet->type));
	    if (gwrite(udpSock, (char *) packet, size) != size) {
		UDPDIAG(("gwrite on UDP failed.  Closing UDP connection\n"));
		warning("UDP link severed");
		/* serverDead=1; */
		commModeReq = commMode = COMM_TCP;
		commStatus = STAT_CONNECTED;
		commSwitchTimeout = 0;
		if (udpWin) {
		    udprefresh(UDP_STATUS);
		    udprefresh(UDP_CURRENT);
		}
		if (udpSock >= 0)
		    closeUdpConn();
	    }
	    break;

	default:
	    /* critical stuff, use TCP */
	    if (gwrite(sock, (char *) packet, size) != size) {
		printf("gwrite failed.  Server must be dead\n");
		serverDead = 1;
	    }
	}
    }
}
#endif /*__CEXTRACT__*/

static void
handlePlanet(packet)
    struct planet_spacket *packet;
{
    struct planet *plan;
    /* FAT: prevent excessive redraw */
    int     redrawflag = 0;
#ifdef HOCKEY
    int     hockey_update = 0;
#endif /*HOCKEY*/

    SANITY_PLANNUM(packet->pnum);

    plan = &planets[packet->pnum];
    if (plan->pl_owner != packet->owner) {
	redrawflag = 1;
	hockey_update = 1;
    }
    plan->pl_owner = packet->owner;

    if (plan->pl_owner < (1 << 0) || plan->pl_owner > (1 << (number_of_teams - 1)))
	plan->pl_owner = NOBODY;

    if (plan->pl_info != packet->info)
	redrawflag = 1;
    plan->pl_info = packet->info;
    /* Redraw the planet because it was updated by server */

    if (plan->pl_flags != (int) ntohs(packet->flags))
	redrawflag = 1;
    plan->pl_flags = (unsigned short) ntohs(packet->flags);

    if (plan->pl_armies != ntohl((unsigned)packet->armies))
	redrawflag = 1;

    plan->pl_armies = ntohl((unsigned)packet->armies);
    if (plan->pl_info == 0) {
	plan->pl_owner = NOBODY;
    }
    if (redrawflag) {
	plan->pl_flags |= PLREDRAW;
	pl_update[packet->pnum].plu_update = 1;	/* used to mean the planet
						   had moved, now set as a
						   sign we need to erase AND
						   redraw. -JR */
	pl_update[packet->pnum].plu_x = planets[packet->pnum].pl_x;
	pl_update[packet->pnum].plu_y = planets[packet->pnum].pl_y;
	if (infomapped && infotype == PLANETTYPE &&
	    ((struct planet *) infothing)->pl_no == packet->pnum)
	    infoupdate = 1;
#ifdef HOCKEY
        if(hockey_update && hockey) hockeyInit();
#endif /*HOCKEY*/
    }
}

static void
handlePhaser(packet)
    struct phaser_spacket *packet;
{
    struct phaser *phas;

    SANITY_PHASNUM(packet->pnum);

    phas = &phasers[packet->pnum];
#ifdef CHECK_DROPPED
    /* can't fire weapons cloaked, this guy must be uncloaked.. */
    /* applying this to torps is trickier... -JR */
    if (packet->status != PHFREE) {
	if (reportDroppedPackets && (players[packet->pnum].p_flags & PFCLOAK))
	    printf("Dropped uncloak, player %d. (fired phaser)\n", packet->pnum);
	players[packet->pnum].p_flags &= ~(PFCLOAK);
    } else {
	if (longest_ph_fuse < phas->ph_fuse)
	    longest_ph_fuse = phas->ph_fuse;
    }
#endif
    phas->ph_status = packet->status;
    phas->ph_dir = packet->dir;
    phas->ph_x = ntohl((unsigned)packet->x);
    phas->ph_y = ntohl((unsigned)packet->y);
    phas->ph_target = ntohl((unsigned)packet->target);
    phas->ph_fuse = 0;

#ifdef ROTATERACE
    if (rotate) {
	rotate_gcenter(&phas->ph_x, &phas->ph_y);
	rotate_dir(&phas->ph_dir, rotate_deg);
    }
#endif
#ifdef SOUND
    if ((me->p_no == packet->pnum) && (packet->status != PHFREE)) {
	S_PlaySound(S_PHASER);
    }
#endif
#ifdef UNIX_SOUND
    if ((me->p_no == packet->pnum) && (packet->status != PHFREE)) {
        play_sound(SND_PHASER); /* Phasers */
    }
#endif
}

#ifndef __CEXTRACT__
void
handleMessage(packet)
    struct mesg_spacket *packet;
{
    if ((int) packet->m_from >= nplayers)
	packet->m_from = 255;
    dmessage(packet->mesg, packet->m_flags, packet->m_from, packet->m_recpt);
}
#endif /*__CEXTRACT__*/


static void
handleQueue(packet)
    struct queue_spacket *packet;
{
    queuePos = ntohs(packet->pos);
    /* printf("Receiving queue position %d\n",queuePos); */
}

static void
handleEmpty(ptr)
    char   *ptr;
{
    printf("Unknown packet type: %d\n", *ptr);
    return;
}

void
sendTeamReq(team, ship)
    int     team, ship;
{
    struct outfit_cpacket outfitReq;

    outfitReq.type = CP_OUTFIT;
    outfitReq.team = team;
    outfitReq.ship = ship;
    sendServerPacket((struct player_spacket *) & outfitReq);
}

static void
handlePickok(packet)
    struct pickok_spacket *packet;
{
    pickOk = packet->state;
#ifdef RECORDER
    if (playback) {		/* added when the packet is recorded. */
	extern int lastTeamReq;
	lastTeamReq = packet->pad2;
    }
#endif
}

void
sendLoginReq(name, pass, loginname, query)
    char   *name, *pass;
    char   *loginname;
    int     query;
{
    struct login_cpacket packet;

    strcpy(packet.name, name);
    strcpy(packet.password, pass);
    if (strlen(loginname) > 15)
	loginname[15] = 0;
    strcpy(packet.login, loginname);
    packet.type = CP_LOGIN;
    packet.query = query;
    packet.pad2 = 0x69;		/* added 1/19/93 KAO */
    packet.pad3 = 0x43;		/* added 1/19/93 KAO *//* was 0x42 3/2/93 */
    sendServerPacket((struct player_spacket *) & packet);
}

static void
handleLogin(packet)
    struct login_spacket *packet;
{


    loginAccept = packet->accept;
    if ((packet->pad2 == 69) && (packet->pad3 == 42))
	paradise = 1;
    else {
	/*nshiptypes = 8;*/
	/*nplayers=20;*/
	/*nplanets=40;*/
    }
    if (packet->accept) {

	/* we no longer accept keymaps from the server */

	mystats->st_flags = ntohl((unsigned)packet->flags);
	keeppeace = (me->p_stats.st_flags / ST_KEEPPEACE) & 1;
    }
}

void
sendTractorReq(state, pnum)
    int    state;
    int    pnum;
{
    struct tractor_cpacket tractorReq;

    tractorReq.type = CP_TRACTOR;
    tractorReq.state = state;
    tractorReq.pnum = pnum;
    sendServerPacket((struct player_spacket *) & tractorReq);

    if (state)
	fTractor = pnum | 0x40;
    else
	fTractor = 0;
}

void
sendRepressReq(state, pnum)
    int    state;
    int    pnum;
{
    struct repress_cpacket repressReq;

    repressReq.type = CP_REPRESS;
    repressReq.state = state;
    repressReq.pnum = pnum;
    sendServerPacket((struct player_spacket *) & repressReq);

    if (state)
	fRepress = pnum | 0x40;
    else
	fRepress = 0;
}

void
sendDetMineReq(torp)
    int   torp;
{
    struct det_mytorp_cpacket detReq;

    detReq.type = CP_DET_MYTORP;
    detReq.tnum = htons(torp);
    sendServerPacket((struct player_spacket *) & detReq);
}

static void
handlePlasmaInfo(packet)
    struct plasma_info_spacket *packet;
{
    struct plasmatorp *thetorp;

    SANITY_PLASNUM(ntohs(packet->pnum));

    thetorp = &plasmatorps[ntohs(packet->pnum)];
    if (packet->status == PTEXPLODE && thetorp->pt_status == PTFREE) {
	/* FAT: redundant explosion; don't update p_nplasmatorp */
	return;
    }
    if (!thetorp->pt_status && packet->status) {
	players[thetorp->pt_owner].p_nplasmatorp++;
#ifdef UNIX_SOUND
    play_sound (SND_PLASMA); /* Plasma */
#endif
    }
    if (thetorp->pt_status && !packet->status) {
	players[thetorp->pt_owner].p_nplasmatorp--;
    }
    thetorp->pt_war = packet->war;
    if (thetorp->pt_status != packet->status) {
	/* FAT: prevent explosion timer from being reset */
	thetorp->pt_status = packet->status;
	if (thetorp->pt_status == PTEXPLODE) {
	    thetorp->pt_fuse = BIGINT;
	}
    }
}

static void
handlePlasma(packet)
    struct plasma_spacket *packet;
{
    struct plasmatorp *thetorp;

    SANITY_PLASNUM(ntohs(packet->pnum));

    thetorp = &plasmatorps[ntohs(packet->pnum)];
    thetorp->pt_x = ntohl((unsigned)packet->x);
    thetorp->pt_y = ntohl((unsigned)packet->y);

#ifdef ROTATERACE
    if (rotate) {
	rotate_gcenter(&thetorp->pt_x, &thetorp->pt_y);
    }
#endif
}

static void
handleFlags(packet)
    struct flags_spacket *packet;
{
    SANITY_PNUM(packet->pnum);

    if (players[packet->pnum].p_flags != ntohl((unsigned)packet->flags) ||
    players[packet->pnum].p_tractor != ((short) packet->tractor & (~0x40))) {
	/* FAT: prevent redundant player update */
	redrawPlayer[packet->pnum] = 1;
    } else
	return;

#ifdef CHECK_DROPPED
/* TEST */
/* For the dropped uncloak kludge, completely ignore uncloaks :-) */
/*    if(players[packet->pnum].p_flags & PFCLOAK) packet->flags |= htonl(PFCLOA(unsigned)K);*/
/* TEST */
    /* when a player cloaks, clear his phaser */
    if (ntohl((unsigned)packet->flags) & PFCLOAK) {
	if (phasers[packet->pnum].ph_status != PFREE) {
	    if (reportDroppedPackets)
		printf("Lost phaser free packet, player %d. (cloaked)\n", packet->pnum);
	    phasers[packet->pnum].ph_status = PHFREE;
	    phasers[packet->pnum].ph_fuse = 0;
	}
    }
#endif

    players[packet->pnum].p_flags = ntohl((unsigned)packet->flags);
#ifdef INCLUDE_VISTRACT
    if (packet->tractor & 0x40)
	players[packet->pnum].p_tractor = (short) packet->tractor & (~0x40);	/* ATM - visible
										   tractors */
    else
#endif				/* INCLUDE_VISTRACT */
	players[packet->pnum].p_tractor = -1;
}

static void
handleKills(packet)
    struct kills_spacket *packet;
{

    SANITY_PNUM(packet->pnum);

    if (players[packet->pnum].p_kills != ntohl((unsigned)packet->kills) / 100.0) {
	players[packet->pnum].p_kills = ntohl((unsigned)packet->kills) / 100.0;
	/* FAT: prevent redundant player update */
	updatePlayer[packet->pnum] |= ALL_UPDATE;
	if (infomapped && infotype == PLAYERTYPE &&
	    ((struct player *) infothing)->p_no == packet->pnum)
	    infoupdate = 1;
#ifdef PLPROF
	printf("Got handleKills for %d\n", packet->pnum);
#endif				/* PLPROF */
#ifdef ARMY_SLIDER
	if (me == &players[packet->pnum]) {
	    calibrate_stats();
	    redrawStats();
	}
#endif				/* ARMY_SLIDER */
    }
}

static void
handlePStatus(packet)
    struct pstatus_spacket *packet;
{
    SANITY_PNUM(packet->pnum);

    if (packet->status == PEXPLODE) {
	players[packet->pnum].p_explode = 0;
    }
    /*
       Ignore DEAD status. Instead, we treat it as PEXPLODE. This gives us
       time to animate all the frames necessary for the explosions at our own
       pace.
    */
    if (packet->status == PDEAD) {
	packet->status = PEXPLODE;
    }
    if (players[packet->pnum].p_status != packet->status) {
	players[packet->pnum].p_status = packet->status;
	redrawPlayer[packet->pnum] = 1;
#ifdef PLPROF
	printf("Got handlePStatus for %d\n", packet->pnum);
#endif				/* PLPROF */
	updatePlayer[packet->pnum] |= ALL_UPDATE;
	if (infomapped && infotype == PLAYERTYPE &&
	    ((struct player *) infothing)->p_no == packet->pnum)
	    infoupdate = 1;
#ifdef CHECK_DROPPED
	if (players[packet->pnum].p_status == POUTFIT) {
	    int     i;
	    /* clear phasers on this guy */
#if 0
	    for (i = 0; i < nplayers; i++) {
		if (phasers[i].ph_target == packet->pnum && phasers[i].ph_status == PHHIT) {
		    if (reportDroppedPackets)
			printf("Lost phaser free packet, player %d->player %d (target not alive)\n",
			       i, packet->pnum);
		    phasers[i].ph_status = PHFREE;
		}
	    }
#endif
	    if (phasers[packet->pnum].ph_status != PHFREE) {
		if (reportDroppedPackets)
		    printf("Lost phaser free packet, player %d (outfitting)\n", packet->pnum);
		phasers[packet->pnum].ph_status = PHFREE;	/* and his own */
	    }
	    if (reportDroppedPackets && players[packet->pnum].p_ntorp > 1)
		/* only report it on 2 or more left, always clear it. */
		printf("Lost torp free packet, (%d torps) player %d (outfitting)\n",
		       players[packet->pnum].p_ntorp, packet->pnum);
	    players[packet->pnum].p_ntorp = 0;
	    for (i = packet->pnum * ntorps; i < (packet->pnum + 1) * ntorps; i++) {
		torps[i].t_status = TFREE;
	    }
	}
#endif
    }
}

static void
handleMotd(packet)
    struct motd_spacket *packet;
{
    newMotdLine((char *) packet->line);
}

void
sendMessage(mes, group, indiv)
    char   *mes;
    int     group, indiv;
{
    struct mesg_cpacket mesPacket;

#ifdef SHORT_PACKETS
    if (recv_short) {
	int     size;
	size = strlen(mes);
	size += 5;		/* 1 for '\0', 4 packetheader */
	if ((size % 4) != 0)
	    size += (4 - (size % 4));
	mesPacket.pad1 = (char) size;

	/*
	   OH SHIT!!!! sizes[CP_S_MESSAGE] = size;
	*/

	mesPacket.type = CP_S_MESSAGE;
    } else
#endif
	mesPacket.type = CP_MESSAGE;
    mesPacket.group = group;
    mesPacket.indiv = indiv;
    strcpy(mesPacket.mesg, mes);
    sendServerPacket((struct player_spacket *) & mesPacket);
}

static void
handleMask(packet)
    struct mask_spacket *packet;
{
    tournMask = packet->mask;
}

void
sendOptionsPacket()
{
    struct options_cpacket optPacket;
    register int i;
    long    flags;

    optPacket.type = CP_OPTIONS;
    flags = (
	     ST_NOBITMAPS * (!sendmotdbitmaps) +
	     ST_KEEPPEACE * keeppeace +
	     0
	);
    optPacket.flags = htonl((unsigned)flags);
    /* copy the keymap and make sure no ctrl chars are sent [BDyess] */
    for (i = 32; i < 128; i++) {
	optPacket.keymap[i - 32] =
	    (myship->s_keymap[i] & 128) ? i : myship->s_keymap[i];
    }
    /* bcopy(mystats->st_keymap+32, optPacket.keymap,96); */
    sendServerPacket((struct player_spacket *) & optPacket);
}

static void
pickSocket(old)
    int     old;
{
    int     newsocket;
    struct socket_cpacket sockPack;
#ifdef RWATCH
    nextSocket = old;
#else
    newsocket = (getpid() & 32767);
    if (ghoststart)
	nextSocket = old;
    while (newsocket < 2048 || newsocket == old) {
	newsocket = (newsocket + 10687) & 32767;
    }
    sockPack.type = CP_SOCKET;
    sockPack.socket = htonl((unsigned)newsocket);
    sockPack.version = (char) SOCKVERSION;
    sockPack.udp_version = (char) UDPVERSION;
    sendServerPacket((struct player_spacket *) & sockPack);
    /* Did we get new socket # sent? */
    if (serverDead)
	return;
    nextSocket = newsocket;
#endif				/* RWATCH */
}

static void
handleBadVersion(packet)
    struct badversion_spacket *packet;
{
    switch (packet->why) {
    case 0:
	printf("Sorry, this is an invalid client version.\n");
	printf("You need a new version of the client code.\n");
	break;
    default:
	printf("Sorry, but you cannot play xtrek now.\n");
	printf("Try again later.\n");
	break;
    }
    EXIT(1);
}

int
gwrite(fd, buffer, bytes)
    int     fd;
    char   *buffer;
    register int bytes;
{
    long    orig = bytes;
    register long n;
#ifdef RECORDER
    if (playback)		/* pretend all is well */
	return (bytes);
#endif
    while (bytes) {
	n = sock_write(fd, buffer, (unsigned)bytes);
	if (n < 0) {
	    if (fd == udpSock) {
		fprintf(stderr, "Tried to write %d, 0x%x, %d\n",
			fd, (unsigned int) buffer, bytes);
		perror("write");
		printUdpInfo();
	    }
	    return (-1);
	}
	bytes -= n;
	buffer += n;
    }
    return (orig);
}

int
sock_read(s, data, size)
    int     s, size;
    char   *data;
{
#ifdef RECORDER
    if (playback)
	return readRecorded(s, data, size);
#endif

#ifndef AMIGA
    return read(s, data, (unsigned)size);
#else
#ifdef DNET
    return DNet_Read(s, data, size);
#else
    /* NET_SOCKETS code here */
    /* No idea if this works, old version had it: */
    return recv(s, data, size, 0);
#endif				/* DNET  */
#endif				/* AMIGA */
}

static void
handleHostile(packet)
    struct hostile_spacket *packet;
{
    register struct player *pl;

    SANITY_PNUM(packet->pnum);

    pl = &players[packet->pnum];
    if (pl->p_swar != packet->war ||
	pl->p_hostile != packet->hostile) {
	/* FAT: prevent redundant player update & redraw */
	pl->p_swar = packet->war;
	pl->p_hostile = packet->hostile;
	/* updatePlayer[packet->pnum]=1; why? */
	redrawPlayer[packet->pnum] = 1;
    }
}

static void
handlePlyrLogin(packet)
    struct plyr_login_spacket *packet;
{
    register struct player *pl;

    SANITY_PNUM(packet->pnum);

#ifdef PLPROF
    printf("Got handlPlyrLogin for %d\n", packet->pnum);
#endif				/* PLPROF */
    updatePlayer[packet->pnum] |= ALL_UPDATE;
    pl = &players[packet->pnum];

    strcpy(pl->p_name, packet->name);
    strcpy(pl->p_monitor, packet->monitor);
    strcpy(pl->p_login, packet->login);
    pl->p_stats.st_rank = packet->rank;
    if (packet->pnum == me->p_no) {
	/* This is me.  Set some stats */
	if (lastRank == -1) {
	    if (loggedIn) {
		lastRank = packet->rank;
	    }
	} else {
	    if (lastRank != packet->rank) {
		lastRank = packet->rank;
		promoted = 1;
	    }
	}
    }
}

static void
handleStats(packet)
    struct stats_spacket *packet;
{
    register struct player *pl;

    SANITY_PNUM(packet->pnum);

#ifdef PLPROF
    printf("Got handleStats for %d\n", packet->pnum);
#endif				/* PLPROF */
    updatePlayer[packet->pnum] |= LARGE_UPDATE;
    if (infomapped && infotype == PLAYERTYPE &&
	((struct player *) infothing)->p_no == packet->pnum)
	infoupdate = 1;
    pl = &players[packet->pnum];
    pl->p_stats.st_tkills = ntohl((unsigned)packet->tkills);
    pl->p_stats.st_tlosses = ntohl((unsigned)packet->tlosses);
    pl->p_stats.st_kills = ntohl((unsigned)packet->kills);
    pl->p_stats.st_losses = ntohl((unsigned)packet->losses);
    pl->p_stats.st_tticks = ntohl((unsigned)packet->tticks);
    pl->p_stats.st_tplanets = ntohl((unsigned)packet->tplanets);
    pl->p_stats.st_tarmsbomb = ntohl((unsigned)packet->tarmies);
    pl->p_stats.st_sbkills = ntohl((unsigned)packet->sbkills);
    pl->p_stats.st_sblosses = ntohl((unsigned)packet->sblosses);
    pl->p_stats.st_armsbomb = ntohl((unsigned)packet->armies);
    pl->p_stats.st_planets = ntohl((unsigned)packet->planets);
    pl->p_stats.st_maxkills = ntohl((unsigned)packet->maxkills) / 100.0;
    pl->p_stats.st_sbmaxkills = ntohl((unsigned)packet->sbmaxkills) / 100.0;
}

static void
handlePlyrInfo(packet)
    struct plyr_info_spacket *packet;
{
    register struct player *pl;
    static int lastship = -1;

    SANITY_PNUM(packet->pnum);

#ifdef PLPROF
    printf("Got PlyrInfo for %d\n", packet->pnum);
#endif				/* PLPROF */
    updatePlayer[packet->pnum] |= ALL_UPDATE;
    if (infomapped && infotype == PLAYERTYPE &&
	((struct player *) infothing)->p_no == packet->pnum)
	infoupdate = 1;
    pl = &players[packet->pnum];
    pl->p_ship = getship(packet->shiptype);
    if (packet->pnum == me->p_no && currentship != packet->shiptype) {
	currentship = packet->shiptype;
	/* sendOptionsPacket(); */
    }
    pl->p_teami = mask_to_idx(packet->team);
    if(pl->p_teami > number_of_teams) pl->p_teami = number_of_teams;
    pl->p_mapchars[0] = teaminfo[pl->p_teami].letter;
    /* printf("team: %d, letter: %c\n",pl->p_teami,pl->p_mapchars[0]); */
    pl->p_mapchars[1] = shipnos[pl->p_no];
    if (me == pl && lastship != currentship) {
	lastship = currentship;
	redrawTstats();
	calibrate_stats();
	redrawStats();		/* tsh */
    }
    redrawPlayer[packet->pnum] = 1;
}

void
sendUpdatePacket(speed)
    long    speed;
{
    struct updates_cpacket packet;

    packet.type = CP_UPDATES;
    timerDelay = speed;
    packet.usecs = htonl((unsigned)speed);
    sendServerPacket((struct player_spacket *) & packet);
#ifdef DEBUG
    printf("sent request for an update every %d microseconds (%d/sec)\n",speed,1000000/speed);
#endif
}

static void
handlePlanetLoc(packet)
    struct planet_loc_spacket *packet;
{
    struct planet *pl;

    SANITY_PLANNUM(packet->pnum);

    pl = &planets[packet->pnum];
    pl_update[packet->pnum].plu_x = pl->pl_x;
    pl_update[packet->pnum].plu_y = pl->pl_y;

    if (pl_update[packet->pnum].plu_update != -1) {
	pl_update[packet->pnum].plu_update = 1;
	/*
	   printf("update: %s, old (%d,%d) new (%d,%d)\n", pl->pl_name,
	   pl->pl_x, pl->pl_y, ntohl((unsigned)packet->x),ntohl((unsigned)packet->y));
	*/
    } else {
	pl_update[packet->pnum].plu_update = 0;
	pl_update[packet->pnum].plu_x = ntohl((unsigned)packet->x);
	pl_update[packet->pnum].plu_y = ntohl((unsigned)packet->y);
    }
    pl->pl_x = ntohl((unsigned)packet->x);
    pl->pl_y = ntohl((unsigned)packet->y);
    strcpy(pl->pl_name, packet->name);
    pl->pl_namelen = strlen(packet->name);
    pl->pl_flags |= PLREDRAW;
    if (infomapped && infotype == PLANETTYPE &&
	((struct planet *) infothing)->pl_no == packet->pnum)
	infoupdate = 1;
    reinitPlanets = 1;
    if (pl->pl_x > blk_gwidth) {
	blk_gwidth = 200000;
	blk_windgwidth = ((float) MAPSIDE) / blk_gwidth;
    }
#ifdef ROTATERACE
    if (rotate) {
	rotate_gcenter(&pl->pl_x, &pl->pl_y);
    }
#endif
}

#if 0

static void
handleReserved(packet)
    struct reserved_spacket *packet;
{
    struct reserved_cpacket response;

#ifndef RWATCH
    encryptReservedPacket(packet, &response, serverName, me->p_no);
    sendServerPacket((struct player_spacket *) & response);
#endif				/* RWATCH */
}

#else


static void
handleReserved(packet)
    struct reserved_spacket *packet;
{
#ifdef AUTHORIZE
    struct reserved_cpacket response;

    response.type = CP_RESERVED;
    if (RSA_Client) {		/* can use -o option for old blessing */
	warning(RSA_VERSION);
	strncpy(response.resp, RSA_VERSION, RESERVED_SIZE);
	memcpy(response.data, packet->data, RESERVED_SIZE);
    } else {
#if 0
	/*
	   if we ever go back to reserved.c checking, we'll reactivate this
	   code.  However, our reserved.c module hasn't done any real
	   computation since paradise 1.
	*/
	encryptReservedPacket(packet, &response, serverName, me->p_no);
#else
	memcpy(response.data, packet->data, 16);
	memcpy(response.resp, packet->data, 16);
#endif
    }
    sendServerPacket((struct player_spacket *) & response);
#endif
}

static void
handleRSAKey(packet)
    struct rsa_key_spacket *packet;
{
#ifdef AUTHORIZE
    struct rsa_key_cpacket response;
    void rsa_black_box P(( unsigned char* out, unsigned char* in, 
                           unsigned char* public, unsigned char* global));
#ifndef DNET
    struct sockaddr_in saddr;
#endif
    unsigned char *data;
#ifndef TERM
    int     len;
#endif /*!TERM [BDyess]*/

#ifdef GATEWAY
    extern unsigned long netaddr;
#endif

#ifndef DNET
#ifdef GATEWAY
    /* if we didn't get it from -H, go ahead and query the socket */
    if (netaddr == 0) {
	len = sizeof(saddr);
	if (getpeername(sock, (struct sockaddr *) & saddr, &len) < 0) {
	    perror("getpeername(sock)");
	    exit(1);
	}
    } else {
	saddr.sin_addr.s_addr = serveraddr; /*netaddr;*/
	saddr.sin_port = serv_port;
    }
#else				/* GATEWAY */
#ifdef TERM
    saddr.sin_addr.s_addr = serveraddr;
    saddr.sin_port = serv_port;
#else				/* TERM [BDyess] */
    /* query the socket to determine the remote host (ATM) */
    len = sizeof(saddr);
    if (getpeername(sock, (struct sockaddr *) & saddr, &len) < 0) {
	perror("getpeername(sock)");
	exit(1);
    }
#endif 				/* TERM [BDyess] */
#endif				/* GATEWAY */

    data = packet->data;
    bcopy(&saddr.sin_addr.s_addr, data, sizeof(saddr.sin_addr.s_addr));
    data += sizeof(saddr.sin_addr.s_addr);
    bcopy(&saddr.sin_port, data, sizeof(saddr.sin_port));
#else				/* DNET */
    {
	extern long netrek_server_addr;
	extern unsigned short netrek_server_port;

	data = packet->data;
	bcopy(&netrek_server_addr, data, sizeof(netrek_server_addr));
	data += sizeof(netrek_server_addr);
	bcopy(&netrek_server_port, data, sizeof(netrek_server_port));
    }
#endif				/* DNET */

#ifdef DEBUG
    {
	struct timeval start, end;
	gettimeofday(&start, NULL);
#endif
	rsa_black_box(response.resp, packet->data,
		      response.public, response.global);
#ifdef DEBUG
	gettimeofday(&end, NULL);
	printf("rsa_black_box took %d ms.\n",
	       (1000 * (end.tv_sec - start.tv_sec) +
		(end.tv_usec - start.tv_usec) / 1000));
    }
#endif
    response.type = CP_RSA_KEY;

    sendServerPacket((struct player_spacket *) & response);
#ifdef DEBUG
    printf("RSA verification requested.\n");
#endif
#endif				/* AUTHORIZE */
}

#endif

#ifdef INCLUDE_SCAN
static void
handleScan(packet)
    struct scan_spacket *packet;
{
    struct player *pp;

    SANITY_PNUM(packet->pnum);

    if (packet->success) {
	pp = &players[packet->pnum];
	pp->p_fuel = ntohl((unsigned)packet->p_fuel);
	pp->p_armies = ntohl((unsigned)packet->p_armies);
	pp->p_shield = ntohl((unsigned)packet->p_shield);
	pp->p_damage = ntohl((unsigned)packet->p_damage);
	pp->p_etemp = ntohl((unsigned)packet->p_etemp);
	pp->p_wtemp = ntohl((unsigned)packet->p_wtemp);
	informScan(packet->pnum);
    }
}

static void
informScan(p)
    int     p;
{
}

#endif				/* INCLUDE_SCAN */

/*
 * UDP stuff
 */
void
sendUdpReq(req)
    int     req;
{
    struct udp_req_cpacket packet;
#ifdef RWATCH
    return;
#endif				/* RWATCH */

    packet.type = CP_UDP_REQ;
    packet.request = req;

    if (req >= COMM_MODE) {
	packet.request = COMM_MODE;
	packet.connmode = req - COMM_MODE;
	sendServerPacket((struct player_spacket *) & packet);
	return;
    }
    if (req == COMM_UPDATE) {
#ifdef SHORT_PACKETS
	if (recv_short) {	/* not necessary */
/* Let the client do the work, and not the network :-) */
	    register int i;
	    for (i = 0; i < nplayers * ntorps; i++)
		torps[i].t_status = TFREE;

	    for (i = 0; i < nplayers * nplasmas; i++)
		plasmatorps[i].pt_status = PTFREE;

	    for (i = 0; i < nplayers; i++) {
		players[i].p_ntorp = 0;
		players[i].p_nplasmatorp = 0;
		phasers[i].ph_status = PHFREE;
	    }
	}
#endif
	sendServerPacket((struct player_spacket *) & packet);
	warning("Sent request for full update");
	return;
    }
    if (req == commModeReq) {
	warning("Request is in progress, do not disturb");
	return;
    }
    if (req == COMM_UDP) {
	/* open UDP port */
	if (openUdpConn() >= 0) {
	    UDPDIAG(("Bound to local port %d on fd %d\n", udpLocalPort, udpSock));
	} else {
	    UDPDIAG(("Bind to local port %d failed\n", udpLocalPort));
	    commModeReq = COMM_TCP;
	    commStatus = STAT_CONNECTED;
	    commSwitchTimeout = 0;
	    if (udpWin)
		udprefresh(UDP_STATUS);
	    warning("Unable to establish UDP connection");

	    return;
	}
    }
    /* send the request */
    packet.type = CP_UDP_REQ;
    packet.request = req;
    packet.port = htonl((unsigned)udpLocalPort);
#ifdef GATEWAY
    if (!strcmp(serverName, gw_mach)) {
	packet.port = htons(gw_serv_port);	/* gw port that server should
						   call */
	UDPDIAG(("+ Telling server to contact us on %d\n", gw_serv_port));
    }
#endif
#ifdef USE_PORTSWAP
    packet.connmode = CONNMODE_PORT;	/* have him send his port */
#else
    packet.connmode = CONNMODE_PACKET;	/* we get addr from packet */
#endif
    sendServerPacket((struct player_spacket *) & packet);

    /* update internal state stuff */
    commModeReq = req;
    if (req == COMM_TCP)
	commStatus = STAT_SWITCH_TCP;
    else
	commStatus = STAT_SWITCH_UDP;
    commSwitchTimeout = 25;	/* wait 25 updates (about five seconds) */

    UDPDIAG(("Sent request for %s mode\n", (req == COMM_TCP) ?
	     "TCP" : "UDP"));

#ifndef USE_PORTSWAP
    if ((req == COMM_UDP) && recvUdpConn() < 0) {
	UDPDIAG(("Sending TCP reset message\n"));
	packet.request = COMM_TCP;
	packet.port = 0;
	commModeReq = COMM_TCP;
	sendServerPacket((struct player_spacket *) & packet);
	/* we will likely get a SWITCH_UDP_OK later; better ignore it */
	commModeReq = COMM_TCP;
	commStatus = STAT_CONNECTED;
	commSwitchTimeout = 0;
	closeUdpConn();
    }
#endif

    if (udpWin)
	udprefresh(UDP_STATUS);
}

static void
handleUdpReply(packet)
    struct udp_reply_spacket *packet;
{
    struct udp_req_cpacket response;

    UDPDIAG(("Received UDP reply %d\n", packet->reply));
    commSwitchTimeout = 0;

    response.type = CP_UDP_REQ;

    switch (packet->reply) {
    case SWITCH_TCP_OK:
	if (commMode == COMM_TCP) {
	    UDPDIAG(("Got SWITCH_TCP_OK while in TCP mode; ignoring\n"));
	} else {
	    commMode = COMM_TCP;
	    commStatus = STAT_CONNECTED;
	    warning("Switched to TCP-only connection");
	    closeUdpConn();
	    UDPDIAG(("UDP port closed\n"));
	    if (udpWin) {
		udprefresh(UDP_STATUS);
		udprefresh(UDP_CURRENT);
	    }
	}
	break;
    case SWITCH_UDP_OK:
	if (commMode == COMM_UDP) {
	    UDPDIAG(("Got SWITCH_UDP_OK while in UDP mode; ignoring\n"));
	} else {
	    /* the server is forcing UDP down our throat? */
	    if (commModeReq != COMM_UDP) {
		UDPDIAG(("Got unsolicited SWITCH_UDP_OK; ignoring\n"));
	    } else {
#ifdef USE_PORTSWAP
		udpServerPort = ntohl((unsigned)packet->port);
#ifdef nodef			/* simulate calvin error */
		/* XXX TMP */
		udpServerPort = 3333;
#endif
		if (connUdpConn() < 0) {
		    UDPDIAG(("Unable to connect, resetting\n"));
		    warning("Connection attempt failed");
		    commModeReq = COMM_TCP;
		    commStatus = STAT_CONNECTED;
		    if (udpSock >= 0)
			closeUdpConn();
		    if (udpWin) {
			udprefresh(UDP_STATUS);
			udprefresh(UDP_CURRENT);
		    }
		    response.request = COMM_TCP;
		    response.port = 0;
		    goto send;
		}
#else
		/* this came down UDP, so we MUST be connected */
		/* (do the verify thing anyway just for kicks) */
#endif
		UDPDIAG(("Connected to server's UDP port\n"));
		commStatus = STAT_VERIFY_UDP;
		if (udpWin)
		    udprefresh(UDP_STATUS);
		response.request = COMM_VERIFY;	/* send verify request on UDP */
		response.port = 0;
		commSwitchTimeout = 25;	/* wait 25 updates */
#ifdef USE_PORTSWAP
	send:
#endif				/* USE_PORTSWAP */
		sendServerPacket((struct player_spacket *) & response);
	    }
	}
	break;
    case SWITCH_DENIED:
	if (ntohs(packet->port)) {
	    UDPDIAG(("Switch to UDP failed (different version)\n"));
	    warning("UDP protocol request failed (bad version)");
	} else {
	    UDPDIAG(("Switch to UDP denied\n"));
	    warning("UDP protocol request denied");
	}
	commModeReq = commMode;
	commStatus = STAT_CONNECTED;
	commSwitchTimeout = 0;
	if (udpWin)
	    udprefresh(UDP_STATUS);
	if (udpSock >= 0)
	    closeUdpConn();
	break;
    case SWITCH_VERIFY:
	UDPDIAG(("Received UDP verification\n"));
	break;
    default:
	fprintf(stderr, "netrek: Got funny reply (%d) in UDP_REPLY packet\n",
		packet->reply);
	break;
    }
}

#define MAX_PORT_RETRY  10
static int
openUdpConn()
{
#ifndef DNET
    struct sockaddr_in addr;
    struct hostent *hp;
    int     attempts;

#ifdef RECORDER
    if (playback)
	return 0;
#endif
    if (udpSock >= 0) {
	fprintf(stderr, "netrek: tried to open udpSock twice\n");
	return (0);		/* pretend we succeeded (this could be bad) */
    }
    if ((udpSock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	perror("netrek: unable to create DGRAM socket");
	return (-1);
    }
#ifdef nodef
    set_udp_opts(udpSock);
#endif				/* nodef */

    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;

    errno = 0;
    udpLocalPort = (getpid() & 32767) + (random() % 256);
    for (attempts = 0; attempts < MAX_PORT_RETRY; attempts++) {
	while (udpLocalPort < 2048) {
	    udpLocalPort = (udpLocalPort + 10687) & 32767;
	}
#ifdef GATEWAY
	/* we need the gateway to know where to find us */
	if (!strcmp(serverName, gw_mach)) {
	    UDPDIAG(("+ gateway test: binding to %d\n", gw_local_port));
	    udpLocalPort = gw_local_port;
	}
#endif
	addr.sin_port = htons(udpLocalPort);
	if (bind(udpSock, (struct sockaddr *) & addr, sizeof(addr)) >= 0)
	    break;
    }
    if (attempts == MAX_PORT_RETRY) {
	perror("netrek: bind");
	UDPDIAG(("Unable to find a local port to bind to\n"));
	close(udpSock);
	udpSock = -1;
	return (-1);
    }
    UDPDIAG(("Local port is %d\n", udpLocalPort));

    /* determine the address of the server */
    if (!serveraddr) {
	if ((addr.sin_addr.s_addr = inet_addr(serverName)) == -1) {
	    if ((hp = gethostbyname(serverName)) == NULL) {
		printf("Who is %s?\n", serverName);
#ifdef AUTOKEY
		if (autoKey)
		    W_AutoRepeatOn();
#endif

		EXIT(0);
	    } else {
		addr.sin_addr.s_addr = *(long *) hp->h_addr;
	    }
	}
	serveraddr = addr.sin_addr.s_addr;
	UDPDIAG(("Found serveraddr == 0x%x\n", (unsigned int) serveraddr));
    }
    return (0);
#else				/* DNET */
#ifdef RECORDER
    if (playback)
	return 0;
#endif
    return DNetOpenUDPConn(serverName);
#endif				/* DNET */
}

#ifdef USE_PORTSWAP
static int
connUdpConn()
{
#ifndef DNET
    struct sockaddr_in addr;
    int     len;

#ifdef RECORDER
    if (playback)
	return 0;
#endif
    addr.sin_addr.s_addr = serveraddr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(udpServerPort);

    UDPDIAG(("Connecting to host 0x%x on port %d\n", serveraddr, udpServerPort));
    if (connect(udpSock, &addr, sizeof(addr)) < 0) {
	fprintf(stderr, "Error %d: ");
	perror("netrek: unable to connect UDP socket");
	printUdpInfo();		/* debug */
	return (-1);
    }
#ifdef nodef
    len = sizeof(addr);
    if (getsockname(udpSock, &addr, &len) < 0) {
	perror("netrek: unable to getsockname(UDP)");
	UDPDIAG(("Can't get our own socket; connection failed\n"));
	close(udpSock);
	udpSock = -1;
	return -1;
    }
    printf("udpLocalPort %d, getsockname port %d\n",
	   udpLocalPort, addr.sin_port);
#endif

    return (0);
#endif				/* DNET */
}
#endif

#ifndef USE_PORTSWAP
static int
recvUdpConn()
{
#ifndef DNET
    fd_set  readfds;
    struct timeval to;
    struct sockaddr_in from;
    int     fromlen, res;

#ifdef RECORDER
    if (playback)
	return 0;
#endif
    bzero(&from, sizeof(from));	/* don't get garbage if really broken */

    /* we patiently wait until the server sends a packet to us */
    /* (note that we silently eat the first one) */
    UDPDIAG(("Issuing recvfrom() call\n"));
    printUdpInfo();
    fromlen = sizeof(from);
    FD_ZERO(&readfds);
    FD_SET(udpSock, &readfds);
    to.tv_sec = 6;		/* wait 3 seconds, then abort */
    to.tv_usec = 0;
    if ((res = select(32, &readfds, 0, 0, &to)) <= 0) {
	if (!res) {
	    UDPDIAG(("timed out waiting for response"));
	    warning("UDP connection request timed out");
	    return (-1);
	} else {
	    perror("select() before recvfrom()");
	    return (-1);
	}
    }
    if (recvfrom(udpSock, buf, BUFSIZ, 0, (struct sockaddr *) & from, &fromlen) < 0) {
	perror("recvfrom");
	UDPDIAG(("recvfrom failed, aborting UDP attempt"));
	return (-1);
    }
    if (from.sin_addr.s_addr != serveraddr) {
	/* safe? */
	serveraddr = from.sin_addr.s_addr;
	UDPDIAG(("Warning: from 0x%x, but server is 0x%x\n",
	   (unsigned int) from.sin_addr.s_addr, (unsigned int) serveraddr));
    }
    if (from.sin_family != AF_INET) {
	UDPDIAG(("Warning: not AF_INET (%d)\n", from.sin_family));
    }
    udpServerPort = ntohs(from.sin_port);
    UDPDIAG(("recvfrom() succeeded; will use server port %d\n", udpServerPort));
#ifdef GATEWAY
    if (!strcmp(serverName, gw_mach)) {
	UDPDIAG(("+ actually, I'm going to use %d\n", gw_port));
	udpServerPort = gw_port;
	from.sin_port = htons(udpServerPort);
    }
#endif

    if (connect(udpSock, (struct sockaddr *) & from, sizeof(from)) < 0) {
	perror("netrek: unable to connect UDP socket after recvfrom()");
	close(udpSock);
	udpSock = -1;
	return (-1);
    }
    return (0);
#else				/* DNET */
#ifdef RECORDER
    if (playback)
	return 0;
#endif
    return DNetRecvUDPConn();
#endif				/* DNET */
}
#endif

int
closeUdpConn()
{
    V_UDPDIAG(("Closing UDP socket\n"));
#ifdef RECORDER
    if (playback)
	return 0;
#endif

#ifdef DNET
    DNetCloseUDP();
    return 0;
#else
    if (udpSock < 0) {
	fprintf(stderr, "netrek: tried to close a closed UDP socket\n");
	return (-1);
    }
    shutdown(udpSock, 2);
    close(udpSock);
    udpSock = -1;
    return 0;
#endif				/* DNET */
}

static void
printUdpInfo()
{
#ifndef DNET
    struct sockaddr_in addr;
    int     len;

    len = sizeof(addr);
    if (getsockname(udpSock, (struct sockaddr *) & addr, &len) < 0) {
/*      perror("printUdpInfo: getsockname");*/
	return;
    }
    UDPDIAG(("LOCAL: addr=0x%x, family=%d, port=%d\n",
	     (unsigned int) addr.sin_addr.s_addr,
	     addr.sin_family, ntohs(addr.sin_port)));

    if (getpeername(udpSock, (struct sockaddr *) & addr, &len) < 0) {
/*      perror("printUdpInfo: getpeername");*/
	return;
    }
    UDPDIAG(("PEER : addr=0x%x, family=%d, port=%d\n",
	     (unsigned int) addr.sin_addr.s_addr,
	     addr.sin_family, ntohs(addr.sin_port)));
#endif
}

static void
handleSequence(packet)
    struct sequence_spacket *packet;
{
    static int recent_count = 0, recent_dropped = 0;
    long    newseq;

    drop_flag = 0;
    if (chan != udpSock)
	return;			/* don't pay attention to TCP sequence #s */
    udpTotal++;
    recent_count++;

    /* update percent display every 256 updates (~50 seconds usually) */
    if (!(udpTotal & 0xff))
	if (udpWin)
	    udprefresh(UDP_DROPPED);

    newseq = (long) ntohs(packet->sequence);
/*    printf("read %d - ", newseq);*/

    if (((unsigned short) sequence) > 65000 &&
	((unsigned short) newseq) < 1000) {
	/* we rolled, set newseq = 65536+sequence and accept it */
	sequence = ((sequence + 65536) & 0xffff0000) | newseq;
    } else {
	/* adjust newseq and do compare */
	newseq |= (sequence & 0xffff0000);

	if (!udpSequenceChk) {	/* put this here so that turning seq check */
	    sequence = newseq;	/* on and off doesn't make us think we lost */
	    return;		/* a whole bunch of packets. */
	}
	if (newseq > sequence) {
	    /* accept */
	    if (newseq != sequence + 1) {
		udpDropped += (newseq - sequence) - 1;
		udpTotal += (newseq - sequence) - 1;	/* want TOTAL packets */
		recent_dropped += (newseq - sequence) - 1;
		recent_count += (newseq - sequence) - 1;
		if (udpWin)
		    udprefresh(UDP_DROPPED);
		UDPDIAG(("sequence=%ld, newseq=%ld, we lost some\n",
			 sequence, newseq));
	    }
	    sequence = newseq;
	} else {
	    /* reject */
	    if (packet->type == SP_SC_SEQUENCE) {
		V_UDPDIAG(("(ignoring repeat %ld)\n", newseq));
	    } else {
		UDPDIAG(("sequence=%ld, newseq=%ld, ignoring transmission\n",
			 sequence, newseq));
	    }
	    /*
	       the remaining packets will be dropped and we shouldn't count
	       the SP_SEQUENCE packet either
	    */
	    packets_received--;
	    drop_flag = 1;
	}
    }
/*    printf("newseq %d, sequence %d\n", newseq, sequence);*/
    if (recent_count > UDP_RECENT_INTR) {
	/* once a minute (at 5 upd/sec), report on how many were dropped */
	/* during the last UDP_RECENT_INTR updates                       */
	udpRecentDropped = recent_dropped;
	recent_count = recent_dropped = 0;
	if (udpWin)
	    udprefresh(UDP_DROPPED);
    }
}


/*
static void dumpShip(shipp)
struct ship *shipp;
{
  printf("ship stats:\n");
  printf("phaser range = %d\n", shipp->s_phaserrange);
  printf("max speed = %d\n", shipp->s_maxspeed);
  printf("max shield = %d\n", shipp->s_maxshield);
  printf("max damage = %d\n", shipp->s_maxdamage);
  printf("max egntemp = %d\n", shipp->s_maxegntemp);
  printf("max wpntemp = %d\n", shipp->s_maxwpntemp);
  printf("max armies = %d\n", shipp->s_maxarmies);
  printf("type = %d\n", shipp->s_type);
  printf("torp speed = %d\n", shipp->s_torpspeed);
  printf("letter = %c\n", shipp->s_letter);
  printf("desig = %2.2s\n", shipp->s_desig);
  printf("bitmap = %d\n\n", shipp->s_bitmap);
}
*/

static void
handleShipCap(packet)		/* SP_SHIP_CAP */
    struct ship_cap_spacket *packet;
{
    struct shiplist *temp;

    /*
       What are we supposed to do?
    */

    SANITY_SHIPNUM(ntohs(packet->s_type));

    if (packet->operation) {	/* remove ship from list */
	temp = shiptypes;
	if (temp->ship->s_type == (int) ntohs(packet->s_type)) {
	    shiptypes = temp->next;
	    shiptypes->prev = NULL;
	}
	while (temp->next != NULL) {
	    if (temp->next->ship->s_type == (int) ntohs(packet->s_type)) {
		temp = temp->next;
		temp->prev->next = temp->next;
		if (temp->next)
		    temp->next->prev = temp->prev;
		free(temp->ship);
		free(temp);
		return;
	    } else {
		temp = temp->next;
	    }
	}
    }
    /*
       Since we're adding the ship, we need to find out if we already have
       that ship, and if so, replace it.
    */

    temp = shiptypes;
    while (temp != NULL) {
	if (temp->ship->s_type == (int) ntohs(packet->s_type)) {
	    temp->ship->s_type = ntohs(packet->s_type);
	    temp->ship->s_torpspeed = ntohs(packet->s_torpspeed);
	    temp->ship->s_phaserrange = ntohs(packet->s_phaserrange);
	    if (temp->ship->s_phaserrange < 200)	/* backward
							   compatibility */
		temp->ship->s_phaserrange *= PHASEDIST / 100;
	    temp->ship->s_maxspeed = ntohl((unsigned)packet->s_maxspeed);
	    temp->ship->s_maxfuel = ntohl((unsigned)packet->s_maxfuel);
	    temp->ship->s_maxshield = ntohl((unsigned)packet->s_maxshield);
	    temp->ship->s_maxdamage = ntohl((unsigned)packet->s_maxdamage);
	    temp->ship->s_maxwpntemp = ntohl((unsigned)packet->s_maxwpntemp);
	    temp->ship->s_maxegntemp = ntohl((unsigned)packet->s_maxegntemp);
	    temp->ship->s_maxarmies = ntohs(packet->s_maxarmies);
	    temp->ship->s_letter = packet->s_letter;
	    temp->ship->s_desig[0] = packet->s_desig1;
	    temp->ship->s_desig[1] = packet->s_desig2;
	    temp->ship->s_bitmap = ntohs(packet->s_bitmap);
	    buildShipKeymap(temp->ship);
/*dumpShip(temp->ship);*/
	    return;
	}
	temp = temp->next;
    }

    /*
       Not there, so we need to make a new entry in the list for it.
    */
    temp = (struct shiplist *) malloc(sizeof(struct shiplist));
    temp->next = shiptypes;
    temp->prev = NULL;
    if (shiptypes)
	shiptypes->prev = temp;
    shiptypes = temp;
    temp->ship = (struct ship *) malloc(sizeof(struct ship));
    temp->ship->s_type = ntohs(packet->s_type);
    temp->ship->s_torpspeed = ntohs(packet->s_torpspeed);
    temp->ship->s_phaserrange = ntohs(packet->s_phaserrange);
    temp->ship->s_maxspeed = ntohl((unsigned)packet->s_maxspeed);
    temp->ship->s_maxfuel = ntohl((unsigned)packet->s_maxfuel);
    temp->ship->s_maxshield = ntohl((unsigned)packet->s_maxshield);
    temp->ship->s_maxdamage = ntohl((unsigned)packet->s_maxdamage);
    temp->ship->s_maxwpntemp = ntohl((unsigned)packet->s_maxwpntemp);
    temp->ship->s_maxegntemp = ntohl((unsigned)packet->s_maxegntemp);
    temp->ship->s_maxarmies = ntohs(packet->s_maxarmies);
    temp->ship->s_letter = packet->s_letter;
    temp->ship->s_desig[0] = packet->s_desig1;
    temp->ship->s_desig[1] = packet->s_desig2;
    temp->ship->s_bitmap = ntohs(packet->s_bitmap);
    buildShipKeymap(temp->ship);
/*  dumpShip(temp->ship);*/
}

static void
handleMotdPic(packet)		/* SP_SHIP_CAP */
    struct motd_pic_spacket *packet;
{
    int     x, y, page, width, height;

    x = ntohs(packet->x);
    y = ntohs(packet->y);
    width = ntohs(packet->width);
    height = ntohs(packet->height);
    page = ntohs(packet->page);

    newMotdPic(x, y, width, height, (char *) packet->bits, page);
}

static void
handleStats2(packet)
    struct stats_spacket2 *packet;
{
    struct stats2 *p;		/* to hold packet's player's stats2 struct */

    SANITY_PNUM(packet->pnum);

    updatePlayer[packet->pnum] |= LARGE_UPDATE;
    if (infomapped && infotype == PLAYERTYPE &&
	((struct player *) infothing)->p_no == packet->pnum)
	infoupdate = 1;
    p = &(players[packet->pnum].p_stats2);	/* get player's stats2 struct */
    p->st_genocides = ntohl((unsigned)packet->genocides);
    p->st_tmaxkills = (float) ntohl((unsigned)packet->maxkills) / 100.0;
    p->st_di = (float) ntohl((unsigned)packet->di) / 100.0;
    p->st_tkills = (int) ntohl((unsigned)packet->kills);
    p->st_tlosses = (int) ntohl((unsigned)packet->losses);
    p->st_tarmsbomb = (int) ntohl((unsigned)packet->armsbomb);
    p->st_tresbomb = (int) ntohl((unsigned)packet->resbomb);
    p->st_tdooshes = (int) ntohl((unsigned)packet->dooshes);
    p->st_tplanets = (int) ntohl((unsigned)packet->planets);
    p->st_tticks = (int) ntohl((unsigned)packet->tticks);
    p->st_sbkills = (int) ntohl((unsigned)packet->sbkills);
    p->st_sblosses = (int) ntohl((unsigned)packet->sblosses);
    p->st_sbticks = (int) ntohl((unsigned)packet->sbticks);
    p->st_sbmaxkills = (float) ntohl((unsigned)packet->sbmaxkills) / 100.0;
    p->st_wbkills = (int) ntohl((unsigned)packet->wbkills);
    p->st_wblosses = (int) ntohl((unsigned)packet->wblosses);
    p->st_wbticks = (int) ntohl((unsigned)packet->wbticks);
    p->st_wbmaxkills = (float) ntohl((unsigned)packet->wbmaxkills) / 100.0;
    p->st_jsplanets = (int) ntohl((unsigned)packet->jsplanets);
    p->st_jsticks = (int) ntohl((unsigned)packet->jsticks);
    if (p->st_rank != (int) ntohl((unsigned)packet->rank) ||
	p->st_royal != (int) ntohl((unsigned)packet->royal)) {
	p->st_rank = (int) ntohl((unsigned)packet->rank);
	p->st_royal = (int) ntohl((unsigned)packet->royal);
	updatePlayer[packet->pnum] |= ALL_UPDATE;
    }
}

static void
handleStatus2(packet)
    struct status_spacket2 *packet;
{
    updatePlayer[me->p_no] |= LARGE_UPDATE;
    if (infomapped && infotype == PLAYERTYPE &&
	((struct player *) infothing)->p_no == me->p_no)
	infoupdate = 1;
    status2->tourn = packet->tourn;
    status2->dooshes = ntohl((unsigned)packet->dooshes);
    status2->armsbomb = ntohl((unsigned)packet->armsbomb);
    status2->resbomb = ntohl((unsigned)packet->resbomb);
    status2->planets = ntohl((unsigned)packet->planets);
    status2->kills = ntohl((unsigned)packet->kills);
    status2->losses = ntohl((unsigned)packet->losses);
    status2->sbkills = ntohl((unsigned)packet->sbkills);
    status2->sblosses = ntohl((unsigned)packet->sblosses);
    status2->sbtime = ntohl((unsigned)packet->sbtime);
    status2->wbkills = ntohl((unsigned)packet->wbkills);
    status2->wblosses = ntohl((unsigned)packet->wblosses);
    status2->wbtime = ntohl((unsigned)packet->wbtime);
    status2->jsplanets = ntohl((unsigned)packet->jsplanets);
    status2->jstime = ntohl((unsigned)packet->jstime);
    status2->time = ntohl((unsigned)packet->time);
    status2->timeprod = ntohl((unsigned)packet->timeprod);
}

static void
handlePlanet2(packet)
    struct planet_spacket2 *packet;
{
    SANITY_PLANNUM(packet->pnum);

    planets[packet->pnum].pl_owner = packet->owner;
    planets[packet->pnum].pl_info = packet->info;
    planets[packet->pnum].pl_flags = ntohl((unsigned)packet->flags);
    if(PL_TYPE(planets[packet->pnum]) != PLPLANET) {
      planets[packet->pnum].pl_owner = ALLTEAM;
    }
    planets[packet->pnum].pl_timestamp = ntohl((unsigned)packet->timestamp);
    planets[packet->pnum].pl_armies = ntohl((unsigned)packet->armies);
    planets[packet->pnum].pl_flags |= PLREDRAW;
    pl_update[packet->pnum].plu_update = 1;
    pl_update[packet->pnum].plu_x = planets[packet->pnum].pl_x;
    pl_update[packet->pnum].plu_y = planets[packet->pnum].pl_y;
    if (infomapped && infotype == PLANETTYPE &&
	((struct planet *) infothing)->pl_no == packet->pnum)
	infoupdate = 1;
}
#ifdef ASTEROIDS

static void 
handleTerrainInfo2(pkt)
  struct terrain_info_packet2 *pkt;
{
#ifdef ZDIAG2
    fprintf( stderr, "Receiving terrain info packet\n" );
    fprintf( stderr, "Terrain dims: %d x %d\n", ntohs(pkt->xdim), ntohs(pkt->ydim) );
#endif
    received_terrain_info = TERRAIN_STARTED;
    terrain_x = ntohs(pkt->xdim);
    terrain_y = ntohs(pkt->ydim);
}; 

static void
handleTerrain2(pkt)
    struct terrain_packet2 *pkt;
{
    static int curseq = 0, totbytes = 0, done = 0;
    int i;
#if defined(ZDIAG) || defined(ZDIAG2)
    int status;
#endif /* ZDIAG || ZDIAG2 */
    unsigned long dlen;
#ifdef ZDIAG2
    static unsigned char sum = 0;
    static unsigned numnz = 0;
#endif
    static unsigned char *gzipTerrain = NULL, *orgTerrain = NULL;
    
#ifdef ZDIAG2
    fprintf( stderr, "Receiving Terrain packet.  This should be %d.\n", curseq+1 );
#endif

    if( (done == TERRAIN_DONE) && (received_terrain_info == TERRAIN_STARTED ) ){
      /* receiving new terrain info */
      free( gzipTerrain );
      free( orgTerrain );
      free( terrainInfo );
      gzipTerrain = orgTerrain = NULL;
      terrainInfo = NULL;
      curseq = done = totbytes = 0;
    }
      
    curseq++;
    if( (curseq != pkt->sequence) || !(received_terrain_info) ){
      /* Should fill in a list of all packets missed */
      /* or request header packet from server */
      fprintf( stderr, "Blech!  Received terrain packet before terrain_info\n" );
      return;
    }
#ifdef ZDIAG2
    fprintf( stderr, "Receiving packet %d out of %d\n", curseq, pkt->total_pkts );
#endif
    if( !gzipTerrain ){
      gzipTerrain = (unsigned char *)malloc( pkt->total_pkts << 7 );
#if defined(ZDIAG) || defined(ZDIAG2)
      fprintf( stderr, "Allocating %d bytes for gzipTerrain.\n", pkt->total_pkts << 7 );
#endif
		/* another yukko constant */
    }
    if( !orgTerrain ){
      orgTerrain = (unsigned char *)malloc( terrain_x*terrain_y );
      dlen = terrain_x * terrain_y;
#if defined(ZDIAG) || defined(ZDIAG2)
      fprintf( stderr, "Allocating %d bytes for orgTerrain.\n", dlen );
#endif
    }
    for( i = 0; i < pkt->length; i++ ){
#ifdef ZDIAG2
      if( !(i%10) ){
        fprintf( stderr, "Params: %d, %d\n", ((curseq-1)<<7)+i, i );
      }
#endif
      gzipTerrain[((curseq-1)<<7)+i] = pkt->terrain_type[i];
    }
    totbytes += pkt->length;
    if( curseq == pkt->total_pkts ){
#if defined(ZDIAG) || defined(ZDIAG2)
      status = uncompress( orgTerrain, &dlen, gzipTerrain, totbytes );
      if( status != Z_OK ){
        if( status == Z_BUF_ERROR ){
          fprintf( stderr, "Unable to uncompress -- Z_BUF_ERROR.\n" );
        }
        if( status == Z_MEM_ERROR ){
          fprintf( stderr, "Unable to uncompress -- Z_MEM_ERROR.\n" );
        }
        if( status = Z_DATA_ERROR ){
          fprintf( stderr, "Unable to uncompress -- Z_DATA_ERROR!\n" );
        }
      }
      else{
        fprintf( stderr, "Total zipped terrain received: %d bytes\n", totbytes );
      }
#else
      uncompress( orgTerrain, &dlen, gzipTerrain, totbytes );
#endif
      terrainInfo = (struct t_unit *)malloc( dlen * sizeof( struct t_unit ) );
      for( i = 0; i < dlen; i++ ){
        terrainInfo[i].types = orgTerrain[i];
#ifdef ZDIAG2
	sum |= orgTerrain[i];
        if( orgTerrain[i] != 0 ){
          numnz++;
        }
#endif
      }
      done = received_terrain_info = TERRAIN_DONE;
#ifdef ZDIAG2
      fprintf( stderr, "Sum = %d, numnz = %d\n", sum, numnz );
#endif
    }
}    
#endif /* ASTEROIDS */

static void
handleTempPack(packet)		/* SP_SHIP_CAP */
    struct obvious_packet *packet;
{
    struct obvious_packet reply;
    /* printf("New MOTD info available\n"); */
    erase_motd();
    reply.type = CP_ASK_MOTD;
    sendServerPacket((struct player_spacket *) & reply);
}

/* handlers for the extension1 packet */

int
compute_extension1_size(pkt)
    char   *pkt;
{
    if (pkt[0] != SP_PARADISE_EXT1)
	return -1;

    switch (pkt[1]) {
    case SP_PE1_MISSING_BITMAP:
	return sizeof(struct pe1_missing_bitmap_spacket);
    case SP_PE1_NUM_MISSILES:
	return sizeof(struct pe1_num_missiles_spacket);
    default:
	return -1;
    }
}

static void
handleExtension1(packet)
    struct paradiseext1_spacket *packet;
{
    switch (packet->subtype) {
    case SP_PE1_MISSING_BITMAP:
	{
	    struct pe1_missing_bitmap_spacket *pkt =
	    (struct pe1_missing_bitmap_spacket *) packet;

	    newMotdPic(ntohs(pkt->x),
		       ntohs(pkt->y),
		       ntohs(pkt->width),
		       ntohs(pkt->height),
		       0,
		       ntohs(pkt->page));
	}
	break;
    case SP_PE1_NUM_MISSILES:
	me->p_totmissiles = ntohs(((struct pe1_num_missiles_spacket *) packet)->num);
	/* printf("updated totmissiles to %d\n",me->p_totmissiles); */
	if (me->p_totmissiles < 0)
	    me->p_totmissiles = 0;	/* SB/WB have -1 */
	break;
    default:
	printf("unknown paradise extension packet 1 subtype = %d\n",
	       packet->subtype);
    }
}
