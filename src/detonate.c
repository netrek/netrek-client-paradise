/*
 * detonate.c
 */
#include "copyright.h"

#include <stdio.h>
#include <sys/types.h>
#include "config.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

/* Detonate torp */

void
detmine()
{
    if (paradise) {
	sendDetMineReq(-1);
    } else {
	register int i;

	for (i = 0; i < ntorps; i++) {
	    int     j = i + me->p_no * ntorps;
	    if (torps[j].t_status == TMOVE ||
		torps[j].t_status == TSTRAIGHT) {
		sendDetMineReq(j);
	    }
	}
    }
}
