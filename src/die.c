/*
  die.c
  Copyright 1995 Bill Dyess
 */

#include "defines.h"
#include <stdio.h>
#ifdef STDC_HEADERS
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#include <errno.h>
#include "die.h"

/* die prints the given string with %! replaced with the current error 
   string.  Otherwise, acts just like fprintf(stderr, format, va_list) 
   followed by exit(-1).
 */
void die(VaT(const char *format) VaAList) VaDcl {
  VaD(char *format)
  VaList(ap)
  char *buf;
  char *s = strerror(errno);
  int buflen = BUFSIZ, i = 0;

  VaStart(ap,format);
  VaI(ap, char*, format)
  buf = (char*)malloc(buflen);
  while(*format) {
    buf[i++] = *format++;
    if(buf[i-1] == '%' && *format == '!') {
      int l = strlen(s);

      i--;
      format++;
      if(i+l > buflen) {	/* double buffer size if necessary */
        buf = (char*)realloc(buf,buflen*=2);
      }
      strcpy(buf+i,s);
      i += l;
    }
    if(i >= buflen) {
      buf = (char*)realloc(buf,buflen*=2);
    }
  }
  buf[i] = 0;
  vfprintf(stderr,buf,ap);
  free(buf);
  VaEnd(ap)
  exit(errno);
}
