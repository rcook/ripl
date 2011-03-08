/*
 *		RIPL---Richard's Image-Processing Library.
 *		Tcl/Tk Dynamic-Link Library Version.
 *		Written by Richard Cook.
 *
 *		tcldll.h
 *		Main header file.
 *
 *		Version 1.1, last update: 20 January 1998.
 *
 *		History:
 *			20/1/98:	modified to version 1.1.
 *			26/11/97:	first version.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _TCLDLL_H_INCLUDED
#define _TCLDLL_H_INCLUDED

#ifdef __WATCOMC__
	/* Bug fixes for Watcom compiler. */
#	pragma aux abort "*_";
#	pragma aux riplCalloc "*_";
#	pragma aux riplFree "*_";
#	pragma aux riplMain2 "*_";
#	pragma aux riplMalloc "*_";
#	pragma aux riplMatch "*_";
#	pragma aux riplMessage "*_";
#	pragma aux riplRealloc "*_";
#	pragma aux (cdecl) default;
#endif

#include "tcl.h"
#define JASHE_TK
#include "jashe.h"

/* Client data structure */
typedef struct tagripldllData {
	char *jashe_name;
	Tcl_CmdInfo jashe;
} ripldllData;

int Ripldll_Init(Tcl_Interp *interp);

void *riplMalloc(size_t size);
void *riplCalloc(size_t n, size_t size);
void *riplRealloc(void *block, size_t size);
void riplFree(void *block);

#endif
