/* config.h: all the configuration #define's go here [BDyess] */
/* system specific #defines (they're in the Makefile) */


/* Compiling this client with any flags other than the ones supplied can
   cause problems. Sorry. */

/*
  TSH           -- changes by T. Hadley
  TC            -- changes by Terence Chang
  SGALAXY       -- changes by Jerry Frain & Joe Young
  ancient.  defined elsewhere?
*/


/*
  ?
*/
#define GODMESSAGE

/*
  TCURSORS	-- special cursors
*/
#define TCURSORS

/*
  ROTATERACE	-- galaxy rotation
*/
#define ROTATERACE

/*
  NOWARP	-- enable alternates to mouse pointer message warp
*/
#define NOWARP

/*
  EDEN		-- turn on partial eden 1.996c compatibility.
		   Breaks everything else (plasma_info, MAXTORPS, MAXPLASMA)
*/
/*#define EDEN*/

/*
  SHORT_PACKETS -- Heiko Wengler's short packets.  Not prettily transplanted
		   from Berkricksmoo2.0pl14.
*/
#define SHORT_PACKETS

/*
  METASERVER    -- turns on -m metaserver support (Bill Dyess)
*/
#define METASERVER

/*
  VARY_HULL     -- must be defined for vary hull feature (Bill Dyess)
*/
#define VARY_HULL

/*
  MACROS	-- must be defined for macros (Bill Dyess)
*/
#define MACROS

/*
  TIMER		-- enables the dashboard timer (Bill Dyess)
*/
#define TIMER

/*
  CONTINUOUS_MOUSE -- enable the continuous mouse code (Bill Dyess)
*/
#define CONTINUOUS_MOUSE

/*
  XTREKRC_HELP  -- must be defined for .xtrekrc help window (Bill Dyess)
*/
#define XTREKRC_HELP

/*
  PACKET_LIGHTS -- turns on packetLights (Bill Dyess)
*/
#define PACKET_LIGHTS

/*
  WIDE_PLIST    -- enables wide (fully configurable) playerlist (Bill Dyess)
*/
#define WIDE_PLIST

/*
  SIZE_LOGGING  -- shows bytes sent/received on exit (Bill Dyess)
*/
#define SIZE_LOGGING

/*
  FEATURE	-- turns on feature packets (Joe Rumsey)
*/
#define FEATURE

/*
  RC_DISTRESS   -- Receiver configurable distress.  Code from COW-lite, 
  		   partially added by Bill Dyess, finished by Joe Rumsey.
*/
#define RC_DISTRESS

/*
  BEEPLITE      -- Beep-lite code for RCD, code from COW-lite (Joe Rumsey)
*/
#define BEEPLITE

/*
  RECORDER      -- Flight recorder/playback (Joe Rumsey)
*/
#define RECORDER

/*
  NOSHIBITMAPS  -- compile with only CA/SB bitmaps for each race,
		   saves ~50k in the executable, but then the player MUST
		   have bitmap albums, or else see everyone as a CA.
*/
/*#define NOSHIPBITMAPS*/

/*
  LOCAL_SHIPSTATS -- An ugly little ship status thing drawn into the
		     local window. Configurable... (Joe Rumsey)
*/
#define LOCAL_SHIPSTATS

/*
  SHOW_IND	-- Draw an X over Independant planets. from COW-lite(Joe Rumsey)
*/
#define SHOW_IND

/*
  TOOLS 	-- slightly silly thing to allow shell commands from message 
  		   window.  Would be very silly, but also allowed in macros. 
		   from COW (Joe)
*/
#define TOOLS

/*
  HANDLER_TIMES -- record execution times of packet handlers, 
		   don't define normally (Joe Rumsey)
*/
/*#define HANDLER_TIMES*/

/*
  COW_HAS_IT_WHY_SHOULDNT_WE -- COW-borg feature that shows the map while you're
				at the motd.  Not defined here, is optional,
				defaults to off, and I will not document it,
				Nor will I put it in 'O'ptions. (Joe Rumsey)
*/
/*#define COW_HAS_IT_WHY_SHOULDNT_WE*/

/*
  HOCKEY	-- Turns on lots of hockey specific features (Bill Dyess)
*/
#define HOCKEY

/*
  BUFFERING	-- Turns on window double buffering, eliminating flicker 
                   (Bill Dyess, Dave Ahn)
*/
#define BUFFERING

/*
  XPM		-- Enables full color Pixmaps. (Bill Dyess)
*/
#define XPM

/*
  HACKED_XPMLIB    -- use the hacked XPM library
*/
/*#define HACKED_XPMLIB*/

/*
  UNIX_SOUND    -- Enables Platform Independent Sound Support. (Sujal Patel)
*/
#define UNIX_SOUND

/*
  ASTEROIDS     -- Enables the client to understand and display asteroids.
                   (Mike McGrath)
*/
#define ASTEROIDS


/*
  BROKEN_STIPPLE -- enable this if your scrollbars are blank. [BDyess]
 */
/*#define BROKEN_STIPPLE*/

/*
  LITE		-- Build TedTurner/Lite.  (Dave Ahn)
 */

/* #define LITE */

#ifdef LITE

#undef XPM
#undef BUFFERING

#endif

