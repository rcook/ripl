/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		ripldbug.h
 *		Header file for debugging macros/functions.
 *
 *		Version 1.1, last update: 23 January 1998.
 *
 *		History:
 *			23/1/98:		removed RIPL_VALIDATE_IS_POWER_OF_2 macro.
 *			23/1/98:		added RIPL_VALIDATE_IS_POWER_OF_2 macro.
 *			20/1/98:		version 1.1.
 *			20/1/98:		slightly altered debugging macros (added more brackets).
 *			19/1/98:		added RIPL_DEBUG_STATEMENT macro.
 *			24/12/97:	added RGBMap debugging macros.
 *			27/11/97:	first version.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 *
 *		Define the symbol _RIPL_PARANOID_DEBUGGING for these macros
 *		to have any effect. Otherwise they expand to blank.
 */
#ifndef _RIPLDBUG_H_INCLUDED
#define _RIPLDBUG_H_INCLUDED

#include "ripldefs.h"

#ifdef _RIPL_DEBUG_WITH_ASSERT
#	include <assert.h>
#	define RIPL_ASSERT(expr)			assert(expr)
#else
#	include "riplmsg.h"
#	define RIPL_ASSERT(expr) \
        if (!(expr)) { \
            riplMessage(itDebug, \
                "Assertion failed: %s \n" \
                "Line %u in file " __FILE__ "\n\n" \
                "Programme will terminate.\n", #expr, __LINE__); \
            abort(); \
        }
#endif

#ifdef _RIPL_PARANOID_DEBUGGING
    /* General-purpose debugging macro---exactly same as 'assert' for time being. */
#	define RIPL_VALIDATE(expr)		RIPL_ASSERT(expr);
    /* Verifies that the specified greymap object is valid. */
#	define RIPL_VALIDATE_GREYMAP(greymap) \
        RIPL_ASSERT((greymap) && (greymap)->data); \
        RIPL_ASSERT((greymap)->cols>0 && (greymap)->rows>0); \
        RIPL_ASSERT((greymap)->size== \
            (unsigned long)(greymap)->cols*(greymap)->rows);
    /* Verifies that the specified RGBMap object is valid. */
#	define RIPL_VALIDATE_RGBMAP(rgbmap) \
        RIPL_ASSERT((rgbmap)); \
        RIPL_ASSERT(((rgbmap)->red && (rgbmap)->green \
            && (rgbmap)->blue && !(rgbmap)->mono) \
            || (!(rgbmap)->red && !(rgbmap)->green \
            && ! (rgbmap)->blue && (rgbmap)->mono)); \
        RIPL_ASSERT((rgbmap)->cols>0 && (rgbmap)->rows>0); \
        RIPL_ASSERT((rgbmap)->size==(unsigned long)(rgbmap)->cols*(rgbmap)->rows);
    /* Validate greymaps passed to operator-handling functions. */
#	define RIPL_VALIDATE_OP_GREYMAPS(greymap1, greymap2) \
        RIPL_ASSERT((greymap1) && (greymap2)); \
        RIPL_ASSERT((greymap1)->data && (greymap2)->data); \
        RIPL_ASSERT((greymap1)->cols>0 && (greymap1)->rows>0); \
        RIPL_ASSERT((greymap1)->size==(unsigned long)(greymap1)->cols*(greymap1)->rows); \
        RIPL_ASSERT((greymap1)->cols==(greymap2)->cols); \
        RIPL_ASSERT((greymap1)->rows==(greymap2)->rows); \
        RIPL_ASSERT((greymap1)->size==(greymap2)->size);
    /* Validate RGBMap objects passed to operator-handling functions. */
#	define RIPL_VALIDATE_OP_RGBMAPS(rgbmap1, rgbmap2) \
        RIPL_ASSERT((rgbmap1) && (rgbmap2)); \
        RIPL_ASSERT(((rgbmap1)->red && (rgbmap1)->green \
            && (rgbmap1)->blue && !(rgbmap1)->mono) \
            || (!(rgbmap1)->red && !(rgbmap1)->green \
            && ! (rgbmap1)->blue && (rgbmap1)->mono)); \
        RIPL_ASSERT(((rgbmap2)->red && (rgbmap2)->green \
            && (rgbmap2)->blue && !(rgbmap2)->mono) \
            || (!(rgbmap2)->red && !(rgbmap2)->green \
            && ! (rgbmap2)->blue && (rgbmap2)->mono)); \
        RIPL_ASSERT(((rgbmap1)->mono && (rgbmap2)->mono) \
            || (!(rgbmap1)->mono && !(rgbmap2)->mono)); \
        RIPL_ASSERT((rgbmap1)->cols>0 && (rgbmap1)->rows>0); \
        RIPL_ASSERT((rgbmap1)->size==(unsigned long)(rgbmap1)->cols*(rgbmap1)->rows); \
        RIPL_ASSERT((rgbmap1)->cols==(rgbmap2)->cols); \
        RIPL_ASSERT((rgbmap1)->rows==(rgbmap2)->rows); \
        RIPL_ASSERT((rgbmap1)->size==(rgbmap2)->size);
    /* General-purpose debugging---compiles 'text' only if debugging enabled. */
#	define RIPL_DEBUG_STATEMENT(text)	text
#else
#	define RIPL_VALIDATE(expr)
#	define RIPL_VALIDATE_GREYMAP(greymap)
#	define RIPL_VALIDATE_RGBMAP(rgbmap)
#	define RIPL_VALIDATE_OP_GREYMAPS(greymap1, greymap2)
#	define RIPL_VALIDATE_OP_RGBMAPS(rgbmap1, rgbmap2)
#	define RIPL_DEBUG_STATEMENT(text)
#endif

#ifdef _RIPL_DEBUG_MESSAGES
#	define RIPL_DEBUG_MESSAGE(msg) \
        riplMessage(itDebug, \
            "Debugging message: \"%s\"\n" \
            "At line %u in file " __FILE__ ".\n", \
            msg, __LINE__);
#else
#	define RIPL_DEBUG_MESSAGE(msg)
#endif

#endif

