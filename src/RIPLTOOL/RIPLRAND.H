/*
 *		riplrand.h
 *		Header file for Ripl Random-Number Generator Toolbox.
 *
 *		Version 1.1, last update: 23 February 1998.
 *
 *		History:
 *			23/2/98:		first implemented.
 *
 *		Several functions based heavily on the specified functions
 *		published in 'Numerical Recipes in C' (2nd edition),
 *		by Press/Teukolsky/Vetterling/Flannery.
 *
 *		Parts copyright © 1998, Richard A. Cook.
 */
#ifndef _RIPLRAND_H_INCLUDED
#define _RIPLRAND_H_INCLUDED

float riplrandRan0(long *idum);
float riplrandRan1(long *idum);
float riplrandExp(long *idum);
float riplrandGauss(long *idum);

#endif
