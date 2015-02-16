/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		tclmsg.c
 *		Source file defining message output stuff for Tcl/Tk GUI
 *		version of Ripl.
 *
 *		Version 1.1, last update: 20 January 1998.
 *
 *		History:
 *			20/1/98:	acquired current filename.
 *			19/1/98:	cut out some obsolete stuff.
 *			27/11/97:	first implementation.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include <stdio.h>
#include "ripl.h"

#if defined(__WIN32__) || defined(__NT__)
/* Use Windows MessageBox GUI elements. */
#	include <windows.h>

#	define BUFFER_LEN			1024
static char buffer[BUFFER_LEN];

/* Display message---similar format to ANSI-C printf. */
int riplMessage(riplIconType icon, const char *format, ...) {
    va_list ap;
    UINT style;

    va_start(ap, format);
    vsprintf(buffer, format, ap);
    switch (icon) {
        case itWarning:
            style=MB_ICONEXCLAMATION;
            break;
        case itError:
            style=MB_ICONHAND;
            break;
        case itConsole:
        case itInfo:
            style=MB_ICONINFORMATION;
            break;
        case itDebug:
            return MessageBox(NULL,
                buffer, RIPL_APPNAME " Debugging", MB_ICONHAND | MB_OK);
        default:
            style=0;
            break;
    }
    return MessageBox(NULL, buffer, RIPL_APPNAME, style | MB_OK);
}

/* Display message---similar format to ANSI-C vprintf. */
int riplVMessage(riplIconType icon, const char *format, va_list ap) {
    UINT style;
    
    vsprintf(buffer, format, ap);
    switch (icon) {
        case itWarning:
            style=MB_ICONEXCLAMATION;
            break;
        case itError:
            style=MB_ICONHAND;
            break;
        case itConsole:
        case itInfo:
            style=MB_ICONINFORMATION;
            break;
        case itDebug:
            return MessageBox(NULL,
                buffer, RIPL_APPNAME " Debugging", MB_ICONHAND | MB_OK);
        default:
            style=0;
            break;
    }
    return MessageBox(NULL, buffer, RIPL_APPNAME, style | MB_OK);
}
#else
#	error Tcl/Tk version is Win32/NT only.
#endif
