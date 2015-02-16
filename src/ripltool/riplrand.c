/*
 *		riplrand.c
 *		Source file for Ripl Random-Number Generator Toolbox.
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
#include <math.h>
#include "riplrand.h"

#define IA			16807
#define IM			2147483647L
#define AM			(1.0/IM)
#define IQ			127773L
#define IR			2836
#define MASK		123459876L

/*
 * Park-Miller 'minimal-standard' random-number generator with seed value
 * 'idum' (must not equal MASK). Returns uniformly-distributed value between
 * 0.0 and 1.0 exclusive.
 *
 * Time: O(1).
 * See 'ran0' in 'Numerical Recipes in C', p279.
 */
float riplrandRan0(long *idum) {

	long k;
	float ans;

	*idum^=MASK;
	k=(*idum)/IQ;
	*idum=IA*(*idum-k*IQ)-IR*k;
	if (*idum<0) *idum+=IM;
	ans=AM*(*idum);
	*idum^=MASK;
	return ans;
}

#define NTAB		32
#define NDIV		(1+(IM-1)/NTAB)
#define EPS			1.2e-7
#define RNMX		(1.0-EPS)

/*
 * Park-Miller 'minimal-standard' random-number generator with Bays-Durham
 * shuffle and added safeguards. Seed value 'idum'. Set negative to initialize
 * thereafter do not alter between successive calls.
 *
 * Time: O(1.3).
 * See 'ran1' in 'Numerical Recipes in C', p280.
 */
float riplrandRan1(long *idum) {

	int j;
	long k;
	static long iy=0;
	static long iv[NTAB];
	float temp;

	if (*idum<=0 || !iy) {
		if (-(*idum)<1) *idum=1;
		else *idum=-(*idum);
		for (j=NTAB+7; j>=0; j--) {
			k=(*idum)/IQ;
			*idum=IA*(*idum-k*IQ)-IR*k;
			if (*idum<0) *idum+=IM;
			if (j<NTAB) iv[j]=*idum;
		}
		iy=iv[0];
	}
	k=(*idum)/IQ;
	*idum=IA*(*idum-k*IQ)-IR*k;
	if (*idum<0) *idum+=IM;
	j=(int)(iy/NDIV);
	iy=iv[j];
	iv[j]=*idum;
	if ((temp=AM*iy)>RNMX) return RNMX;
	else return temp;
}

/*
 * Generates positive unit-mean exponentially distributed pseudorandom numbers.
 *
 * See 'expdev' in 'Numerical Recipes in C', p287.
 */
float riplrandExp(long *idum) {

	float dum;

	do dum=riplrandRan1(idum);
	while (dum==0.0);
	return -log(dum);
}

/*
 * Generates zero-mean unit-variance normally distributed pseudorandom numbers.
 *
 * See 'gasdev' in 'Numerical Recipes in C', p289/290.
 */
float riplrandGauss(long *idum) {

	static unsigned iset=0;
	static float gset;
	float fac, rsq, v1, v2;

	if (iset==0) {
		do {
			v1=2.0*riplrandRan1(idum)-1.0;
			v2=2.0*riplrandRan1(idum)-1.0;
			rsq=v1*v1+v2*v2;
		} while (rsq>=1.0 || rsq==0.0);
		fac=sqrt(-2.0*log(rsq)/rsq);
		gset=v1*fac;
		iset=1;
		return v2*fac;
	}
	else {
		iset=0;
		return gset;
	}
}
