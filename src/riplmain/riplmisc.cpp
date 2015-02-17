/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplmisc.c
 *		Source file defining some useful general-purpose functions.
 *
 *		Version 1.1, last update: 24 January 1998.
 *
 *		History:
 *			24/1/98:		fixed bug in riplParseResponseFile.
 *			20/1/98:		version 1.1.
 *			27/11/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "riplmisc.h"
#include "ripldbug.h"
#include "riplallc.h"

/* Constants for local use. */
#define ARG_BUFFER_SIZE		256
#define RESP_LINE_SIZE		100

/* Read command-line arguments from the specified response file. */
char **riplParseResponseFile(const char *pfileName,
    unsigned *pargc) {
    FILE *pfile;
    static char arg_buffer[ARG_BUFFER_SIZE];
    char line_buffer[RESP_LINE_SIZE];
    unsigned i, remain=ARG_BUFFER_SIZE, count=0;
    char *line_ptr, *arg_ptr=arg_buffer, **pargv;

    RIPL_VALIDATE(pfileName)
    RIPL_VALIDATE(pargc)

    /* Open response file and read it in line by line. */
    pfile=fopen(pfileName, "rt");
    RIPL_VALIDATE(pfile)
    while (!feof(pfile)) {
        memset(line_buffer, 0, RESP_LINE_SIZE);
        if (!fgets(line_buffer, RESP_LINE_SIZE, pfile)) break;
        line_ptr=strtok(line_buffer, " \t\n\r");
        while (line_ptr) {
            i=strlen(line_ptr);
            if (i+1>remain) {
                fclose(pfile);
                return NULL;
            }
            strcpy(arg_ptr, line_ptr);
            arg_ptr+=i+1;
            remain-=i+1;
            line_ptr=strtok(NULL, " \t\n\r");
            count++;
        }
    }
    if (ferror(pfile)) {
        fclose(pfile);
        return NULL;
    }
    fclose(pfile);
    *pargc=count;
    pargv=(char **)riplCalloc(count, sizeof(char *));
    RIPL_VALIDATE(pargv)
    for (arg_ptr=arg_buffer, i=0; i<count; i++) {
        pargv[i]=arg_ptr;
        arg_ptr=strchr(arg_ptr, 0)+1;
    }
    return pargv;
}

/* Return 'true' if specified file exists. */
bool riplFileExists(const char *pfileName) {

    FILE *pfile;

    RIPL_VALIDATE(pfileName)

    pfile=fopen(pfileName, "r");
    if (pfile) {
        fclose(pfile);
        return true;
    }
    return false;
}

/* Round double value to nearest integer. */
int riplRound(double val) {

    int ival=val+1;

    if (val-ival+1<ival-val) return ival-1;
    return ival;
}

#ifndef HAVE_STRLWR
/* Convert specified zero-terminated string to lower-case. */
char *strlwr(char *s) {

    char *ptr=s;

    while (*ptr) {
        if (isupper(*ptr)) *ptr=(char)tolower(*ptr);
        ptr++;
    }
    return s;
}
#endif

#ifndef HAVE_STRUPR
/* Convert specified zero-terminated string to upper-case. */
char *strupr(char *s) {

    char *ptr=s;

    while (*ptr) {
        if (islower(*ptr)) *ptr=(char)toupper(*ptr);
        ptr++;
    }
    return s;
}
#endif

#ifndef HAVE_STRDUP
/* Duplicate specified string. */
char *strdup(const char *s) {

    char *new_s=(char *)riplCalloc(strlen(s)+1, sizeof(char));
    
    return new_s ? strcpy(new_s, s):NULL;
}
#endif

