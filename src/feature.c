/*
 * Feature.c
 *
 * March, 1994.    Joe Rumsey, Tedd Hadley
 *
 * most of the functions needed to handle SP_FEATURE/CP_FEATURE
 * packets.  fill in the features list below for your client, and
 * add a call to reportFeatures just before the RSA response is sent.
 * handleFeature should just call checkFeature, which will search the
 * list and set the appropriate variable.  features unknown to the
 * server are set to the desired value for client features, and off
 * for server/client features.
 *
 * feature packets look like:
struct feature_cpacket {
   char                 type;
   char                 feature_type;
   char                 arg1,
                        arg2;
   int                  value;
   char                 name[80];
};

 *  type is CP_FEATURE, which is 60.  feature_spacket is identical.
 */
/*#define FEAT_DEBUG*/


#include "config.h"
#include "defines.h"
#ifdef FEATURE
#include "copyright.h"

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"
#include "packets.h"


void    sendFeature P((char *name, int feature_type,
		       int value, int arg1, int arg2));
void    reportFeatures P((void));
void    sendServerPacket P((struct player_spacket * packet));

/* not the actual packets: this holds a list of features to report for */
/* this client. */
struct feature {
    char   *name;
    int    *var;		/* holds allowed/enabled status */
    char    feature_type;	/* 'S' or 'C' for server/client */
    int     value;		/* desired status */
    char   *arg1, *arg2;	/* where to copy args, if non-null */
} features[] = {
    /*
       also sent seperately, put here for checking later. should be ok that
       it's sent twice.
    */
    {"FEATURE_PACKETS", &F_feature_packets, 'S', 1, 0, 0},

    {"VIEW_BOX", &allowViewBox, 'C', 1, 0, 0},
    {"SHOW_ALL_TRACTORS", &allowShowAllTractorPressor, 'S', 1, 0, 0},
    {"SHOW_NUM_ARMIES_ON_LOCAL", &allowShowArmiesOnLocal, 'C', 1, 0, 0},
#ifdef CONTINUOUS_MOUSE
    {"CONTINUOUS_MOUSE", &allowContinuousMouse, 'C', 1, 0, 0},
#endif
    {"NEWMACRO", &F_UseNewMacro, 'C', 1, 0, 0},
    /* {"SMARTMACRO",&UseSmartMacro, 'C', 1, 0, 0}, */
    {"MULTIMACROS", &F_multiline_enabled, 'S', 1, 0, 0},
    {"WHY_DEAD", &why_dead, 'S', 1, 0, 0},
    {"CLOAK_MAXWARP", &cloakerMaxWarp, 'S', 1, 0, 0},
/*{"DEAD_WARP", &F_dead_warp, 'S', 1, 0, 0},*/
#ifdef RC_DISTRESS
    {"RC_DISTRESS", &F_gen_distress, 'S', 1, 0, 0},
#ifdef BEEPLITE
    {"BEEPLITE", &F_allow_beeplite, 'C', 1, (char*)&F_beeplite_flags, 0},
#endif
#endif
#ifdef ASTEROIDS
/* terrain features */
    {"TERRAIN", &F_terrain, 'S', 1, (char*)&F_terrain_major, (char*)&F_terrain_minor},
/* Gzipped MOTD */
    {"GZ_MOTD", &F_gz_motd, 'S', 1, (char*)&F_gz_motd_major, (char*)&F_gz_motd_minor},
#endif /* ASTEROIDS */
    {0, 0, 0, 0, 0, 0}
};

static void
checkFeature(packet)
    struct feature_spacket *packet;
{
    int     i;
    int     value = ntohl((unsigned)packet->value);

#ifdef FEAT_DEBUG
    if (packet->type != SP_FEATURE) {
	printf("Packet type %d sent to checkFeature!\n", packet->type);
	return;
    }
#endif
    printf("%s: %s(%d)\n", &packet->name[0],
	   ((value == 1) ? "ON" : (value == 0) ? "OFF" : "UNKNOWN"),
	   value);

    for (i = 0; features[i].name != 0; i++) {
	if (strcmpi(packet->name, features[i].name) == 0) {
	    /*
	       if server returns unknown, set to off for server mods, desired
	       value for client mods. Otherwise,  set to value from server.
	    */
	    *features[i].var = (value == -1 ?
		 (features[i].feature_type == 'S' ? 0 : features[i].value) :
				value);
	    if (features[i].arg1)
		*features[i].arg1 = packet->arg1;
	    if (features[i].arg2)
		*features[i].arg2 = packet->arg2;
	    break;
	}
    }
    if (features[i].name == 0) {
	printf("Feature %s from server unknown to client!\n", packet->name);
    }
/* special cases: */
    if ((strcmpi(packet->name, "FEATURE_PACKETS")==0))
	reportFeatures();
    else if ((strcmpi(packet->name, "RC_DISTRESS") == 0) && F_gen_distress)
	distmacro = dist_prefered;
    else if ((strcmpi(packet->name, "BEEPLITE") == 0)) {
	switch (value) {
	case -1:		/* Unknown, we can use all of the features! */
	    F_beeplite_flags = LITE_PLAYERS_MAP |
		LITE_PLAYERS_LOCAL |
		LITE_SELF |
		LITE_PLANETS |
		LITE_SOUNDS |
		LITE_COLOR;
	    break;
	case 1:
	    if (F_beeplite_flags == 0) {	/* Server says we can have
						   beeplite, but no
						   options??? must be
						   configured wrong. */
		F_beeplite_flags = LITE_PLAYERS_MAP |
		    LITE_PLAYERS_LOCAL |
		    LITE_SELF |
		    LITE_PLANETS |
		    LITE_SOUNDS |
		    LITE_COLOR;
	    }
	    if (!(F_beeplite_flags & LITE_PLAYERS_MAP))
		printf("  LITE_PLAYERS_MAP   disabled\n");
	    if (!(F_beeplite_flags & LITE_PLAYERS_LOCAL))
		printf("  LITE_PLAYERS_LOCAL disabled\n");
	    if (!(F_beeplite_flags & LITE_SELF))
		printf("  LITE_SELF          disabled\n");
	    if (!(F_beeplite_flags & LITE_PLANETS))
		printf("  LITE_PLANETS       disabled\n");
	    if (!(F_beeplite_flags & LITE_SOUNDS))
		printf("  LITE_SOUNDS        disabled\n");
	    if (!(F_beeplite_flags & LITE_COLOR))
		printf("  LITE_COLOR         disabled\n");
	    break;
	default:
	    break;
	}
    }
}

/* call this from handleReserved */
void
reportFeatures()
{
    struct feature *f;

    /* 
       Don't send features[0], which is the feature for FEATURE_PACKETS itself.
       That's sent from main()
     */
    for (f = &features[1]; f->name != 0; f++) {
	sendFeature(f->name,
		    f->feature_type,
		    f->value,
		    (f->arg1 ? *f->arg1 : 0),
		    (f->arg2 ? *f->arg2 : 0));
#ifdef FEAT_DEBUG
	printf("(C->S) %s (%c): %d\n", f->name, f->feature_type, f->value);
#endif
    }
}

void
sendFeature(name, feature_type, value, arg1, arg2)
    char   *name;
    int     feature_type;
    int     value;
    int     arg1, arg2;
{
    struct feature_cpacket packet;

#ifdef FEAT_DEBUG
    printf( "Sending feature %s (value = %d)\n", name, value );
#endif
    strncpy(packet.name, name, sizeof(packet.name));
    packet.type = CP_FEATURE;
    packet.name[sizeof(packet.name) - 1] = 0;
    packet.feature_type = feature_type;
    packet.value = htonl((unsigned)value);
    packet.arg1 = arg1;
    packet.arg2 = arg2;
    sendServerPacket((struct player_spacket *) & packet);
}

#ifndef __CEXTRACT__
void
handleFeature(packet)
    struct feature_spacket *packet;
{
#ifdef FEAT_DEBUG
    printf( "Handling Feature packet\n" );
#endif
    checkFeature(packet);
}
#endif /*__CEXTRACT__*/
#endif				/* FEATURE */
