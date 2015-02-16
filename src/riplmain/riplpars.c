/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplpars.c
 *		Source file for parsing and string matching stuff.
 *
 *		Version 1.1, last update: 20 January 1998.
 *
 *		History:
 *			20/1/98:		version 1.1.
 *			20/1/98:		first implemented
 *							(moved from riplop to separate file).
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include "riplpars.h"
#include "ripldbug.h"

/*
 * Get a value of type 'riplBool' from command line.
 * Text must be either 0/1 or TRUE/FALSE (case insensitive).
 */
riplBool riplArgGet_riplBool(const char *text,
	riplBool *value) {

	static char buffer[6];

	RIPL_VALIDATE(text && value)

	strncpy(buffer, text, 6);
	strlwr(buffer);
	if (!strcmp(buffer, "0") || !strcmp(buffer, "false")) {
		*value=false;
		return true;
	}
	if (!strcmp(buffer, "1") || !strcmp(buffer, "true")) {
		*value=true;
		return true;
	}
	return false;
}

/*
 * Get a value of type 'riplGrey' from command line.
 * Must have a value in range 0 to RIPL_MAX_GREY.
 */
riplBool riplArgGet_riplGrey(const char *text,
	riplGrey *value) {

	char *endptr;
	unsigned long val;

	RIPL_VALIDATE(text && value)

	val=strtoul(text, &endptr, 0);
	if (*endptr) return false;
	if (val>RIPL_MAX_GREY) return false;
	*value=(riplGrey)val;
	return true;
}

/*
 * Get a value of type 'unsigned' from command line.
 * Must have a value in range 0 to UINT_MAX.
 */
riplBool riplArgGet_unsigned(const char *text,
	unsigned *value) {

	char *endptr;
	unsigned long val;

	RIPL_VALIDATE(text && value)

	val=strtoul(text, &endptr, 0);
	if (*endptr) return false;
#ifndef __WATCOMC__
	if (val>UINT_MAX) return false;
#endif
	*value=(unsigned)val;
	return true;
}

/*
 * Get a value of type 'int' from command line.
 * Must have a value in range INT_MIN to INT_MAX.
 */
riplBool riplArgGet_int(const char *text,
	int *value) {

	char *endptr;
	long val;

	RIPL_VALIDATE(text && value)

	val=strtol(text, &endptr, 0);
	if (*endptr) return false;
#ifndef __WATCOMC__
	if (val<INT_MIN || val>INT_MAX) return false;
#endif
	*value=(int)val;
	return true;
}

/*
 * Get a value of type 'long' from command line.
 * Must not under- or overflow 32-bit integer data type.
 */
riplBool riplArgGet_long(const char *text,
	long *value) {

	char *endptr;
	long val;

	RIPL_VALIDATE(text && value)

	val=strtol(text, &endptr, 0);
	if (*endptr) return false;
	*value=val;
	return true;
}

/*
 * Get a value of type 'float' from command line.
 * Must be within standard float range.
 */
riplBool riplArgGet_float(const char *text,
	float *value) {
	
	char *endptr;
	double val;

	RIPL_VALIDATE(text && value)

	val=strtod(text, &endptr);
	if (*endptr) return false;
	*value=(float)val;
	return true;
}

/*
 * Get a value of type 'double' from command line.
 * Must be within standard double range.
 */
riplBool riplArgGet_double(const char *text,
	double *value) {
	
	char *endptr;
	double val;

	RIPL_VALIDATE(text && value)

	val=strtod(text, &endptr);
	if (*endptr) return false;
	*value=val;
	return true;
}

/* Match a string against another up to specified number of characters. */
riplBool riplMatch(const char *input,
	const char *ref,
	unsigned match_chars) {

	RIPL_VALIDATE(input && ref)

	if (strlen(input)<match_chars) return false;
	while (*input) {
		if (tolower(*input)!=*ref) return false;
		input++;
		ref++;
	}
	return true;
}
