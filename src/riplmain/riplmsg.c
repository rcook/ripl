/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplmsg.c
 *		Source file defining message input stuff.
 *		This is where user should define platform-dependent
 *		input/output functions.
 *
 *		Version 1.1, last update: 20 January 1998.
 *
 *		History:
 *			20/1/98:		version 1.1.
 *			27/11/97:	first implemented.
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include <stdio.h>
#include <stdlib.h>
#include "riplmsg.h"
#include "riplmisc.h"

#if defined(__WIN32__) || defined(__NT__)
/* Use Windows GUI elements in addition to stdout. */
#include <windows.h>

#define BUFFER_LEN			1024
static char buffer[BUFFER_LEN];

/* Display message---similar format to ANSI-C printf. */
int riplMessage(riplIconType icon, const char *format, ...) {
    va_list ap;
    UINT style;

    va_start(ap, format);
    vsprintf(buffer, format, ap);
    switch (icon) {
        case itConsole:
            return vprintf(format, ap);
        case itDebug:
            return MessageBox(NULL,
                buffer, RIPL_APPNAME " Debugging", MB_ICONHAND | MB_OK);
        case itWarning:
            style=MB_ICONEXCLAMATION;
            break;
        case itError:
            style=MB_ICONHAND;
            break;
        case itInfo:
            style=MB_ICONINFORMATION;
            break;
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
        case itConsole:
            return vprintf(format, ap);
        case itDebug:
            return MessageBox(NULL,
                buffer, RIPL_APPNAME " Debugging", MB_ICONHAND | MB_OK);
        case itWarning:
            style=MB_ICONEXCLAMATION;
            break;
        case itError:
            style=MB_ICONHAND;
            break;
        case itInfo:
            style=MB_ICONINFORMATION;
            break;
        default:
            style=0;
            break;
    }
    return MessageBox(NULL, buffer, RIPL_APPNAME, style | MB_OK);
}

#else
/* Use ANSI-standard C console functions. */

/* Display message---similar format to ANSI-C printf. */
int riplMessage(riplIconType icon, const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    switch (icon) {
        case itWarning:
            printf(RIPL_APPNAME " Warning:\n");
            return vprintf(format, ap);
        case itError:
            fprintf(stderr, RIPL_APPNAME " Error:\n");
            return vfprintf(stderr, format, ap);
        case itDebug:
            fprintf(stderr, RIPL_APPNAME " Debugging:\n");
            return vfprintf(stderr, format, ap);
        case itConsole:
        case itInfo:
        case itNone:
        default:
            return vprintf(format, ap);
    }
}

/* Display message---similar format to ANSI-C vprintf. */
int riplVMessage(riplIconType icon, const char *format, va_list ap) {
    switch (icon) {
        case itWarning:
            printf(RIPL_APPNAME " Warning:\n");
            return vprintf(format, ap);
        case itError:
            fprintf(stderr, RIPL_APPNAME " Error:\n");
            return vfprintf(stderr, format, ap);
        case itDebug:
            fprintf(stderr, RIPL_APPNAME " Debugging:\n");
            return vfprintf(stderr, format, ap);
        case itConsole:
        case itInfo:
        case itNone:
        default:
            return vprintf(format, ap);
    }
}

#endif
