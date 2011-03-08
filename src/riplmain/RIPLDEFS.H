/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		ripldefs.h
 *		Header file defining some platform-dependent macros and stuff.
 *		Included by ripl.h.
 *
 *		Version 1.1, last update: 17 May 1998.
 *
 *		History:
 *			17/5/98:		Scrapped allocation macros facility.
 *			16/5/98:		RConsole now obsolete.
 *			31/1/98:		added some more constants and changed prefix to RIPL_M_
 *			23/1/98:		added mathematical constants and renamed them all
 *							with prefixes RIPL_
 *			20/1/98:		version 1.1.
 *			24/12/97:	added Watcom386 DOS extender stuff and M_PI.
 *			2/12/97:		first major update.
 *			27/11/97:	first version.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _RIPLDEFS_H_INCLUDED
#define _RIPLDEFS_H_INCLUDED

/* Definitions of some mathematical constants. */
#define RIPL_M_E					2.71828182845904523536
#define RIPL_M_LOG2E				1.44269504088896340736
#define RIPL_M_LOG10E			0.434294481903251827651
#define RIPL_M_LN2				0.693147180559945309417
#define RIPL_M_LN10				2.30258509299404568402
#define RIPL_M_PI					3.14159265358979323846
#define RIPL_M_PI_2				1.57079632679489661923
#define RIPL_M_PI_4				0.785398163397448309616
#define RIPL_M_1_PI				0.318309886183790671538
#define RIPL_M_2_PI				0.636619772367581343076
#define RIPL_M_1_SQRTPI			0.564189583547756286948
#define RIPL_M_2_SQRTPI			1.12837916709551257390
#define RIPL_M_TWICE_PI			6.28318530717959
#define RIPL_M_SQRT_TWICE_PI	2.506628274631
#define RIPL_M_SQRT2				1.41421356237309504880
#define RIPL_M_SQRT_2			0.707106781186547524401

/*
 * Main symbols:
 *		_RIPL_PARANOID_DEBUGGING
 *		_RIPL_DEBUG_WITH_ASSERT
 *		_RIPL_DEBUG_MESSAGES
 *		_RIPL_ALLOC_DEBUG
 */
#define _RIPL_PARANOID_DEBUGGING
#define _RIPL_DEBUG_MESSAGES

/*
 *		_RIPL_PARANOID_DEBUGGING
 *
 * Define this macro to force Ripl to systematically check all memory
 * allocations and so on by inserting calls to 'assert' or other
 * suitable validation function in place of RIPL_VALIDATE lines
 * in the source code. Suppress by defining NDEBUG.
 *
 * Default: on (perform paranoid debugging checks).
 */

/*
 *		_RIPL_DEBUG_WITH_ASSERT
 *
 * Define this macro to compile Ripl with calls to the standard
 * function 'assert' as opposed to the Ripl assertion routine
 * during run-time validation (enabled with _RIPL_PARANOID_DEBUGGING
 * described above).
 *
 * Default: undefined (use Ripl assertion routine).
 */

/*
 *		_RIPL_DEBUG_MESSAGES
 *
 * Define this macro to compile Ripl with debugging messages indicating
 * entry into functions etc.
 *
 * Default: undefined (use Ripl assertion routine).
 */

/*
 *		_RIPL_ALLOC_DEBUG
 * Introduce extra debugging and logging code for memory allocations.
 *
 * Default: undefined (no extra debugging code)
 */

/*
 * General-purpose macros.
 */
#define RIPL_APPNAME				"RIPL"
#define RIPL_VERSION				"1.1"
#define RIPL_AUTHOR				"Richard A. Cook"
#define RIPL_DESCRIPTION		"Richard's Image-Processing Library"
#define RIPL_DATE					__DATE__ " " __TIME__
#define RIPL_EXENAME				"ripl"
#define RIPL_CMDLINE				"<input> <output>"

#endif
