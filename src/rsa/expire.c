#include <sys/time.h>
#include <time.h>
#include <stdio.h>


/* Here's a handy perl script to set any timeout you want:

   perl -e 'require "timelocal.pl"; print &timelocal(0,0,0,9,9,93);'

   The arguments to timelocal are (sec, min, hours, mday, mon, year).
   These are all 0 based indexing so the 7th day of the month is
   really 6 to the perl script.
   */
/* New way: use 
   ../scripts/expiretime hour:minute:second month/day/year >> expire.c
   and then edit expire.c to move the time line where it goes.
   [BDyess]
*/

static time_t expire = 946530000;

void 
checkExpire(verbose)
    int     verbose;
{
    time_t  now;

    now = time(NULL);
    if (verbose) {
	printf("This client expires on %s\n", ctime(&expire));
	return;
    }
    if (now > expire) {
	printf(
          "This client has expired.  You may bypass the expire by\n"
          "using the -o option which disables RSA authentication.  Expired\n"
          "clients are not supported by paradise-workers.\n"
          "You can find a new set of binaries on ftp.netrek.org in the\n"
          "directory /pub/netrek/clients/tedturner/.  If you have any\n"
          "problems mail to ahn@netrek.org\n");
	exit(0);

    } else if (now + 10 * 24 * 60 * 60 > expire) {
	printf("This client expires in %d days.\n", (int) (expire - now) / (60 * 60 * 24));
    }
}
