/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplcmd.c
 *		This is the main entrypoint for ripl.
 *
 *		Version 1.1, last update: 20 January 1998.
 *
 *		History:
 *			20/1/98:		version 1.1 (renamed from main.c).
 *			27/11/97:	first implemented.
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include "ripl.h"

int main(int argc, char **argv) {
	return riplMain1(argc-1, argv+1);
}
