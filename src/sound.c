/*
 * sound.c - Platform Independant Sound Support - Dec. 1994
 *
 * Copyright 1994 Sujal M. Patel (smpatel@wam.umd.edu)
 * Conditions in "copyright.h"          
 */

#include "defines.h"
#include <stdio.h>
#ifdef STDC_HEADERS
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#ifdef HAVE_TIME_H
#include <time.h>
#endif
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#else
#ifdef HAVE_SYS_FCNTL_H
#include <sys/fcntl.h>
#endif
#endif
#include <signal.h>
#include <sys/stat.h>
#include "data.h"
#include "uname.h"



static int f;
static char audioOK = 1;
static char sound_flags[20]; /* Sound Flag for sound 1-19 */



void init_sound ()
{
  int i, pid, child;
  char *argv[3];
  char filename[512];
#if 0 /* replaced with MACHINE_UNAME */
#ifdef linux
  char *arch = "linux";
#else
#ifdef sun
  char *arch = "sun";
#else
#ifdef __FreeBSD__
  char *arch = "freebsd";
#else
  char *arch = "generic";
#endif
#endif
#endif
#endif

  char *arch = MACHINE_UNAME;

  signal(SIGCHLD, SIG_IGN);

if (unixSoundPath[0] == '?') { audioOK = 0; return; }
else if (!(child = fork ()))
     {
       sprintf (filename, "paradise.sndsrv.%s", arch);
       argv[0] = filename;
       argv[1] = unixSoundPath;
       argv[2] = 0;
       execvp(filename, argv);
       fprintf (stderr, "Couldn't Execute Sound Server (paradise.sndsrv.%s)!\n", arch);
       exit (0);
     }

  /* My god, why is this so ugly.. Clean me up!! */
  /* This was originally used to ease debuging, since the sound */
  /* Server could be executed independently.  We should just */
  /* use stdin/stdout for our communications */
  pid = getpid();
  sprintf (filename, "/tmp/paradise.sound.fifo.pid.%d", pid);
  mkfifo (filename, (mode_t)(0666 & ~umask(0)));
  sleep (1);

  if (kill (child, 0)) audioOK = 0;  /* Child has exited (there is no sound server running) */

  if (audioOK)
  {
    f = open (filename, O_WRONLY);
  }

  unlink (filename);

  for (i = 0; i < 19; i++) sound_flags[i] = 0;
} 



void play_sound (k)
int k;
{
  char c;

  c = k;
  if ((playSounds) && (audioOK)) write (f, &c, sizeof (c));
}



void maybe_play_sound (k)
int k;
{
  char c;

  if (sound_flags[k] & 1) return;

  sound_flags[k] |= 1;

  c = (unsigned char)(k);
  if ((playSounds) && (audioOK)) write (f, &c, sizeof (c));
}



void sound_completed (k)
int k;
{
  sound_flags[k] &= ~1;
}



void kill_sound ()
{ 
  char c;

  c = -1;               
  if ((playSounds) && (audioOK)) write (f, &c, sizeof (c));
}
