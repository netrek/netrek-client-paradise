/**
	<Va.h> -- MUVES "Va" (variable argument) package definitions

**/
/*
	created:	94/08/16	D A Gwyn
	last edit:	94/08/19	D A Gwyn
	SCCS ID:	@(#)Va.h	1.1
 */
/**

	The Va package provides portable support for functions taking
	a variable number of arguments.  <Va.h> defines several macros
	that work together to hide the differences between the old
	UNIX <varargs.h> facility and the new Standard C <stdarg.h>.

	Rather than describing each Va package macro separately, it is
	best to give an example of the proper usage of the whole set of
	macros.  It should be easy to adapt this generic example to any
	specific requirement.

	The example is for a function named Advise that has two
	required arguments, the first being a printf-like format string
	and the second a flag that indicates (when true) that an extra
	"verbosity level" is provided as the third argument.  Remaining
	arguments are those, if any, associated with the format string.
	The Advise function prints the formatted result on the standard
	error output if and only if the verbosity level is given and is
	greater than 0.  It returns true unless it had trouble printing
	on the standard error output stream.

	Any code that wants to invoke the Advise function must include
	a proper declaration for it:

	#include	<Va.h>
	extern bool	Advise( VaT(const char *) VaT(bool) VaDots );

	The implementation of the Advise function might be:

	#include	<stdio.h>
	#include	<std.h>
	#include	<Va.h>

	// VARARGS				// not VARARGS2
	bool
	Advise( VaT( const char *format ) VaT( bool verbose ) VaAList )
		VaDcl
		{
		VaD( char	*format )	// no "const" here
		VaD( bool	verbose )
		VaList(		ap )
		register int	verbosity;
		register bool	status;

		VaStart( ap, verbose )
		VaI( ap, char *, format )	// no "const" here
		VaI( ap, bool, verbose )

		if ( verbose )
			verbosity = VaArg( ap, int );
		else
			verbosity = 0;

		if ( verbosity > 0 )
			status = vfprintf( stderr, format, ap ) > 0;
		else
			status = true;

		VaEnd( ap )
		return status;
		}

	Note that several of these macros are reminiscent of the va_*
	macros in <varargs.h> or <stdarg.h>, but there are significant
	differences.  Proper usage of the "function-like" macros, in
	particular, does not require semicolons; this is intentional,
	in order to avoid warnings about "null statements" from certain
	compilers and "lint".  It is suggested that the best way to use
	this package is to copy the above example and then make changes
	to the copy that are necessary for the specific application.
**/

#if	__STDC__

#include	<stdarg.h>

#define	VaT( t )		t,
#define	VaDots			...
#define	VaAList			...
#define	VaD( d )		/* nothing */
#define	VaDcl			/* nothing */
#define	VaList( ap )		va_list ap;
#define	VaStart( ap, A0 )	va_start( ap,A0 );
#define	VaI( ap, T, Ai )	/* nothing */
#define	VaArg( ap, T )		va_arg( ap, T )
#define	VaEnd( ap )		va_end( ap );

#else	/* "classic" version of UNIX assumed */

#include	<varargs.h>

#define	VaT( t )		/* nothing */
#define	VaDots			/* nothing */
#define	VaAList			va_alist
#define	VaD( d )		d;
#define	VaDcl			va_dcl
#define	VaList( ap )		va_list ap;
#define	VaStart( ap, A0 )	va_start( ap );
#define	VaI( ap, T, Ai )	Ai = va_arg( ap, T );
#define	VaArg( ap, T )		va_arg( ap, T )
#define	VaEnd( ap )		va_end( ap );

#endif

