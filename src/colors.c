/* colors.c
 *
 * Kevin P. Smith  6/11/89
 */
#include "defines.h"
#include "Wlib.h"
#include <stdio.h>
#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#include "config.h"
#include "copyright2.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

void
getColorDefs()
{
    borderColor = W_Grey;
    backColor = W_Black;
    foreColor = W_White;
    textColor = W_White;
    shipCol[0] = W_Grey;
    shipCol[1] = W_Yellow;
    shipCol[2] = W_Red;
    shipCol[3] = W_Green;
    shipCol[4] = W_Cyan;
    shipCol[5] = W_White;
    warningColor = W_Red;
    unColor = W_Grey;
    rColor = W_Red;
    yColor = W_Yellow;
    gColor = W_Green;
    myColor = W_White;
}
