/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplmsg.h
 *		Header file defining message input stuff.
 *
 *		Version 1.1, last update: 20 January 1998.
 *
 *		History:
 *			20/1/98:		version 1.1.
 *			27/11/97:	first version.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _RIPLMSG_H_INCLUDED
#define _RIPLMSG_H_INCLUDED

#include "ripldefs.h"
#include "riplmain-defs.h"
#include <cstdarg>

typedef enum tagriplIconType {
    itWarning,
    itError,
    itConsole,
    itInfo,
    itNone,
    itDebug
} riplIconType;

RIPL_RIPLMAIN_API int riplMessage(riplIconType icon, const char* format, ...);
int riplVMessage(riplIconType icon, const char *format, va_list ap);

#endif
