
#include "defines.h"
#include <stdio.h>
#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#include <sys/types.h>
#include <netinet/in.h>
#ifdef HAVE_TIME_H
#include <time.h>
#if defined(HAVE_SYS_TIME_H) && defined(TIME_WITH_SYS_TIME)
#include <sys/time.h>
#endif
#else
#include <sys/time.h>
#endif
#ifdef STDC_HEADERS
#include <stdlib.h>
#endif

#include "config.h"
#include "gameconf.h"
#include "data.h"
#include "gppackets.h"
#include "images.h"
#include "strfuncs.h"

/* These stupid danged system prototypes are starting to irritate me */
#if 0
extern char *strdup();
#endif

int     number_of_teams;

struct teaminfo_s *teaminfo = 0;

void
load_default_teams()
{
    number_of_teams = 4;

    /* independent is teaminfo[-1], allteam is teaminfo[4] */
    teaminfo = 1 + (struct teaminfo_s *) malloc(sizeof(*teaminfo) * (number_of_teams + 2));

    strcpy(teaminfo[-1].name, "Independant");
    teaminfo[-1].letter = 'I';
    strcpy(teaminfo[-1].shortname, "IND");

    strcpy(teaminfo[0].name, "Federation");
    teaminfo[0].letter = 'F';
    strcpy(teaminfo[0].shortname, "FED");


    strcpy(teaminfo[1].name, "Romulan");
    teaminfo[1].letter = 'R';
    strcpy(teaminfo[1].shortname, "ROM");


    strcpy(teaminfo[2].name, "Klingon");
    teaminfo[2].letter = 'K';
    strcpy(teaminfo[2].shortname, "KLI");


    strcpy(teaminfo[3].name, "Orion");
    teaminfo[3].letter = 'O';
    strcpy(teaminfo[3].shortname, "ORI");

    strcpy(teaminfo[4].name, "All");
    teaminfo[4].letter = '-';
    strcpy(teaminfo[4].shortname, "ALL");
}

void
load_default_teamlogos()
{
   switch(number_of_teams) {
     case 4:
       teaminfo[3].shield_logo = getImage(I_ORISHIELD);
     case 3:
       teaminfo[2].shield_logo = getImage(I_KLISHIELD);
     case 2:
       teaminfo[1].shield_logo = getImage(I_ROMSHIELD);
     case 1:
       teaminfo[0].shield_logo = getImage(I_FEDSHIELD);
   }
}

void
load_generic_teams()
{
/* needs to be converted to xpm */
    /*
       reserved letters: A G T
    */
    int     i;

    /* independent is teaminfo[-1] */
    teaminfo = 1 + (struct teaminfo_s *) malloc(sizeof(*teaminfo) * (number_of_teams + 2));

    strcpy(teaminfo[-1].name, "Independant");
    teaminfo[-1].letter = 'I';
    strcpy(teaminfo[-1].shortname, "IND");

    load_default_teamlogos();	/* loads the first 4 team logos */

    for (i = 0; i < number_of_teams; i++) {
	sprintf(teaminfo[i].name, "Team #%d", i);
	teaminfo[i].letter = 'J' + i;	/* I, J through P */
	sprintf(teaminfo[i].shortname, "T%02d", i);

	/* we should draw something nifty here */
	if (i >= 4)		/* the first 4 have been loaded already. */
	    teaminfo[i].shield_logo = W_BitmapToImage(1, 1, (char*)&i);
	/* XXX uh-oh if more than 4 teams */
    }
}

void
initialize_thingies()
{
    int     i;
    int     n = (npthingies * nplayers + ngthingies);
    thingies = (struct thingy *) malloc(sizeof(*thingies) * n);
    for (i = 0; i < n; i++) {
	thingies[i].t_shape = SHP_BLANK;
	thingies[i].t_no = i;
	thingies[i].t_owner = (i >= npthingies * nplayers) ? -1 : (i / npthingies);
    }
}

void
initialize_players()
{
    int     i;

    players = (struct player *) malloc(sizeof(*players) * nplayers);

    bzero(players, (int)(sizeof(struct player) * nplayers));

    for (i = 0; i < nplayers; i++) {
	players[i].p_status = PFREE;
	players[i].p_cloakphase = 0;
	players[i].p_no = i;
	players[i].p_ntorp = 0;
	players[i].p_ndrone = 0;/* TSH */
	players[i].p_explode = 1;
	players[i].p_stats.st_tticks = 1;
	players[i].p_ship = shiptypes->ship;
    }
}

void
initialize_torps()
{
    int     i;

    torps = (struct torp *) malloc(sizeof(*torps) * nplayers * ntorps);

    for (i = 0; i < nplayers * ntorps; i++) {
	torps[i].t_status = TFREE;
	torps[i].t_no = i;
	torps[i].t_owner = (i / ntorps);
    }
}

void
initialize_plasmas()
{
    int     i;

    plasmatorps = (struct plasmatorp *) malloc(sizeof(*plasmatorps) * nplayers * nplasmas);

    for (i = 0; i < nplayers * nplasmas; i++) {
	plasmatorps[i].pt_status = PTFREE;
	plasmatorps[i].pt_no = i;
	plasmatorps[i].pt_owner = (i / nplasmas);
    }
}

static void
initialize_phasers()
{
    int     i;

    phasers = (struct phaser *) malloc(sizeof(*phasers) * nplayers * nphasers);

    for (i = 0; i < nplayers * nphasers; i++) {
	phasers[i].ph_status = PHFREE;
	phasers[i].ph_fuse = 0;
    }
}

void
initialize_planets()
{
    int     i;

    planets = (struct planet *) malloc(sizeof(*planets) * nplanets);

    for (i = 0; i < nplanets; i++) {
	struct planet *curr = &planets[i];
	curr->pl_no = i;
	curr->pl_flags = 0;
	curr->pl_owner = 0;
	curr->pl_x = curr->pl_y = -10000;
	sprintf(curr->pl_name, "planet%d", i);
	curr->pl_namelen = strlen(curr->pl_name);
	curr->pl_armies = 0;
	curr->pl_info = 0;
	curr->pl_deadtime = 0;
	curr->pl_couptime = 0;
	curr->pl_timestamp = 0;

	/* initialize planet redraw for moving planets */
	pl_update[i].plu_update = -1;
    }
}

void
init_ranks()
{
    ranks2 = (struct rank2 *) malloc(sizeof(*ranks2) * nranks2);
    ranks2[0].genocides = 0;
    ranks2[0].di = 0;
    ranks2[0].battle = 0.0;
    ranks2[0].strategy = 0.0;
    ranks2[0].specship = 0.0;
    ranks2[0].name = strdup("Recruit");

    ranks2[1].genocides = 1;
    ranks2[1].di = 10;
    ranks2[1].battle = 0.30;
    ranks2[1].strategy = 0.3;
    ranks2[1].specship = 0.0;
    ranks2[1].name = strdup("Specialist");

    ranks2[2].genocides = 2;
    ranks2[2].di = 25;
    ranks2[2].battle = 0.40;
    ranks2[2].strategy = 0.6;
    ranks2[2].specship = 0.0;
    ranks2[2].name = strdup("Cadet");

    ranks2[3].genocides = 3;
    ranks2[3].di = 45;
    ranks2[3].battle = 0.50;
    ranks2[3].strategy = 0.9;
    ranks2[3].specship = 0.0;
    ranks2[3].name = strdup("Midshipman");

    ranks2[4].genocides = 4;
    ranks2[4].di = 70;
    ranks2[4].battle = 0.70;
    ranks2[4].strategy = 1.2;
    ranks2[4].specship = 0.0;
    ranks2[4].name = strdup("Ensn. J.G.");

    ranks2[5].genocides = 5;
    ranks2[5].di = 100;
    ranks2[5].battle = 0.90;
    ranks2[5].strategy = 1.5;
    ranks2[5].specship = 0.0;
    ranks2[5].name = strdup("Ensign");

    ranks2[6].genocides = 6;
    ranks2[6].di = 140;
    ranks2[6].battle = 1.10;
    ranks2[6].strategy = 2.0;
    ranks2[6].specship = 0.0;
    ranks2[6].name = strdup("Lt. J.G.");

    ranks2[7].genocides = 8;
    ranks2[7].di = 190;
    ranks2[7].battle = 1.30;
    ranks2[7].strategy = 2.5;
    ranks2[7].specship = 0.0;
    ranks2[7].name = strdup("Lieutenant");

    ranks2[8].genocides = 10;
    ranks2[8].di = 250;
    ranks2[8].battle = 1.50;
    ranks2[8].strategy = 3.0;
    ranks2[8].specship = 0.5;
    ranks2[8].name = strdup("Lt. Cmdr.");

    ranks2[9].genocides = 15;
    ranks2[9].di = 300;
    ranks2[9].battle = 1.80;
    ranks2[9].strategy = 3.5;
    ranks2[9].specship = 0.7;
    ranks2[9].name = strdup("Commander");

    ranks2[10].genocides = 18;
    ranks2[10].di = 350;
    ranks2[10].battle = 2.00;
    ranks2[10].strategy = 4.0;
    ranks2[10].specship = 1.0;
    ranks2[10].name = strdup("Captain");

    ranks2[11].genocides = 25;
    ranks2[11].di = 400;
    ranks2[11].battle = 2.10;
    ranks2[11].strategy = 4.3;
    ranks2[11].specship = 2.5;
    ranks2[11].name = strdup("Fleet Capt.");

    ranks2[12].genocides = 50;
    ranks2[12].di = 500;
    ranks2[12].battle = 2.15;
    ranks2[12].strategy = 4.8;
    ranks2[12].specship = 3.0;
    ranks2[12].name = strdup("Commodore");

    ranks2[13].genocides = 75;
    ranks2[13].di = 700;
    ranks2[13].battle = 2.20;
    ranks2[13].strategy = 5.3;
    ranks2[13].specship = 3.3;
    ranks2[13].name = strdup("Moff");

    ranks2[14].genocides = 100;
    ranks2[14].di = 900;
    ranks2[14].battle = 2.25;
    ranks2[14].strategy = 5.7;
    ranks2[14].specship = 3.6;
    ranks2[14].name = strdup("Grand Moff");

    ranks2[15].genocides = 300;
    ranks2[15].di = 1200;
    ranks2[15].battle = 2.30;
    ranks2[15].strategy = 6.0;
    ranks2[15].specship = 3.8;
    ranks2[15].name = strdup("Rear Adml.");

    ranks2[16].genocides = 700;
    ranks2[16].di = 1700;
    ranks2[16].battle = 2.35;
    ranks2[16].strategy = 6.1;
    ranks2[16].specship = 4.0;
    ranks2[16].name = strdup("Admiral");

    ranks2[17].genocides = 1000;
    ranks2[17].di = 2500;
    ranks2[17].battle = 2.40;
    ranks2[17].strategy = 6.2;
    ranks2[17].specship = 4.2;
    ranks2[17].name = strdup("Grand Adml.");
}

void
init_royal()
{
    royal = (struct royalty *) malloc(sizeof(*royal) * nroyals);

    royal[0].name = strdup("none");
    royal[1].name = strdup("Wesley");
    royal[2].name = strdup("Centurion");
    royal[3].name = strdup("Praetor");
    royal[4].name = strdup("Emperor");
#if 0
    royal[5].name = strdup("Wizard");
    royal[6].name = strdup("Duke");
    royal[7].name = strdup("Count");
    royal[8].name = strdup("Baron");
    royal[9].name = strdup("Knight");
    royal[10].name = strdup("Dread");
    royal[11].name = strdup("Lord of Destruction");
    royal[12].name = strdup("BlitzMeister");
    royal[13].name = strdup("Hitler");
    royal[14].name = strdup("Terminator");
    royal[15].name = strdup("Kamikaze");
    royal[16].name = strdup("Speed Kamikaze");
    royal[17].name = strdup("Carpet Bomber");
    royal[18].name = strdup("NukeMeister");
    royal[19].name = strdup("Terrorist");
    royal[20].name = strdup("Democrat");
    royal[21].name = strdup("Executioner");
    royal[22].name = strdup("DooshMeister");
    royal[23].name = strdup("Diplomat");
    royal[24].name = strdup("speed Diplomat");
    royal[25].name = strdup("Addict");
#endif				/* 0 */
}

void
reinit_ranks()
{
    int     i;
    ranks2 = (struct rank2 *) malloc(sizeof(*ranks2) * nranks2);

    for (i = 0; i < nranks2; i++) {
	ranks2[i].name = strdup("blank");
    }
}

void
reinit_royal()
{
    int     i;
    royal = (struct royalty *) malloc(sizeof(*royal) * nroyals);

    for (i = 0; i < nroyals; i++) {
	royal[i].name = strdup("blank");
    }
}


void
resize_players()
{
    int     me_no = 0;

    if (me)
	me_no = me->p_no;
    players = (struct player *) realloc(players, sizeof(*players) * nplayers);
    if (me) {
	me = &players[me_no];
	myship = me->p_ship;
    }
}

void
free_teams()
{
    int     i;
    for (i = 0; i < number_of_teams; i++) {
	W_FreeImage(teaminfo[i].shield_logo);
    }
    /* we offsetted by 1 to make room for IND */
    free(teaminfo - 1);
    teaminfo = 0;
}

void
free_torps()
{
    free(torps);
    torps = 0;
}

void
free_phasers()
{
    free(phasers);
    phasers = 0;
}

void
free_plasmas()
{
    free(plasmatorps);
    plasmatorps = 0;
}

void
free_thingies()
{
    free(thingies);
    thingies = 0;
}

void
free_ranks()
{
    int     i;
    for (i = 0; i < nranks2; i++)
	if (ranks2[i].name)
	    free(ranks2[i].name);
    free(ranks2);
    ranks2 = 0;
}

void
free_royal()
{
    int     i;
    for (i = 0; i < nroyals; i++)
	if (royal[i].name)
	    free(royal[i].name);

    free(royal);
    royal = 0;
}

/*
 */

void
build_default_configuration()
{
    load_default_teams();
    /* can't load logos until we have some windows */

    init_shiptypes();
    initialize_players();
    initialize_torps();
    initialize_thingies();
    initialize_plasmas();
    initialize_planets();
    initialize_phasers();


    init_ranks();
    init_royal();
}

/*
 */


int
compute_gameparam_size(buf)
    char   *buf;
{
    struct gameparam_spacket *pkt = (struct gameparam_spacket *) buf;

    if (*buf != SP_GPARAM)
	return -1;

    switch (pkt->subtype) {
    case 0:
	return sizeof(struct gp_sizes_spacket);
    case 1:
	return sizeof(struct gp_team_spacket);
    case 2:
	return sizeof(struct gp_teamlogo_spacket);
    case 3:
	return sizeof(struct gp_shipshape_spacket);
    case 4:
	return sizeof(struct gp_shipbitmap_spacket);
    case 5:
	return sizeof(struct gp_rank_spacket);
    case 6:
	return sizeof(struct gp_royal_spacket);
    case 7:
	return sizeof(struct gp_teamplanet_spacket);
    default:
	return -1;
    }
}

static void
handleGPsizes(pkt)
    struct gp_sizes_spacket *pkt;
{
    free_ranks();
    free_royal();

    free_teams();
    free_torps();
    free_phasers();
    free_plasmas();
    free_thingies();

    nplayers = pkt->nplayers;
    number_of_teams = pkt->nteams;
    /* shiptypes */
    nranks2 = pkt->nranks;
    nroyals = pkt->nroyal;
    nphasers = pkt->nphasers;
    ntorps = pkt->ntorps;
    nplasmas = pkt->nplasmas;
    npthingies = pkt->nthingies;
    ngthingies = pkt->gthingies;

    /* gwidth */
    /* flags */

    load_generic_teams();

    reinit_ranks();
    reinit_royal();

    resize_players();
    initialize_torps();
    initialize_phasers();
    initialize_plasmas();
    initialize_thingies();
}

static void
handleGPteam(pkt)
    struct gp_team_spacket *pkt;
{
    struct teaminfo_s *currteam;

    if ((int) pkt->index >= number_of_teams) {
	fprintf(stderr, "Team #%d %s is out of range (0..%d)\n", pkt->index,
		pkt->teamname, number_of_teams);
	return;
    }
    currteam = &teaminfo[pkt->index];

    currteam->letter = pkt->letter;

    strncpy(currteam->shortname, pkt->shortname, 3);
    currteam->shortname[3] = 0;

    strncpy(currteam->name, pkt->teamname, sizeof(currteam->name) - 1);
    currteam->name[sizeof(currteam->name) - 1] = 0;;
}

static void
handleGPteamlogo(pkt)
    struct gp_teamlogo_spacket *pkt;
{
    static char buf[13 * 99];	/* 99x99 */
    static int curr_height = 0;
    static int lwidth, lheight;
    static int teamindex;
    int     pwidth;

    if ((unsigned) pkt->teamindex >= number_of_teams) {
	fprintf(stderr, "Team #%d is out of range (0..%d)\n", pkt->teamindex,
		number_of_teams);
	return;
    }
    if (pkt->y != curr_height) {
	fprintf(stderr, "Bad gp_teamlogo packet sequence y(%d) != curr_height(%d)\n",
		pkt->y, curr_height);
	curr_height = 0;
	return;
    }
    if (curr_height) {
	if (lwidth != pkt->logowidth || lheight != pkt->logoheight ||
	    teamindex != pkt->teamindex) {
	    fprintf(stderr, "gp_teamlogo packet sequence error, %d!=%d || %d!=%d || %d!=%d\n",
		    lwidth, pkt->logowidth, lheight, pkt->logoheight,
		    teamindex, pkt->teamindex);
	    curr_height = 0;
	    return;
	}
    } else {
	teamindex = pkt->teamindex;
	lwidth = pkt->logowidth;
	lheight = pkt->logoheight;
	if (lwidth > 99 || lheight > 99) {
	    fprintf(stderr, "logo too big (%dx%d), rejecting\n", lwidth, lheight);
	    curr_height = 0;
	    return;
	}
    }
    pwidth = (lwidth - 1) / 8 + 1;
    memcpy(buf + pwidth * curr_height, pkt->data, 
           (unsigned int)(pwidth * pkt->thisheight));
    curr_height += pkt->thisheight;

    if (curr_height >= lheight) {
	W_FreeImage(teaminfo[teamindex].shield_logo);
	teaminfo[teamindex].shield_logo = 
	                                W_BitmapToImage(lwidth, lheight, buf);
	curr_height = 0;
    }
}

static void
handleGPshipshape(pkt)
    struct gp_shipshape_spacket *pkt;
{
    if (pkt->race < -1 || pkt->race >= number_of_teams) {
	fprintf(stderr, "race #%d out of range (-1..%d)\n", pkt->race,
		number_of_teams - 1);
	return;
    }
    if ( /* pkt->shipno<0 || */ (int) pkt->shipno >= nshiptypes) {
	fprintf(stderr, "ship class #%d out of range (0..%d)\n", pkt->shipno,
		nshiptypes - 1);
	return;
    }
#ifdef FIXME
    replace_shipshape(pkt->race, pkt->shipno, pkt->nviews,
		      pkt->width, pkt->height);
#endif
}

static void
handleGPshipbitmap(pkt)
    struct gp_shipbitmap_spacket *pkt;
{
    if (pkt->race < -1 || pkt->race >= number_of_teams) {
	fprintf(stderr, "race #%d out of range (-1..%d)\n", pkt->race,
		number_of_teams - 1);
	return;
    }
    if ( /* pkt->shipno<0 || */ (int) pkt->shipno >= nshiptypes) {
	fprintf(stderr, "ship class #%d out of range (0..%d)\n", pkt->shipno,
		nshiptypes - 1);
	return;
    }
#ifdef FIXME
    replace_ship_bitmap(pkt->race, pkt->shipno, pkt->thisview, pkt->bitmapdata);
#endif
}

static void
handleGPrank(pkt)
    struct gp_rank_spacket *pkt;
{
    struct rank2 *curr;
    if (pkt->rankn >= nranks2) {
	fprintf(stderr, "rank #%d %s out of range (0..%d)\n", pkt->rankn,
		pkt->name, nranks2 - 1);
	return;
    }
    curr = &ranks2[pkt->rankn];
    free(curr->name);

    curr->genocides = htonl((unsigned)pkt->genocides);
    curr->di = htonl((unsigned)pkt->milliDI) / 1000.0;
    curr->battle = htonl((unsigned)pkt->millibattle) / 1000.0;
    curr->strategy = htonl((unsigned)pkt->millistrat) / 1000.0;
    curr->specship = htonl((unsigned)pkt->millispec) / 1000.0;
    curr->name = strdup(pkt->name);
}

static void
handleGProyal(pkt)
    struct gp_royal_spacket *pkt;
{
    if ((int) pkt->rankn >= nroyals) {
	fprintf(stderr, "Royalty #%d %s out of range (0..%d)\n", pkt->rankn,
		pkt->name, nroyals - 1);
	return;
    }
    free(royal[pkt->rankn].name);
    royal[pkt->rankn].name = strdup(pkt->name);
}

#ifdef FIXME
static unsigned char mplanet_bits[] = {
    0xe0, 0x03, 0x18, 0x0c, 0x04, 0x10, 0x02, 0x20, 0x02, 0x20, 0x01, 0x40,
    0x01, 0x40, 0x01, 0x40, 0x01, 0x40, 0x01, 0x40, 0x02, 0x20, 0x02, 0x20,
0x04, 0x10, 0x18, 0x0c, 0xe0, 0x03, 0x00, 0x00};
static unsigned char planet_bits[] = {
    0x00, 0xf8, 0x03, 0x00, 0x00, 0x07, 0x1c, 0x00, 0xc0, 0x00, 0x60, 0x00,
    0x20, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x01, 0x08, 0x00, 0x00, 0x02,
    0x04, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x04, 0x02, 0x00, 0x00, 0x08,
    0x02, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x08, 0x01, 0x00, 0x00, 0x10,
    0x01, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x10,
    0x01, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x10,
    0x02, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x08,
    0x04, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x02,
    0x10, 0x00, 0x00, 0x01, 0x20, 0x00, 0x80, 0x00, 0xc0, 0x00, 0x60, 0x00,
0x00, 0x07, 0x1c, 0x00, 0x00, 0xf8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif /*FIXME*/

static void
handleGPteamplanet(pkt)
    struct gp_teamplanet_spacket *pkt;
{
#ifdef FIXME
    {
#define	TACTICALSIZE	sizeof(pkt->tactical)
	unsigned char tactical[TACTICALSIZE];
	int     i;
	int     race = pkt->teamn;

	if (race < -1 || race >= number_of_teams) {
	    fprintf(stderr, "race #%d out of range (-1..%d)\n", race,
		    number_of_teams - 1);
	    return;
	}
	for (i = 0; i < TACTICALSIZE; i++) {
	    tactical[i] = (pkt->tactical[i] & pkt->tacticalM[i]) |
		(planet_bits[i] & ~pkt->tacticalM[i]);
	}

	W_FreeImage(bplanets[race + 1]);
	bplanets[race + 1] = W_BitmapToImage(30, 30, tactical);

#undef TACTICALSIZE
    }

    {
#define	GALACTICSIZE	sizeof(pkt->galactic)
	unsigned char galactic[GALACTICSIZE];
	int     i;
	int     race = pkt->teamn;

	if (race < -1 || race >= number_of_teams) {
	    fprintf(stderr, "race #%d out of range (-1..%d)\n", race,
		    number_of_teams - 1);
	    return;
	}
	for (i = 0; i < GALACTICSIZE; i++) {
	    galactic[i] = (pkt->galactic[i] & pkt->galacticM[i]) |
		(mplanet_bits[i] & ~pkt->galacticM[i]);
	}

	W_FreeImage(mbplanets[race + 1]);
	mbplanets[race + 1] = W_BitmapToImage(16, 16, galactic);

#undef GALACTICSIZE
    }
#endif /* FIXME */
}

#ifndef __CEXTRACT__
void
handleGameparams(pkt)
    struct gameparam_spacket *pkt;
{
    switch (pkt->subtype) {
    case 0:
	handleGPsizes((struct gp_sizes_spacket *) pkt);
	break;
    case 1:
	handleGPteam((struct gp_team_spacket *) pkt);
	break;
    case 2:
	handleGPteamlogo((struct gp_teamlogo_spacket *) pkt);
	break;
    case 3:
	handleGPshipshape((struct gp_shipshape_spacket *) pkt);
	break;
    case 4:
	handleGPshipbitmap((struct gp_shipbitmap_spacket *) pkt);
	break;
    case 5:
	handleGPrank((struct gp_rank_spacket *) pkt);
	break;
    case 6:
	handleGProyal((struct gp_royal_spacket *) pkt);
	break;
    case 7:
	handleGPteamplanet((struct gp_teamplanet_spacket *) pkt);
	break;
    default:
	fprintf(stderr, "Gameparams packet subtype %d not yet implemented\n",
		pkt->subtype);
    }
}
#endif /*__CEXTRACT__*/
