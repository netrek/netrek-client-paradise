/* strfuncs.c
 *
 * Kevin P. Smith  6/11/89
 * Bill Dyess      9/29/93  - moved all default settings to resetDefaults
 *                           from main
 * Bill Dyess	   11/8/93  - created expandFilename function
 * Bill Dyess      11/20/93 - added #include capability
 * Bob Glamm       9/25/95 - moved strcmpi, strncmpi out of defaults.c
 */

#ifndef STRFUNCS_H_
#define STRFUNCS_H_

#include "defines.h"
#include "defs.h"

#ifndef HAVE_STRCMPI
#ifndef HAVE_STRCASECMP
int strcmpi P((char *str1, char *str2));
#endif
#endif

#ifndef HAVE_STRNCMPI
#ifndef HAVE_STRNCASECMP
int strncmpi P((char *str1, char *str2, int len));
#endif
#endif

#ifndef HAVE_STRDUP
#if !defined(IRIX)
char *strdup P((char *s));
#endif
#endif 

#ifndef HAVE_TOLOWER
#if !defined(IRIX)
char tolower P((char ch));
#endif
#endif

#ifndef HAVE_TOUPPER
#if !defined(IRIX)
char toupper P((char ch));
#endif
#endif

#endif /* STRFUNCS_H_ */
