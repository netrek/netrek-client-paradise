
/* reserved.c

 * Kevin P. Smith   7/3/89
 */
#include "copyright2.h"
#include "defines.h"
#include <stdio.h>
#ifdef STDC_HEADERS
#include <stdlib.h>
#endif
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "config.h"
#include "defs.h"
#include "packets.h"

static void
makeReservedPacket(packet)
    struct reserved_spacket *packet;
{
    int     i;

    for (i = 0; i < 16; i++) {
	packet->data[i] = random() % 256;
    } packet->type = SP_RESERVED;
}

static void
encryptReservedPacket(spacket, cpacket, server, pno)
    struct reserved_spacket *spacket;
    struct reserved_cpacket *cpacket;
    char   *server;
    int     pno;
{

    memcpy(cpacket->data, spacket->data, 16);
    memcpy(cpacket->resp, spacket->data, 16);
    cpacket->type = CP_RESERVED;

    /*
       Encryption algorithm goes here. Take the 16 bytes in cpacket->data,
       and create cpacket->resp, which you require the client to also do.  If
       he fails, he gets kicked out.
    */
}
