/*
 * keymap.c
 * Bill Dyess, 10/20/93
 */

#include "copyright.h"
#include "defines.h"
#include <stdio.h>
#include <ctype.h>
#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#include "config.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

#define control(x) (x)+128

/*#define KEYMAP_DEBUG*/
#ifdef KEYMAP_DEBUG
void dumpKeymap();
#endif /*KEYMAP_DEBUG*/

int
doKeymap(data)
    W_Event *data;
{
    int     key = data->key;

#ifdef MACROS
    if (macroState) {		/* macro needed a destination or in macro
				   mode */
	if (key > 256 && data->type != W_EV_BUTTON) {	/* an alt-key will exit
							   the macro mode */
	    /*
	       but let mouse buttons do macros! button conversion is done by
	       calling function.
	    */
	    warning("             ");
	} else {
	    if (key > 256)
		data->key -= 256;
	    doMacro(data);
	    return -1;
	}
    }
#endif				/* MACROS */

    if (key < 256) {		/* not alt key */
	key = myship->s_keymap[key];

#ifdef MACROS
	if (macrotable[key] && (macrotable[key]->flags & MACSINGLE)) {
	    data->key = key;
	    doMacro(data);
	    return -1;
	}
#endif				/* MACROS */

    } else
	key = key - 256;
    return key;
}

unsigned char def_keymap[256];
unsigned char def_buttonmap[12];
void
buildShipKeymap(shipp)
    struct ship *shipp;
{
    char    keybuf[40], ckeybuf[40], buttonbuf[40], cbuttonbuf[40];
    struct stringlist *l;

    bcopy(def_keymap, shipp->s_keymap, 256);
    bcopy(def_buttonmap, shipp->s_buttonmap, 12);

    sprintf(keybuf, "keymap.%c%c", shipp->s_desig[0], shipp->s_desig[1]);
    sprintf(ckeybuf, "ckeymap.%c%c", shipp->s_desig[0], shipp->s_desig[1]);
    sprintf(buttonbuf, "buttonmap.%c%c", shipp->s_desig[0], shipp->s_desig[1]);
    sprintf(cbuttonbuf, "cbuttonmap.%c%c", shipp->s_desig[0], shipp->s_desig[1]);

    for (l = defaults; l; l = l->next) {
	if (!strcmpi(keybuf, l->string))
	    keymapAdd(l->value, (char*)shipp->s_keymap);
	else if (!strcmpi(ckeybuf, l->string))
	    ckeymapAdd(l->value, (char*)shipp->s_keymap);
	else if (!strcmpi(buttonbuf, l->string))
	    buttonmapAdd(l->value, (char*)shipp->s_buttonmap);
	else if (!strcmpi(cbuttonbuf, l->string))
	    cbuttonmapAdd(l->value, (char*)shipp->s_buttonmap);
    }
}

void
initkeymap(type)
    int     type;
{
    char    keybuf[40], ckeybuf[40], buttonbuf[40], cbuttonbuf[40];
    int     i, j;
    struct stringlist *l;

    keybuf[0] = ckeybuf[0] = buttonbuf[0] = cbuttonbuf[0] = 0;

    if (type < 0) {
	for (i = 0; i < 256; i++)
	    def_keymap[i] = i;

	for (l = defaults; l; l = l->next) {
	    if (!strcmpi("keymap", l->string))
		keymapAdd(l->value, (char*)def_keymap);
	    else if (!strcmpi("ckeymap", l->string))
		ckeymapAdd(l->value, (char*)def_keymap);
	    else if (!strcmpi("buttonmap", l->string))
		buttonmapAdd(l->value, (char*)def_buttonmap);
	    else if (!strcmpi("cbuttonmap", l->string))
		cbuttonmapAdd(l->value, (char*)def_buttonmap);
	}

	for (j = 0; j < nshiptypes; j++) {
	    buildShipKeymap(getship(j));
	}
    }
}

void
keymapAdd(str, kmap)
    char   *str, *kmap;
{
    if (str) {
	/* parse non-control char keymap */
	while (*str != '\0' && *(str + 1) != '\0') {
	    if (*str >= 32 && *str < 127) {
		kmap[(int) *str] = *(str + 1);
	    }
	    str += 2;
	}
    }
}

void
ckeymapAdd(cstr, kmap)
    char   *cstr, *kmap;
{
    unsigned char key[2];
    short   state = 0;

    if (cstr) {
	/*
	   control chars are allowed, so use ^char to mean control, and ^^ to
	   mean ^
	*/
	while (*cstr != '\0') {
	    if (*cstr == '^') {
		cstr++;
		if (*cstr == '^' || !*cstr)
		    key[state] = '^';
		else
		    key[state] = 128 + *cstr;
	    } else {
		key[state] = *cstr;
	    }
	    if (*cstr)
		cstr++;
	    if (state)
		kmap[key[0]] = key[1];
	    state = 1 - state;
	}
    }
}

void
buttonmapAdd(str, kmap)
    char   *str, *kmap;
{
    unsigned char button, ch;

    if (str) {
	while (*str != '\0' && *(str + 1) != '\0') {
	    if (*str < 'a')
		button = *str++ - '1';
	    else
		button = 9 + *str++ - 'a';
	    if (button > 11)
		fprintf(stderr, "%c ignored in buttonmap\n", *(str - 1));
	    else {
		ch = *str++;
		kmap[button] = ch;
	    }
	}
    }
}

void
cbuttonmapAdd(cstr, kmap)
    char   *cstr, *kmap;
{
    unsigned char button, ch;

    if (cstr) {
	while (*cstr != '\0' && *(cstr + 1) != '\0') {
	    /*
	       code for cbuttonmap, which allows buttons to be mapped to
	       control keys. [BDyess]
	    */
	    if (*cstr < 'a')
		button = *cstr++ - '1';
	    else
		button = 9 + *cstr++ - 'a';
	    if (button > 11)
		fprintf(stderr, "%c ignored in cbuttonmap\n", *(cstr - 1));
	    else {
		ch = *cstr++;
		if (ch == '^') {
		    ch = *cstr++;
		    if (ch != '^')
			ch += 128;
		}
		kmap[button] = ch;
	    }
	}
    }
}

#ifdef KEYMAP_DEBUG
void
dumpKeymap()
{
    int     i;

    for (i = 0; i < 256; i++) {
	printf("%3d %c : %3d %c\n",
	       i,
	       isprint(i) ? i : '_',
	       myship->s_keymap[i],
	       isprint(myship->s_keymap[i]) ? myship->s_keymap[i] : '_');
    }
}
#endif				/* KEYMAP_DEBUG */
