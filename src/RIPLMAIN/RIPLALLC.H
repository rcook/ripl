/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplallc.h
 *		Header file declaring memory-allocation macros.
 *
 *		Version 1.1, last update: 17 May 1998.
 *
 *		History:
 *			17/5/98:	scrapped allocation macros.
 *			20/1/98:		version 1.1.
 *			27/11/97:	first version.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#ifndef _RIPLALLC_H_INCLUDED
#define _RIPLALLC_H_INCLUDED

#include <stdlib.h>

/* Implement Ripl memory allocators as functions. */
void *riplMalloc(size_t size);
void *riplCalloc(size_t n, size_t size);
void *riplRealloc(void *block, size_t size);
void riplFree(void *block);

#endif
