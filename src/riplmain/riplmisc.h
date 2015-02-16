/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplmisc.h
 *		Header file declaring general-purpose data types and functions.
 *
 *		Version 1.1, last update: 10 February  1998.
 *
 *		History:
 *			10/2/98:		increased maximum image size.
 *			4/2/98:			modified 'riplGraphicFormat'.
 *			4/2/98:			added 'riplByte' data type.
 *			24/1/98:		renamed RIPL_PARSEERROR to RIPL_PARAMERROR.
 *			23/1/98:		added new RIPL_USERERROR return code.
 *			20/1/98:		version 1.1.
 *			27/11/97:	first version.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _RIPLMISC_H_INCLUDED
#define _RIPLMISC_H_INCLUDED

#include "ripldefs.h"

typedef unsigned char riplGrey;
typedef unsigned char riplByte;
typedef unsigned short riplMidGrey;
typedef unsigned long riplBigGrey;
typedef signed short riplMidGreyS;
typedef signed long riplBigGreyS;

typedef enum tagriplBool {
	false=0,
	true=1
} riplBool;

#define RIPL_MAX_COLS			4096
#define RIPL_MAX_ROWS			4096
#define RIPL_RED_WEIGHT			0.3
#define RIPL_GREEN_WEIGHT		0.59
#define RIPL_BLUE_WEIGHT		0.11
#define RIPL_GREY_LEVELS		256
/* It doesn't make sense to define the following any other way. */
#define RIPL_MAX_GREY			(RIPL_GREY_LEVELS-1)
/* Return codes. */
#define RIPL_FIRSTERRORCODE	-1
#define RIPL_PARAMERROR			(RIPL_FIRSTERRORCODE)
#define RIPL_EXECUTEERROR		(RIPL_FIRSTERRORCODE-1)
#define RIPL_USERERROR			(RIPL_FIRSTERRORCODE-2)

typedef enum tagriplGraphicFormat {
	gfInvalid=0,
	gfPBMASCII,
	gfPBMBinary,
	gfPGMASCII,
	gfPGMBinary,
	gfPPMASCII,
	gfPPMBinary,
	gfPCXAny,
	gfPCXBasic,
	gfPCXExtended
} riplGraphicFormat;

char **riplParseResponseFile(const char *pfileName,
	unsigned *pargc);
riplBool riplFileExists(const char *pfileName);
int riplRound(double val);

/* Non-ANSI string functions. */
char *strlwr(char *s);
char *strupr(char *s);
char *strdup(const char *s);

#endif
