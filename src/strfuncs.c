/* strfuncs.c
 *
 * Kevin P. Smith  6/11/89
 * Bill Dyess      9/29/93  - moved all default settings to resetDefaults
 *                           from main
 * Bill Dyess	   11/8/93  - created expandFilename function
 * Bill Dyess      11/20/93 - added #include capability
 * Bob Glamm       9/25/95 - moved strcmpi, strncmpi out of defaults.c
 */

#include "defines.h"
#include <ctype.h>
#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#include "copyright2.h"
#include "config.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"
#include "images.h"
#include "strfuncs.h"

#ifndef HAVE_STRCMPI
#ifndef HAVE_STRCASECMP
int
strcmpi(str1, str2)
    char   *str1, *str2;
{
#ifndef NeXT
    for (;;) {
	register char c1 = *str1, c2 = *str2;
	if (c1 <= 'z' && c1 >= 'a')
	    c1 += 'A' - 'a';
	if (c2 <= 'z' && c2 >= 'a')
	    c2 += 'A' - 'a';
	if (c1 != c2)
	    return (c2 - c1);
	if (c1 == 0 || c2 == 0)
	    return (0);
	str1++;
	str2++;
    }
#else
    return (strcmp(str1, str2));
#endif
}
#endif
#endif

#ifndef HAVE_STRNCMPI
#ifndef HAVE_STRNCASECMP
int
strncmpi(str1, str2, len)
    char   *str1, *str2;
    int     len;
{
    int     i;

    for (i = 0; i < len; i++) {
	if (tolower(*str1) != tolower(*str2))
	    return (*str2 - *str1);
	if (*str1 == 0 || *str2 == 0)
	    return (0);
	str1++;
	str2++;
    }
    return 0;
}
#endif
#endif

#ifndef HAVE_STRDUP
char *
strdup(s)
    char *s;
{
    char *n;

    if( !s )
      return( NULL );
    n = (char *)malloc(strlen(s)+1);
    strcpy( n, s );
    return( n );
}
#endif 

#ifndef HAVE_TOLOWER
char tolower(char ch) {
  if(ch >= 'A' && ch <= 'Z') {
    ch = ch + 'a' - 'A';
  }
  return ch;
}
#endif

#ifndef HAVE_TOUPPER
char toupper(char ch) {
  if(ch >= 'a' && ch <= 'z') {
    ch = ch - 'a' - 'A';
  }
  return ch;
}
#endif

char *
strtoupper(s)
    char *s;
{
    char *t;

    t = s;
    if( !s )
      return(NULL);
    while( *s )
    {
      *s = toupper(*s);
      s++;
    }
    return( t );
}

char *
strtolower(s)
    char *s;
{
    char *t;

    t = s;
    if( !s )
      return(NULL);
    while( *s )
    {
      *s = tolower(*s);
      s++;
    }
    return( t );
}
