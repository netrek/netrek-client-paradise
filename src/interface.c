/*
 * interface.c
 *
 * This file will include all the interfaces between the input routines
 *  and the daemon.  They should be useful for writing robots and the
 *  like
 */
#include "copyright.h"
#include "defines.h"

#include <stdio.h>
#include <sys/types.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#ifdef HAVE_SYS_TIMEB_H
#include <sys/timeb.h>
#endif
#include <signal.h>
#include <math.h>
#include "config.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "packets.h"
#include "proto.h"

void
set_speed(speed)
    int     speed;
{
    /* don't repeat useless commands [BDyess] */
    if (me->p_desspeed != speed || me->p_speed != speed)
	sendSpeedReq(speed);
    me->p_desspeed = speed;
}

void
set_course(dir)
    unsigned int dir;
{
    /* don't repeat commands [BDyess] */
    if (me->p_desdir != dir || me->p_dir != dir)
	sendDirReq((int)dir);
    me->p_desdir = dir;
}

void
shield_up()
{
    if (!(me->p_flags & PFSHIELD)) {
	sendShieldReq(1);
    }
}

void
shield_down()
{
    if (me->p_flags & PFSHIELD) {
	sendShieldReq(0);
    }
}

void
shield_tog()
{
    if (me->p_flags & PFSHIELD) {
	sendShieldReq(0);
    } else {
	sendShieldReq(1);
    }
}

void
bomb_planet()
{
    if (!(me->p_flags & PFBOMB)) {
	sendBombReq(1);
    }
}

void
beam_up()
{
    if (!(me->p_flags & PFBEAMUP)) {
	sendBeamReq(1);		/* 1 means up... */
    }
}

void
beam_down()
{
    if (!(me->p_flags & PFBEAMDOWN)) {
	sendBeamReq(2);		/* 2 means down... */
    }
}

void
cloak()
{
    if (me->p_flags & PFCLOAK) {
	sendCloakReq(0);
    } else {
	sendCloakReq(1);
    }
}

int
mtime()
{
#if 0
    struct timeb tm;

    ftime(&tm);
    /* mask off 16 high bits and add in milliseconds */
    return (tm.time & 0x0000ffff) * 1000 + tm.millitm;
#else
    struct timeval tv;

    gettimeofday(&tv, (struct timezone *) 0);
    return (tv.tv_sec & 0x0ffff) * 1000 + tv.tv_usec / 1000;
#endif
}
