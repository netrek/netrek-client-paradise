/* images.c.  Contains the image control data. [BDyess] */
/* automatically created by scripts/mkimgsrc */

#include "config.h"
#include <stdlib.h>
#include "Wlib.h"
#include "images.h"
#include "struct.h"
#include "proto.h"

/* convert between ship number to image offset.  For example, to get the
   image for a scout, use: imagearray[shipoffset[SCOUT]+I_<team>_AS];
   [BDyess] */
static int shipImageOffset[] = { 
  I_FED_SC - I_FED_AS,
  I_FED_DD - I_FED_AS,
  I_FED_CA - I_FED_AS,
  I_FED_BB - I_FED_AS,
  I_FED_AS - I_FED_AS,
  I_FED_SB - I_FED_AS,
  I_FED_AT - I_FED_AS,
  I_FED_JS - I_FED_AS,
  I_FED_FR - I_FED_AS,
  I_FED_WB - I_FED_AS,
  I_FED_CL - I_FED_AS,
  I_FED_CV - I_FED_AS,
  I_FED_UT - I_FED_AS,
  I_FED_PT - I_FED_AS,
  I_FED_PU - I_FED_AS,
};

static int teamImageOffset[] = {
  I_IND_AS,
  I_FED_AS,
  I_ROM_AS,
  I_KLI_AS,
  I_ORI_AS,
  I_IND_AS,
};

/* compiled in images in alphabetical order, XBM before XPM. [BDyess] */
static unsigned char default_bits[] = {
  0x00, 0xf8, 0x03, 0x00, 0x00, 0x07, 0x1c, 0x00, 0x80, 0x00, 0x20, 0x00,
  0x60, 0x00, 0xc0, 0x00, 0x10, 0x00, 0x00, 0x01, 0x08, 0x00, 0x00, 0x02,
  0x08, 0x00, 0x00, 0x02, 0x04, 0x00, 0x00, 0x04, 0x02, 0x00, 0x00, 0x08,
  0x02, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x08, 0x01, 0x00, 0x00, 0x10,
  0x01, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x10,
  0x01, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x10,
  0x01, 0x00, 0x00, 0x10, 0x02, 0x00, 0x00, 0x08, 0x02, 0x00, 0x00, 0x08,
  0x02, 0x00, 0x00, 0x08, 0x04, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x02,
  0x08, 0x00, 0x00, 0x02, 0x10, 0x00, 0x00, 0x01, 0x60, 0x00, 0xc0, 0x00,
  0x80, 0x00, 0x20, 0x00, 0x00, 0x1f, 0x1f, 0x00, 0x00, 0xf8, 0x03, 0x00,
  };

/* keep sorted (for bsearch) [BDyess] */
static W_Image imagearray[] = {
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/Fed.bronco.colored/AS", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 0 */
   {0, 0, 0, 0, "/Fed.bronco.colored/AT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 1 */
   {0, 0, 0, 0, "/Fed.bronco.colored/BB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 2 */
   {0, 0, 0, 0, "/Fed.bronco.colored/CA", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 3 */
   {0, 0, 0, 0, "/Fed.bronco.colored/DD", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 4 */
   {0, 0, 0, 0, "/Fed.bronco.colored/SB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 5 */
   {0, 0, 0, 0, "/Fed.bronco.colored/SC", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 6 */
   {0, 0, 0, 0, "/Fed.bronco/AS", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 7 */
   {0, 0, 0, 0, "/Fed.bronco/AT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 8 */
   {0, 0, 0, 0, "/Fed.bronco/BB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 9 */
   {0, 0, 0, 0, "/Fed.bronco/CA", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 10 */
   {0, 0, 0, 0, "/Fed.bronco/DD", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 11 */
   {0, 0, 0, 0, "/Fed.bronco/SB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 12 */
   {0, 0, 0, 0, "/Fed.bronco/SC", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 13 */
   {0, 0, 0, 0, "/Fed.colored/AS", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 14 */
   {0, 0, 0, 0, "/Fed.colored/AT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 15 */
   {0, 0, 0, 0, "/Fed.colored/BB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 16 */
   {0, 0, 0, 0, "/Fed.colored/CA", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 17 */
   {0, 0, 0, 0, "/Fed.colored/CL", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 18 */
   {0, 0, 0, 0, "/Fed.colored/CV", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 19 */
   {0, 0, 0, 0, "/Fed.colored/DD", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 20 */
   {0, 0, 0, 0, "/Fed.colored/FR", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 21 */
   {0, 0, 0, 0, "/Fed.colored/JS", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 22 */
   {0, 0, 0, 0, "/Fed.colored/PT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 23 */
   {0, 0, 0, 0, "/Fed.colored/PU", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 24 */
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/Fed.colored/SB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 25 */
   {0, 0, 0, 0, "/Fed.colored/SC", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 26 */
   {0, 0, 0, 0, "/Fed.colored/UT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 27 */
   {0, 0, 0, 0, "/Fed.colored/WB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 28 */
   {0, 0, 0, 0, "/Fed/AS", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 29 */
   {0, 0, 0, 0, "/Fed/AT", 0, I_IND_AT, 0, 0, NULL, NULL, 0, 0}, /* 30 */
   {0, 0, 0, 0, "/Fed/BB", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 31 */
   {0, 0, 0, 0, "/Fed/CA", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 32 */
   {0, 0, 0, 0, "/Fed/CL", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 33 */
   {0, 0, 0, 0, "/Fed/CV", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 34 */
   {0, 0, 0, 0, "/Fed/DD", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 35 */
   {0, 0, 0, 0, "/Fed/FR", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 36 */
   {0, 0, 0, 0, "/Fed/JS", 0, I_FED_SB, 0, 0, NULL, NULL, 0, 0}, /* 37 */
   {0, 0, 0, 0, "/Fed/PT", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 38 */
   {0, 0, 0, 0, "/Fed/PU", 0, I_IND_PT, 0, 0, NULL, NULL, 0, 0}, /* 39 */
   {0, 0, 0, 0, "/Fed/SB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 40 */
   {0, 0, 0, 0, "/Fed/SC", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 41 */
   {0, 0, 0, 0, "/Fed/UT", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 42 */
   {0, 0, 0, 0, "/Fed/WB", 0, I_FED_SB, 0, 0, NULL, NULL, 0, 0}, /* 43 */
   {0, 0, 0, 0, "/Ind/AS", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 44 */
   {0, 0, 0, 0, "/Ind/AT", 0, I_FED_SB, 0, 0, NULL, NULL, 0, 0}, /* 45 */
   {0, 0, 0, 0, "/Ind/BB", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 46 */
   {0, 0, 0, 0, "/Ind/CA", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 47 */
   {0, 0, 0, 0, "/Ind/CL", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 48 */
   {0, 0, 0, 0, "/Ind/CV", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 49 */
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/Ind/DD", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 50 */
   {0, 0, 0, 0, "/Ind/FR", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 51 */
   {0, 0, 0, 0, "/Ind/JS", 0, I_FED_JS, 0, 0, NULL, NULL, 0, 0}, /* 52 */
   {0, 0, 0, 0, "/Ind/PT", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 53 */
   {0, 0, 0, 0, "/Ind/PU", 0, I_FED_SB, 0, 0, NULL, NULL, 0, 0}, /* 54 */
   {0, 0, 0, 0, "/Ind/SB", 0, I_FED_SB, 0, 0, NULL, NULL, 0, 0}, /* 55 */
   {0, 0, 0, 0, "/Ind/SC", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 56 */
   {0, 0, 0, 0, "/Ind/UT", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 57 */
   {0, 0, 0, 0, "/Ind/WB", 0, I_FED_SB, 0, 0, NULL, NULL, 0, 0}, /* 58 */
   {0, 0, 0, 0, "/Kli.bronco.colored/AS", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 59 */
   {0, 0, 0, 0, "/Kli.bronco.colored/AT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 60 */
   {0, 0, 0, 0, "/Kli.bronco.colored/BB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 61 */
   {0, 0, 0, 0, "/Kli.bronco.colored/CA", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 62 */
   {0, 0, 0, 0, "/Kli.bronco.colored/DD", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 63 */
   {0, 0, 0, 0, "/Kli.bronco.colored/SB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 64 */
   {0, 0, 0, 0, "/Kli.bronco.colored/SC", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 65 */
   {0, 0, 0, 0, "/Kli.bronco/AS", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 66 */
   {0, 0, 0, 0, "/Kli.bronco/AT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 67 */
   {0, 0, 0, 0, "/Kli.bronco/BB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 68 */
   {0, 0, 0, 0, "/Kli.bronco/CA", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 69 */
   {0, 0, 0, 0, "/Kli.bronco/DD", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 70 */
   {0, 0, 0, 0, "/Kli.bronco/SB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 71 */
   {0, 0, 0, 0, "/Kli.bronco/SC", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 72 */
   {0, 0, 0, 0, "/Kli.colored/AS", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 73 */
   {0, 0, 0, 0, "/Kli.colored/AT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 74 */
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/Kli.colored/BB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 75 */
   {0, 0, 0, 0, "/Kli.colored/CA", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 76 */
   {0, 0, 0, 0, "/Kli.colored/CL", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 77 */
   {0, 0, 0, 0, "/Kli.colored/CV", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 78 */
   {0, 0, 0, 0, "/Kli.colored/DD", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 79 */
   {0, 0, 0, 0, "/Kli.colored/FR", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 80 */
   {0, 0, 0, 0, "/Kli.colored/JS", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 81 */
   {0, 0, 0, 0, "/Kli.colored/PT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 82 */
   {0, 0, 0, 0, "/Kli.colored/PU", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 83 */
   {0, 0, 0, 0, "/Kli.colored/SB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 84 */
   {0, 0, 0, 0, "/Kli.colored/SC", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 85 */
   {0, 0, 0, 0, "/Kli.colored/UT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 86 */
   {0, 0, 0, 0, "/Kli.colored/WB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 87 */
   {0, 0, 0, 0, "/Kli/AS", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 88 */
   {0, 0, 0, 0, "/Kli/AT", 0, I_IND_AT, 0, 0, NULL, NULL, 0, 0}, /* 89 */
   {0, 0, 0, 0, "/Kli/BB", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 90 */
   {0, 0, 0, 0, "/Kli/CA", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 91 */
   {0, 0, 0, 0, "/Kli/CL", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 92 */
   {0, 0, 0, 0, "/Kli/CV", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 93 */
   {0, 0, 0, 0, "/Kli/DD", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 94 */
   {0, 0, 0, 0, "/Kli/FR", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 95 */
   {0, 0, 0, 0, "/Kli/JS", 0, I_FED_SB, 0, 0, NULL, NULL, 0, 0}, /* 96 */
   {0, 0, 0, 0, "/Kli/PT", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 97 */
   {0, 0, 0, 0, "/Kli/PU", 0, I_IND_PU, 0, 0, NULL, NULL, 0, 0}, /* 98 */
   {0, 0, 0, 0, "/Kli/SB", 0, I_FED_SB, 0, 0, NULL, NULL, 0, 0}, /* 99 */
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/Kli/SC", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 100 */
   {0, 0, 0, 0, "/Kli/UT", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 101 */
   {0, 0, 0, 0, "/Kli/WB", 0, I_FED_SB, 0, 0, NULL, NULL, 0, 0}, /* 102 */
   {0, 0, 0, 0, "/Ori.bronco.colored/AS", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 103 */
   {0, 0, 0, 0, "/Ori.bronco.colored/AT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 104 */
   {0, 0, 0, 0, "/Ori.bronco.colored/BB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 105 */
   {0, 0, 0, 0, "/Ori.bronco.colored/CA", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 106 */
   {0, 0, 0, 0, "/Ori.bronco.colored/DD", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 107 */
   {0, 0, 0, 0, "/Ori.bronco.colored/SB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 108 */
   {0, 0, 0, 0, "/Ori.bronco.colored/SC", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 109 */
   {0, 0, 0, 0, "/Ori.bronco/AS", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 110 */
   {0, 0, 0, 0, "/Ori.bronco/AT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 111 */
   {0, 0, 0, 0, "/Ori.bronco/BB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 112 */
   {0, 0, 0, 0, "/Ori.bronco/CA", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 113 */
   {0, 0, 0, 0, "/Ori.bronco/DD", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 114 */
   {0, 0, 0, 0, "/Ori.bronco/SB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 115 */
   {0, 0, 0, 0, "/Ori.bronco/SC", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 116 */
   {0, 0, 0, 0, "/Ori.colored/AS", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 117 */
   {0, 0, 0, 0, "/Ori.colored/AT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 118 */
   {0, 0, 0, 0, "/Ori.colored/BB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 119 */
   {0, 0, 0, 0, "/Ori.colored/CA", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 120 */
   {0, 0, 0, 0, "/Ori.colored/CL", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 121 */
   {0, 0, 0, 0, "/Ori.colored/CV", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 122 */
   {0, 0, 0, 0, "/Ori.colored/DD", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 123 */
   {0, 0, 0, 0, "/Ori.colored/FR", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 124 */
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/Ori.colored/JS", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 125 */
   {0, 0, 0, 0, "/Ori.colored/PT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 126 */
   {0, 0, 0, 0, "/Ori.colored/PU", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 127 */
   {0, 0, 0, 0, "/Ori.colored/SB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 128 */
   {0, 0, 0, 0, "/Ori.colored/SC", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 129 */
   {0, 0, 0, 0, "/Ori.colored/UT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 130 */
   {0, 0, 0, 0, "/Ori.colored/WB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 131 */
   {0, 0, 0, 0, "/Ori/AS", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 132 */
   {0, 0, 0, 0, "/Ori/AT", 0, I_IND_AT, 0, 0, NULL, NULL, 0, 0}, /* 133 */
   {0, 0, 0, 0, "/Ori/BB", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 134 */
   {0, 0, 0, 0, "/Ori/CA", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 135 */
   {0, 0, 0, 0, "/Ori/CL", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 136 */
   {0, 0, 0, 0, "/Ori/CV", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 137 */
   {0, 0, 0, 0, "/Ori/DD", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 138 */
   {0, 0, 0, 0, "/Ori/FR", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 139 */
   {0, 0, 0, 0, "/Ori/JS", 0, I_FED_SB, 0, 0, NULL, NULL, 0, 0}, /* 140 */
   {0, 0, 0, 0, "/Ori/PT", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 141 */
   {0, 0, 0, 0, "/Ori/PU", 0, I_IND_PU, 0, 0, NULL, NULL, 0, 0}, /* 142 */
   {0, 0, 0, 0, "/Ori/SB", 0, I_FED_SB, 0, 0, NULL, NULL, 0, 0}, /* 143 */
   {0, 0, 0, 0, "/Ori/SC", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 144 */
   {0, 0, 0, 0, "/Ori/UT", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 145 */
   {0, 0, 0, 0, "/Ori/WB", 0, I_FED_SB, 0, 0, NULL, NULL, 0, 0}, /* 146 */
   {0, 0, 0, 0, "/Rom.bronco.colored/AS", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 147 */
   {0, 0, 0, 0, "/Rom.bronco.colored/AT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 148 */
   {0, 0, 0, 0, "/Rom.bronco.colored/BB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 149 */
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/Rom.bronco.colored/CA", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 150 */
   {0, 0, 0, 0, "/Rom.bronco.colored/DD", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 151 */
   {0, 0, 0, 0, "/Rom.bronco.colored/SB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 152 */
   {0, 0, 0, 0, "/Rom.bronco.colored/SC", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 153 */
   {0, 0, 0, 0, "/Rom.bronco/AS", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 154 */
   {0, 0, 0, 0, "/Rom.bronco/AT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 155 */
   {0, 0, 0, 0, "/Rom.bronco/BB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 156 */
   {0, 0, 0, 0, "/Rom.bronco/CA", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 157 */
   {0, 0, 0, 0, "/Rom.bronco/DD", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 158 */
   {0, 0, 0, 0, "/Rom.bronco/SB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 159 */
   {0, 0, 0, 0, "/Rom.bronco/SC", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 160 */
   {0, 0, 0, 0, "/Rom.colored/AS", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 161 */
   {0, 0, 0, 0, "/Rom.colored/AT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 162 */
   {0, 0, 0, 0, "/Rom.colored/BB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 163 */
   {0, 0, 0, 0, "/Rom.colored/CA", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 164 */
   {0, 0, 0, 0, "/Rom.colored/CL", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 165 */
   {0, 0, 0, 0, "/Rom.colored/CV", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 166 */
   {0, 0, 0, 0, "/Rom.colored/DD", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 167 */
   {0, 0, 0, 0, "/Rom.colored/FR", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 168 */
   {0, 0, 0, 0, "/Rom.colored/JS", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 169 */
   {0, 0, 0, 0, "/Rom.colored/PT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 170 */
   {0, 0, 0, 0, "/Rom.colored/PU", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 171 */
   {0, 0, 0, 0, "/Rom.colored/SB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 172 */
   {0, 0, 0, 0, "/Rom.colored/SC", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 173 */
   {0, 0, 0, 0, "/Rom.colored/UT", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 174 */
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/Rom.colored/WB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 175 */
   {0, 0, 0, 0, "/Rom.starwars/AS", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 176 */
   {0, 0, 0, 0, "/Rom.starwars/BB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 177 */
   {0, 0, 0, 0, "/Rom.starwars/CA", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 178 */
   {0, 0, 0, 0, "/Rom.starwars/DD", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 179 */
   {0, 0, 0, 0, "/Rom.starwars/SB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 180 */
   {0, 0, 0, 0, "/Rom.starwars/SC", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 181 */
   {0, 0, 0, 0, "/Rom/AS", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 182 */
   {0, 0, 0, 0, "/Rom/AT", 0, I_IND_AT, 0, 0, NULL, NULL, 0, 0}, /* 183 */
   {0, 0, 0, 0, "/Rom/BB", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 184 */
   {0, 0, 0, 0, "/Rom/CA", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 185 */
   {0, 0, 0, 0, "/Rom/CL", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 186 */
   {0, 0, 0, 0, "/Rom/CV", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 187 */
   {0, 0, 0, 0, "/Rom/DD", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 188 */
   {0, 0, 0, 0, "/Rom/FR", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 189 */
   {0, 0, 0, 0, "/Rom/JS", 0, I_FED_SB, 0, 0, NULL, NULL, 0, 0}, /* 190 */
   {0, 0, 0, 0, "/Rom/PT", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 191 */
   {0, 0, 0, 0, "/Rom/PU", 0, I_IND_PU, 0, 0, NULL, NULL, 0, 0}, /* 192 */
   {0, 0, 0, 0, "/Rom/SB", 0, I_FED_SB, 0, 0, NULL, NULL, 0, 0}, /* 193 */
   {0, 0, 0, 0, "/Rom/SC", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 194 */
   {0, 0, 0, 0, "/Rom/UT", 0, I_FED_CA, 0, 0, NULL, NULL, 0, 0}, /* 195 */
   {0, 0, 0, 0, "/Rom/WB", 0, I_FED_SB, 0, 0, NULL, NULL, 0, 0}, /* 196 */
   {0, 0, 0, 0, "/cloak", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 197 */
   {0, 0, 0, 0, "/dashboard_bg", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 198 */
   {30, 30, 0, 0, "/default", 0, I_DEFAULT, 0, 1, default_bits, NULL, 0, 0}, /* 199 */
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/emph_planet_seq", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 200 */
   {0, 0, 0, 0, "/emph_player_seq", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 201 */
   {0, 0, 0, 0, "/emph_player_seql", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 202 */
   {0, 0, 0, 0, "/exp2", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 203 */
   {0, 0, 0, 0, "/explosion", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 204 */
   {0, 0, 0, 0, "/hull", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 205 */
   {0, 0, 0, 0, "/intro/clock", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 206 */
   {0, 0, 0, 0, "/intro/fedshield", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 207 */
   {0, 0, 0, 0, "/intro/header", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 208 */
   {0, 0, 0, 0, "/intro/header1", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 209 */
   {0, 0, 0, 0, "/intro/header2", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 210 */
   {0, 0, 0, 0, "/intro/header3", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 211 */
   {0, 0, 0, 0, "/intro/header4", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 212 */
   {0, 0, 0, 0, "/intro/headerA", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 213 */
   {0, 0, 0, 0, "/intro/headerB", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 214 */
   {0, 0, 0, 0, "/intro/icon", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 215 */
   {0, 0, 0, 0, "/intro/klishield", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 216 */
   {0, 0, 0, 0, "/intro/noentry", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 217 */
   {0, 0, 0, 0, "/intro/orishield", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 218 */
   {0, 0, 0, 0, "/intro/romshield", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 219 */
   {0, 0, 0, 0, "/intro/safe", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 220 */
   {0, 0, 0, 0, "/newstats/flag_alert", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 221 */
   {0, 0, 0, 0, "/newstats/flag_army", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 222 */
   {0, 0, 0, 0, "/newstats/flag_beaming", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 223 */
   {0, 0, 0, 0, "/newstats/flag_bomb", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 224 */
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/newstats/flag_cloak", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 225 */
   {0, 0, 0, 0, "/newstats/flag_down", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 226 */
   {0, 0, 0, 0, "/newstats/flag_etemp", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 227 */
   {0, 0, 0, 0, "/newstats/flag_fed", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 228 */
   {0, 0, 0, 0, "/newstats/flag_fighter", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 229 */
   {0, 0, 0, 0, "/newstats/flag_fuel", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 230 */
   {0, 0, 0, 0, "/newstats/flag_fuse", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 231 */
   {0, 0, 0, 0, "/newstats/flag_hull", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 232 */
   {0, 0, 0, 0, "/newstats/flag_kli", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 233 */
   {0, 0, 0, 0, "/newstats/flag_missile", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 234 */
   {0, 0, 0, 0, "/newstats/flag_ori", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 235 */
   {0, 0, 0, 0, "/newstats/flag_plasma", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 236 */
   {0, 0, 0, 0, "/newstats/flag_pressor", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 237 */
   {0, 0, 0, 0, "/newstats/flag_repair", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 238 */
   {0, 0, 0, 0, "/newstats/flag_rom", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 239 */
   {0, 0, 0, 0, "/newstats/flag_ship_fed", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 240 */
   {0, 0, 0, 0, "/newstats/flag_ship_kli", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 241 */
   {0, 0, 0, 0, "/newstats/flag_ship_ori", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 242 */
   {0, 0, 0, 0, "/newstats/flag_ship_rom", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 243 */
   {0, 0, 0, 0, "/newstats/flag_speed1", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 244 */
   {0, 0, 0, 0, "/newstats/flag_speed2", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 245 */
   {0, 0, 0, 0, "/newstats/flag_torp", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 246 */
   {0, 0, 0, 0, "/newstats/flag_tractor", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 247 */
   {0, 0, 0, 0, "/newstats/flag_up", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 248 */
   {0, 0, 0, 0, "/newstats/flag_war", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 249 */
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/newstats/flag_wtemp", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 250 */
   {0, 0, 0, 0, "/newstats/status_template", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 251 */
   {0, 0, 0, 0, "/oldexp", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 252 */
   {0, 0, 0, 0, "/planets/A/0", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 253 */
   {0, 0, 0, 0, "/planets/A/1", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 254 */
   {0, 0, 0, 0, "/planets/A/2", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 255 */
   {0, 0, 0, 0, "/planets/A/3", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 256 */
   {0, 0, 0, 0, "/planets/A/4", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 257 */
   {0, 0, 0, 0, "/planets/A/m0", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 258 */
   {0, 0, 0, 0, "/planets/A/m1", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 259 */
   {0, 0, 0, 0, "/planets/A/m2", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 260 */
   {0, 0, 0, 0, "/planets/A/m3", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 261 */
   {0, 0, 0, 0, "/planets/A/m4", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 262 */
   {0, 0, 0, 0, "/planets/A_overlay/0", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 263 */
   {0, 0, 0, 0, "/planets/A_overlay/1", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 264 */
   {0, 0, 0, 0, "/planets/A_overlay/2", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 265 */
   {0, 0, 0, 0, "/planets/A_overlay/3", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 266 */
   {0, 0, 0, 0, "/planets/A_overlay/4", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 267 */
   {0, 0, 0, 0, "/planets/A_overlay/m0", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 268 */
   {0, 0, 0, 0, "/planets/A_overlay/m1", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 269 */
   {0, 0, 0, 0, "/planets/A_overlay/m2", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 270 */
   {0, 0, 0, 0, "/planets/A_overlay/m3", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 271 */
   {0, 0, 0, 0, "/planets/A_overlay/m4", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 272 */
   {0, 0, 0, 0, "/planets/F/0000", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 273 */
   {0, 0, 0, 0, "/planets/F/0001", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 274 */
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/planets/F/0010", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 275 */
   {0, 0, 0, 0, "/planets/F/0011", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 276 */
   {0, 0, 0, 0, "/planets/F/0100", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 277 */
   {0, 0, 0, 0, "/planets/F/0101", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 278 */
   {0, 0, 0, 0, "/planets/F/0110", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 279 */
   {0, 0, 0, 0, "/planets/F/0111", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 280 */
   {0, 0, 0, 0, "/planets/F/1000", 0, I_F_1010, 0, 0, NULL, NULL, 0, 0}, /* 281 */
   {0, 0, 0, 0, "/planets/F/1001", 0, I_F_1011, 0, 0, NULL, NULL, 0, 0}, /* 282 */
   {0, 0, 0, 0, "/planets/F/1010", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 283 */
   {0, 0, 0, 0, "/planets/F/1011", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 284 */
   {0, 0, 0, 0, "/planets/F/1100", 0, I_F_1110, 0, 0, NULL, NULL, 0, 0}, /* 285 */
   {0, 0, 0, 0, "/planets/F/1101", 0, I_F_1111, 0, 0, NULL, NULL, 0, 0}, /* 286 */
   {0, 0, 0, 0, "/planets/F/1110", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 287 */
   {0, 0, 0, 0, "/planets/F/1111", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 288 */
   {0, 0, 0, 0, "/planets/F/m0000", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 289 */
   {0, 0, 0, 0, "/planets/F/m0001", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 290 */
   {0, 0, 0, 0, "/planets/F/m0010", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 291 */
   {0, 0, 0, 0, "/planets/F/m0011", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 292 */
   {0, 0, 0, 0, "/planets/F/m0100", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 293 */
   {0, 0, 0, 0, "/planets/F/m0101", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 294 */
   {0, 0, 0, 0, "/planets/F/m0110", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 295 */
   {0, 0, 0, 0, "/planets/F/m0111", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 296 */
   {0, 0, 0, 0, "/planets/F/m1000", 0, I_F_M1010, 0, 0, NULL, NULL, 0, 0}, /* 297 */
   {0, 0, 0, 0, "/planets/F/m1001", 0, I_F_M1011, 0, 0, NULL, NULL, 0, 0}, /* 298 */
   {0, 0, 0, 0, "/planets/F/m1010", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 299 */
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/planets/F/m1011", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 300 */
   {0, 0, 0, 0, "/planets/F/m1100", 0, I_F_M1110, 0, 0, NULL, NULL, 0, 0}, /* 301 */
   {0, 0, 0, 0, "/planets/F/m1101", 0, I_F_M1111, 0, 0, NULL, NULL, 0, 0}, /* 302 */
   {0, 0, 0, 0, "/planets/F/m1110", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 303 */
   {0, 0, 0, 0, "/planets/F/m1111", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 304 */
   {0, 0, 0, 0, "/planets/F_overlay/0000", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 305 */
   {0, 0, 0, 0, "/planets/F_overlay/0001", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 306 */
   {0, 0, 0, 0, "/planets/F_overlay/0010", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 307 */
   {0, 0, 0, 0, "/planets/F_overlay/0011", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 308 */
   {0, 0, 0, 0, "/planets/F_overlay/0100", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 309 */
   {0, 0, 0, 0, "/planets/F_overlay/0101", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 310 */
   {0, 0, 0, 0, "/planets/F_overlay/0110", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 311 */
   {0, 0, 0, 0, "/planets/F_overlay/0111", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 312 */
   {0, 0, 0, 0, "/planets/F_overlay/1000", 0, I_F_OVERLAY_1010, 0, 0, NULL, NULL, 0, 0}, /* 313 */
   {0, 0, 0, 0, "/planets/F_overlay/1001", 0, I_F_OVERLAY_1011, 0, 0, NULL, NULL, 0, 0}, /* 314 */
   {0, 0, 0, 0, "/planets/F_overlay/1010", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 315 */
   {0, 0, 0, 0, "/planets/F_overlay/1011", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 316 */
   {0, 0, 0, 0, "/planets/F_overlay/1100", 0, I_F_OVERLAY_1110, 0, 0, NULL, NULL, 0, 0}, /* 317 */
   {0, 0, 0, 0, "/planets/F_overlay/1101", 0, I_F_OVERLAY_1111, 0, 0, NULL, NULL, 0, 0}, /* 318 */
   {0, 0, 0, 0, "/planets/F_overlay/1110", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 319 */
   {0, 0, 0, 0, "/planets/F_overlay/1111", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 320 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_0000", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 321 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_0001", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 322 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_0010", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 323 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_0011", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 324 */
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_0100", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 325 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_0101", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 326 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_0110", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 327 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_0111", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 328 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_1000", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 329 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_1001", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 330 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_1010", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 331 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_1011", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 332 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_1100", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 333 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_1101", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 334 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_1110", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 335 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_1111", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 336 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_m0000", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 337 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_m0001", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 338 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_m0010", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 339 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_m0011", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 340 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_m0100", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 341 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_m0101", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 342 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_m0110", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 343 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_m0111", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 344 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_m1000", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 345 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_m1001", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 346 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_m1010", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 347 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_m1011", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 348 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_m1100", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 349 */
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_m1101", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 350 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_m1110", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 351 */
   {0, 0, 0, 0, "/planets/F_overlay/MOO_m1111", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 352 */
   {0, 0, 0, 0, "/planets/F_overlay/m0000", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 353 */
   {0, 0, 0, 0, "/planets/F_overlay/m0001", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 354 */
   {0, 0, 0, 0, "/planets/F_overlay/m0010", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 355 */
   {0, 0, 0, 0, "/planets/F_overlay/m0011", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 356 */
   {0, 0, 0, 0, "/planets/F_overlay/m0100", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 357 */
   {0, 0, 0, 0, "/planets/F_overlay/m0101", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 358 */
   {0, 0, 0, 0, "/planets/F_overlay/m0110", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 359 */
   {0, 0, 0, 0, "/planets/F_overlay/m0111", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 360 */
   {0, 0, 0, 0, "/planets/F_overlay/m1000", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 361 */
   {0, 0, 0, 0, "/planets/F_overlay/m1001", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 362 */
   {0, 0, 0, 0, "/planets/F_overlay/m1010", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 363 */
   {0, 0, 0, 0, "/planets/F_overlay/m1011", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 364 */
   {0, 0, 0, 0, "/planets/F_overlay/m1100", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 365 */
   {0, 0, 0, 0, "/planets/F_overlay/m1101", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 366 */
   {0, 0, 0, 0, "/planets/F_overlay/m1110", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 367 */
   {0, 0, 0, 0, "/planets/F_overlay/m1111", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 368 */
   {0, 0, 0, 0, "/planets/MOObarren", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 369 */
   {0, 0, 0, 0, "/planets/MOOmbarren", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 370 */
   {0, 0, 0, 0, "/planets/R/000", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 371 */
   {0, 0, 0, 0, "/planets/R/001", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 372 */
   {0, 0, 0, 0, "/planets/R/010", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 373 */
   {0, 0, 0, 0, "/planets/R/011", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 374 */
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/planets/R/100", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 375 */
   {0, 0, 0, 0, "/planets/R/101", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 376 */
   {0, 0, 0, 0, "/planets/R/110", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 377 */
   {0, 0, 0, 0, "/planets/R/111", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 378 */
   {0, 0, 0, 0, "/planets/R/m000", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 379 */
   {0, 0, 0, 0, "/planets/R/m001", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 380 */
   {0, 0, 0, 0, "/planets/R/m010", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 381 */
   {0, 0, 0, 0, "/planets/R/m011", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 382 */
   {0, 0, 0, 0, "/planets/R/m100", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 383 */
   {0, 0, 0, 0, "/planets/R/m101", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 384 */
   {0, 0, 0, 0, "/planets/R/m110", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 385 */
   {0, 0, 0, 0, "/planets/R/m111", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 386 */
   {0, 0, 0, 0, "/planets/R_overlay/000", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 387 */
   {0, 0, 0, 0, "/planets/R_overlay/001", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 388 */
   {0, 0, 0, 0, "/planets/R_overlay/010", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 389 */
   {0, 0, 0, 0, "/planets/R_overlay/011", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 390 */
   {0, 0, 0, 0, "/planets/R_overlay/100", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 391 */
   {0, 0, 0, 0, "/planets/R_overlay/101", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 392 */
   {0, 0, 0, 0, "/planets/R_overlay/110", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 393 */
   {0, 0, 0, 0, "/planets/R_overlay/111", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 394 */
   {0, 0, 0, 0, "/planets/R_overlay/m000", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 395 */
   {0, 0, 0, 0, "/planets/R_overlay/m001", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 396 */
   {0, 0, 0, 0, "/planets/R_overlay/m010", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 397 */
   {0, 0, 0, 0, "/planets/R_overlay/m011", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 398 */
   {0, 0, 0, 0, "/planets/R_overlay/m100", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 399 */
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/planets/R_overlay/m101", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 400 */
   {0, 0, 0, 0, "/planets/R_overlay/m110", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 401 */
   {0, 0, 0, 0, "/planets/R_overlay/m111", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 402 */
   {0, 0, 0, 0, "/planets/S/STND", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 403 */
   {0, 0, 0, 0, "/planets/S/THIN", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 404 */
   {0, 0, 0, 0, "/planets/S/TNTD", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 405 */
   {0, 0, 0, 0, "/planets/S/TOXC", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 406 */
   {0, 0, 0, 0, "/planets/S/mSTND", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 407 */
   {0, 0, 0, 0, "/planets/S/mTHIN", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 408 */
   {0, 0, 0, 0, "/planets/S/mTNTD", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 409 */
   {0, 0, 0, 0, "/planets/S/mTOXC", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 410 */
   {0, 0, 0, 0, "/planets/S_overlay/STND", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 411 */
   {0, 0, 0, 0, "/planets/S_overlay/THIN", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 412 */
   {0, 0, 0, 0, "/planets/S_overlay/TNTD", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 413 */
   {0, 0, 0, 0, "/planets/S_overlay/TOXC", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 414 */
   {0, 0, 0, 0, "/planets/S_overlay/mSTND", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 415 */
   {0, 0, 0, 0, "/planets/S_overlay/mTHIN", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 416 */
   {0, 0, 0, 0, "/planets/S_overlay/mTNTD", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 417 */
   {0, 0, 0, 0, "/planets/S_overlay/mTOXC", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 418 */
   {0, 0, 0, 0, "/planets/T/fedmplanet", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 419 */
   {0, 0, 0, 0, "/planets/T/fedplanet", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 420 */
   {0, 0, 0, 0, "/planets/T/indmplanet", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 421 */
   {0, 0, 0, 0, "/planets/T/indplanet", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 422 */
   {0, 0, 0, 0, "/planets/T/klimplanet", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 423 */
   {0, 0, 0, 0, "/planets/T/kliplanet", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 424 */
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/planets/T/orimplanet", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 425 */
   {0, 0, 0, 0, "/planets/T/oriplanet", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 426 */
   {0, 0, 0, 0, "/planets/T/rommplanet", 0, I_MBARREN, 0, 0, NULL, NULL, 0, 0}, /* 427 */
   {0, 0, 0, 0, "/planets/T/romplanet", 0, I_BARREN, 0, 0, NULL, NULL, 0, 0}, /* 428 */
   {0, 0, 0, 0, "/planets/T_overlay/fedmplanet", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 429 */
   {0, 0, 0, 0, "/planets/T_overlay/fedplanet", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 430 */
   {0, 0, 0, 0, "/planets/T_overlay/indmplanet", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 431 */
   {0, 0, 0, 0, "/planets/T_overlay/indplanet", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 432 */
   {0, 0, 0, 0, "/planets/T_overlay/klimplanet", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 433 */
   {0, 0, 0, 0, "/planets/T_overlay/kliplanet", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 434 */
   {0, 0, 0, 0, "/planets/T_overlay/orimplanet", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 435 */
   {0, 0, 0, 0, "/planets/T_overlay/oriplanet", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 436 */
   {0, 0, 0, 0, "/planets/T_overlay/rommplanet", 0, I_MBARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 437 */
   {0, 0, 0, 0, "/planets/T_overlay/romplanet", 0, I_BARREN_OVERLAY, 0, 0, NULL, NULL, 0, 0}, /* 438 */
   {0, 0, 0, 0, "/planets/asteroids/all", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 439 */
   {0, 0, 0, 0, "/planets/asteroids/bottomleftrounded", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 440 */
   {0, 0, 0, 0, "/planets/asteroids/bottomleftsquare", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 441 */
   {0, 0, 0, 0, "/planets/asteroids/bottomrightrounded", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 442 */
   {0, 0, 0, 0, "/planets/asteroids/bottomrightsquare", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 443 */
   {0, 0, 0, 0, "/planets/asteroids/topleftrounded", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 444 */
   {0, 0, 0, 0, "/planets/asteroids/topleftsquare", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 445 */
   {0, 0, 0, 0, "/planets/asteroids/toprightrounded", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 446 */
   {0, 0, 0, 0, "/planets/asteroids/toprightsquare", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 447 */
   {0, 0, 0, 0, "/planets/barren", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 448 */
   {0, 0, 0, 0, "/planets/barren_overlay", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 449 */
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/planets/masteroid", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 450 */
   {0, 0, 0, 0, "/planets/mbarren", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 451 */
   {0, 0, 0, 0, "/planets/mbarren_overlay", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 452 */
   {0, 0, 0, 0, "/planets/mplan_noinfo", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 453 */
   {0, 0, 0, 0, "/planets/mwormhole", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 454 */
   {0, 0, 0, 0, "/planets/plan_noinfo", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 455 */
   {0, 0, 0, 0, "/planets/star", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 456 */
   {0, 0, 0, 0, "/planets/starm", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 457 */
   {0, 0, 0, 0, "/planets/wormhole", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 458 */
   {0, 0, 0, 0, "/rainbow", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 459 */
   {0, 0, 0, 0, "/sbexplosion", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 460 */
   {0, 0, 0, 0, "/stipple", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 461 */
   {0, 0, 0, 0, "/warpbeacon", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 462 */
   {0, 0, 0, 0, "/warpflash", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 463 */
   {0, 0, 0, 0, "/weapons/edrone", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 464 */
   {0, 0, 0, 0, "/weapons/edronecloud", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 465 */
   {0, 0, 0, 0, "/weapons/efighter", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 466 */
   {0, 0, 0, 0, "/weapons/efightercloud", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 467 */
   {0, 0, 0, 0, "/weapons/eplasmacloud", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 468 */
   {0, 0, 0, 0, "/weapons/eplasmatorp", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 469 */
   {0, 0, 0, 0, "/weapons/etorp", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 470 */
   {0, 0, 0, 0, "/weapons/etorpcloud", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 471 */
   {0, 0, 0, 0, "/weapons/kitchen_sink", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 472 */
   {0, 0, 0, 0, "/weapons/mdrone", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 473 */
   {0, 0, 0, 0, "/weapons/mdronecloud", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 474 */
/* {width,height,frames,xpm,filename,loaded,alternate,bad,compiled_in,xbmdata,xpmdata,pixmap,clipmask} */
   {0, 0, 0, 0, "/weapons/mfighter", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 475 */
   {0, 0, 0, 0, "/weapons/mfightercloud", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 476 */
   {0, 0, 0, 0, "/weapons/mplasmacloud", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 477 */
   {0, 0, 0, 0, "/weapons/mplasmatorp", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 478 */
   {0, 0, 0, 0, "/weapons/mtorp", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 479 */
   {0, 0, 0, 0, "/weapons/mtorpcloud", 0, I_DEFAULT, 0, 0, NULL, NULL, 0, 0}, /* 480 */
};

int
getImageNum(W_Image *image)
{
  return image - &imagearray[0];
}

W_Image *
getImage(offset)
  int offset;
{
  W_Image * image = &imagearray[offset];
  if(!image->loaded) {
    W_LoadImage(image);
  }
  return image;
}

W_Image *
getShipImage(team,ship)
  int team,ship;
{
  return getImage(teamImageOffset[team] + shipImageOffset[ship]);
}

#ifndef __STDC__
#define const
#endif /*__STDC__*/

int cmpfilenames(left,right)
  const void *left, *right;
{
  return strcmp((char*)left,((W_Image*)right)->filename);
}

void
loadImageByFilename(filename)
  char *filename;
{
  W_Image *image;

  image = bsearch(filename, imagearray, sizeof(imagearray) / sizeof(W_Image),
                  sizeof(W_Image), cmpfilenames);
  if(image && !image->loaded) W_LoadImage(image);
}

void
loadAllImages()
{
  int i;
  for(i=I_FIRST; i<=I_LAST; i++) 
    if(!imagearray[i].loaded) 
      W_LoadImage(&imagearray[i]);
}
