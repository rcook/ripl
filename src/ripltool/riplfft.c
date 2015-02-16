/*
 *		riplfft.c
 *		Source file for Ripl Fast Fourier Transform Toolbox.
 *
 *		Version 1.1, last update: 23 February 1998.
 *
 *		History:
 *			23/2/98:		changed copyright message.
 *			16/2/98:		misleading comment for 'riplfftReal2DFT' changed.
 *			31/1/98:		renamed mathematical constants.
 *			25/1/98:		created 'ripltdfs.h' and modified accordingly.
 *			23/1/98:		fixed it, I think.
 *			23/1/98:		added note at the bottom about some of it not working.
 *			23/1/98:		created first version of FFT Toolbox from 'miscfft.c'.
 *			21/1/98:		added more FFT stuff.
 *			21/1/98:		moved FFT functions from 'misc.c' to 'miscfft.c'.
 *
 *		Several functions based heavily on the specified functions
 *		published in 'Numerical Recipes in C' (2nd edition),
 *		by Press/Teukolsky/Vetterling/Flannery.
 *
 *		Fourier transform are calculated using a Cooley-Tukey decimation-in-time
 *		algorithm using a Danielson-Lanczos trigonometric recurrence in the
 *		manner described in Numerical Recipes. The routines have been modified
 *		to use zero-based vectors as default.
 *
 *		Parts copyright © 1998, Richard A. Cook.
 */
#include <math.h>
#include "riplfft.h"

/* Some stuff needed locally. */
#define SWAP(a, b)		tempr=(a); (a)=(b); (b)=tempr

/*
 * Takes a vector of length 'size' of float data points and converts it
 * into a complex vector of length 'size'*2.
 */
void riplfftCreateCmplxVctF(float *data,
	unsigned long size,
	float *cmplx_vct) {

	unsigned long i;

	RIPL_VALIDATE(data && cmplx_vct)

	for (i=size; i>0; i--) {
		*cmplx_vct++=*data++;
		*cmplx_vct++=0.0;
	}
}

/*
 * Takes a vector of length 'size' of grey levels and converts it
 * into a complex vector of length 'size'*2.
 */
void riplfftCreateCmplxVctG(riplGrey *data,
	unsigned long size,
	float *cmplx_vct) {

	unsigned long i;

	RIPL_VALIDATE(data && cmplx_vct)

	for (i=size; i>0; i--) {
		*cmplx_vct++=(float)(*data++);
		*cmplx_vct++=0.0;
	}
}

/*
 * Computes the complex discrete Fourier transform of the 'nn'-point
 * complex data vector stored in 'data'.
 *
 * See routine 'four1' on p507 of Numerical Recipes.
 *
 *		        N-1
 *		F(p) = sigma f(n)*exp(-j*2*pi/N*n*p)		f(n) complex
 *		        n=0
 *
 * Set 'type' to 'ttForward' to calculate the forward transform
 * or 'ttInverse' to calculate the inverse transform (multiply by 1/'nn'
 * using 'riplfftComplexScale' to obtain original scaling).
 */
void riplfftComplex1DFT(float *data,
	unsigned long nn,
	riplTransformType type) {

	unsigned long n, mmax, m, j, istep, i;
	double wtemp, wr, wpr, wpi, wi, theta;
	float tempr, tempi;

	RIPL_VALIDATE(data)
	RIPL_VALIDATE_IS_POWER_OF_2(nn)
	RIPL_VALIDATE(type==ttForward || type==ttInverse)

	n=nn<<1;
	j=1;
	for (i=1; i<n; i+=2) {
		if (j>i) {
			SWAP(data[j-1], data[i-1]);
			SWAP(data[j], data[i]);
		}
		m=n>>1;
		while (m>=2 && j>m) {
			j-=m;
			m>>=1;
		}
		j+=m;
	}
	mmax=2;
	while (n>mmax) {
		istep=mmax<<1;
		theta=type*(RIPL_M_TWICE_PI/mmax);
		wtemp=sin(0.5*theta);
		wpr=-2.0*wtemp*wtemp;
		wpi=sin(theta);
		wr=1.0;
		wi=0.0;
		for (m=1; m<mmax; m+=2) {
			for (i=m; i<=n; i+=istep) {
				j=i+mmax;
				tempr=wr*data[j-1]-wi*data[j];
				tempi=wr*data[j]+wi*data[j-1];
				data[j-1]=data[i-1]-tempr;
				data[j]=data[i]-tempi;
				data[i-1]+=tempr;
				data[i]+=tempi;
			}
			wtemp=wr;
			wr=wtemp*wpr-wi*wpi+wr;
			wi=wi*wpr+wtemp*wpi+wi;
		}
		mmax=istep;
	}
}

/*
 * Rescales a complex vector after inverse FFT.
 * Divides each element by the value 'scale'.
 */
void riplfftComplexScale(float *data,
	unsigned long size,
	unsigned long scale) {

	float *ptr;
	unsigned long i;

	RIPL_VALIDATE(data && scale>0)

	for (i=0, ptr=data; i<(size<<1); i++, ptr++) *ptr/=scale;
}

/*
 * Computes the discrete Fourier transform of the 'nn'-point real-valued
 * data vector stored in 'data'.
 *
 * See routine 'realft' on p513 of Numerical Recipes in C (2nd ed.).
 * As it happens a bug also appears in the function printed in the book
 * which is fixed in this implementation.
 *
 *		        N-1
 *		F(p) = sigma f(n)*exp(-j*2*pi/N*n*p)		f(n) real-valued
 *		        n=0
 *
 * The first two values output to 'data' are the real-valued first and
 * last frequency components (corresponding to d.c. and the Nyquist
 * critical frequency). The remaining values are the complex positive
 * frequency components.
 *
 * Set 'type' to 'ttForward' to calculate the forward transform
 * or 'ttInverse' to calculate the inverse transform (multiply by 2/'nn'
 * using 'riplfftRealScale' to obtain original scaling).
 */
void riplfftReal1DFT(float *data,
	unsigned long nn,
	riplTransformType type) {

	unsigned long i, i1, i2, i3, i4;
	float c1=0.5, c2, h1r, h1i, h2r, h2i;
	double wr, wi, wpr, wpi, wtemp, theta;

	RIPL_VALIDATE(data)
	RIPL_VALIDATE_IS_POWER_OF_2(nn)
	RIPL_VALIDATE(type==ttForward || type==ttInverse)

	theta=RIPL_M_PI/(double)(nn>>1);
	if (type==ttForward) {
		c2=-0.5;
		riplfftComplex1DFT(data, nn>>1, ttForward);
	}
	else {
		c2=0.5;
		theta=-theta;
	}
	wtemp=sin(0.5*theta);
	wpr=-2.0*wtemp*wtemp;
	wpi=sin(theta);
	wr=1.0+wpr;
	wi=wpi;
	/* 'i<=(nn>>2)+1' replaces the 'i<=(nn>>2)' found in the original. */
	for (i=2; i<=(nn>>2)+1; i++) {
		i1=(i<<1)-2;
		i2=i1+1;
		i3=nn+1-i2;
		i4=i3+1;
		h1r=c1*(data[i1]+data[i3]);
		h1i=c1*(data[i2]-data[i4]);
		h2r=-c2*(data[i2]+data[i4]);
		h2i=c2*(data[i1]-data[i3]);
		data[i1]=h1r+wr*h2r-wi*h2i;
		data[i2]=h1i+wr*h2i+wi*h2r;
		data[i3]=h1r-wr*h2r+wi*h2i;
		data[i4]=-h1i+wr*h2i+wi*h2r;
		wtemp=wr;
		wr=wtemp*wpr-wi*wpi+wr;
		wi=wi*wpr+wtemp*wpi+wi;
	}
	h1r=data[0];
	if (type==ttForward) {
		data[0]=h1r+data[1];
		data[1]=h1r-data[1];
	}
	else {
		data[0]=c1*(h1r+data[1]);
		data[1]=c1*(h1r-data[1]);
		riplfftComplex1DFT(data, nn>>1, ttInverse);
	}
}

/*
 * Rescales a complex vector after inverse FT or sine/cosine transform.
 * Divides each element by the value 'scale'.
 */
void riplfftRealScale(float *data,
	unsigned long size,
	unsigned long scale) {

	float *ptr;
	unsigned long i;

	RIPL_VALIDATE(data && scale>0)

	for (i=0, ptr=data; i<size; i++, ptr++) *ptr/=scale;
}

/*
 * Computes the discrete sine transform of the 'nn'-point real-valued
 * data vector stored in 'data'.
 *
 * See routine 'sinft' on p517 of Numerical Recipes in C (2nd ed.).
 *
 *		        N-1
 *		F(p) = sigma f(n)*sin(pi*n*p/N)				f(n) real-valued
 *		        n=1
 *
 * To calculate the inverse transform, sine-transform again and multiply
 * by 2/'nn' using 'riplfftRealScale' to obtain original scaling.
 */
void riplfftReal1DST(float *data,
	unsigned long nn) {

	unsigned long j, n2=nn+2;
	float sum, y1, y2;
	double theta, wi=0.0, wr=1.0, wpi, wpr, wtemp;

	RIPL_VALIDATE(data)
	RIPL_VALIDATE_IS_POWER_OF_2(nn)

	theta=RIPL_M_PI/(double)nn;
	wtemp=sin(0.5*theta);
	wpr=-2.0*wtemp*wtemp;
	wpi=sin(theta);
	data[0]=0.0;
	for (j=2; j<=(nn>>1)+1; j++) {
		wtemp=wr;
		wr=wtemp*wpr-wi*wpi+wr;
		wi=wi*wpr+wtemp*wpi+wi;
		y1=wi*(data[j-1]+data[n2-j-1]);
		y2=0.5*(data[j-1]-data[n2-j-1]);
		data[j-1]=y1+y2;
		data[n2-j-1]=y1-y2;
	}
	riplfftReal1DFT(data, nn, ttForward);
	data[0]*=0.5;
	sum=data[1]=0.0;
	for (j=1; j<=nn-1; j+=2) {
		sum+=data[j-1];
		data[j-1]=data[j];
		data[j]=sum;
	}
}

/*
 * Computes the discrete cosine transform of the 'nn'-point real-valued
 * data vector stored in 'data'.
 *
 * See routine 'cosft1' on p518 of Numerical Recipes.
 *
 * INPUT/OUTPUT VECTOR MUST BE OF LENGTH 'nn'+1.
 *
 *		                                 N-1
 *		F(p) = 0.5*(f(0)+((-1)^p)*f(N)+ sigma f(n)*cos(pi*n*p/N)
 *		                                 n=1
 *
 *	f(n) real-valued
 *
 * To calculate the inverse transform, cosine-transform again and multiply
 * by 2/'nn' using 'riplfftRealScale' to obtain original scaling.
 */
void riplfftReal1DCT1(float *data,
	unsigned long nn) {

	unsigned long j;
	float sum, y1, y2;
	double theta, wi=0.0, wr=1.0, wpi, wpr, wtemp;

	RIPL_VALIDATE(data)
	RIPL_VALIDATE_IS_POWER_OF_2(nn)

	theta=RIPL_M_PI/(double)nn;
	wtemp=sin(0.5*theta);
	wpr=-2.0*wtemp*wtemp;
	wpi=sin(theta);
	sum=0.5*(data[0]-data[nn]);
	data[0]=0.5*(data[0]+data[nn]);
	for (j=2; j<=(nn>>1); j++) {
		wtemp=wr;
		wr=wtemp*wpr-wi*wpi+wr;
		wi=wi*wpr+wtemp*wpi+wi;
		y1=0.5*(data[j-1]+data[nn-j+1]);
		y2=data[j-1]-data[nn-j+1];
		data[j-1]=y1-wi*y2;
		data[nn-j+1]=y1+wi*y2;
		sum+=wr*y2;
	}
	riplfftReal1DFT(data, nn, ttForward);
	data[nn]=data[1];
	data[1]=sum;
	for (j=4; j<=nn; j+=2) {
		sum+=data[j-1];
		data[j-1]=sum;
	}
}

/*
 * Computes the staggered discrete cosine transform of the 'nn'-point
 * real-valued data vector stored in 'data'.
 *
 * See routine 'cosft2' on p520 of Numerical Recipes.
 *
 *		        N-1
 *		F(p) = sigma f(n)*cos(pi*(n+0.5)*p/N)			f(n) real-valued
 *		        n=0
 *
 * With inverse:
 *
 *		                           N-1
 *		f(n) = (1/N)*F(0)+(2/N) * sigma F(p)*cos(pi*(n+0.5)*p/N)
 *		                           p=1
 *
 * Set 'type' to 'ttForward' to calculate the forward transform
 * or 'ttInverse' to calculate the inverse transform (multiply by 2/'nn'
 * using 'riplfftRealScale' to obtain original scaling).
 */
void riplfftReal1DCT2(float *data,
	unsigned long nn,
	riplTransformType type) {

	unsigned long i;
	float sum, sum1, y1, y2, ytemp;
	double theta, wi=0.0, wi1, wpi, wpr, wr=1.0, wr1, wtemp;

	RIPL_VALIDATE(data)
	RIPL_VALIDATE_IS_POWER_OF_2(nn)
	RIPL_VALIDATE(type==ttForward || type==ttInverse)

	theta=0.5*RIPL_M_PI/nn;
	wr1=cos(theta);
	wi1=sin(theta);
	wpr=-2.0*wi1*wi1;
	wpi=sin(2.0*theta);
	if (type==ttForward) {
		for (i=1; i<=(nn>>1); i++) {
			y1=0.5*(data[i-1]+data[nn-i]);
			y2=wi1*(data[i-1]-data[nn-i]);
			data[i-1]=y1+y2;
			data[nn-i]=y1-y2;
			wtemp=wr1;
			wr1=wtemp*wpr-wi1*wpi+wr1;
			wi1=wi1*wpr+wtemp*wpi+wi1;
		}
		riplfftReal1DFT(data, nn, ttForward);
		for (i=3; i<=nn; i+=2) {
			wtemp=wr;
			wr=wtemp*wpr-wi*wpi+wr;
			wi=wi*wpr+wtemp*wpi+wi;
			y1=data[i-1]*wr-data[i]*wi;
			y2=data[i-1]*wi+data[i]*wr;
			data[i-1]=y1;
			data[i]=y2;
		}
		sum=0.5*data[1];
		for (i=nn; i>=2; i-=2) {
			sum1=sum;
			sum+=data[i-1];
			data[i-1]=sum1;
		}
	}
	else {
		ytemp=data[nn-1];
		for (i=nn; i>=4; i-=2) data[i-1]=data[i-3]-data[i-1];
		data[1]=2.0*ytemp;
		for (i=3; i<=nn; i+=2) {
			wtemp=wr;
			wr=wtemp*wpr-wi*wpi+wr;
			wi=wi*wpr+wtemp*wpi+wi;
			y1=data[i-1]*wr+data[i]*wi;
			y2=data[i]*wr-data[i-1]*wi;
			data[i-1]=y1;
			data[i]=y2;
		}
		riplfftReal1DFT(data, nn, ttInverse);
		for (i=1; i<=(nn>>1); i++) {
			y1=data[i-1]+data[nn-i];
			y2=(0.5/wi1)*(data[i-1]-data[nn-i]);
			data[i-1]=0.5*(y1+y2);
			data[nn-i]=0.5*(y1-y2);
			wtemp=wr1;
			wr1=wtemp*wpr-wi1*wpi+wr1;
			wi1=wi1*wpr+wtemp*wpi+wi1;
		}
	}
}

/*
 * Computes the complex discrete Fourier transform of the 'nn'-point
 * 'dims'-dimensional complex data vector stored in 'data'.
 *
 * See routine 'fourn' on p523 of Numerical Recipes.
 *
 *		              N1-1      NM-1
 *		F(p1...pM) = sigma ... sigma f(n1...nM)*
 *		              n1=0      nM=0
 *		                  exp(-j*2*pi/(N1*...*NM)*(n1+...+nM)*(p1+...+pM)
 *
 *		For M-dimensional complex data input f(n1...nM).
 *
 * Set 'type' to 'ttForward' to calculate the forward transform
 * or 'ttInverse' to calculate the inverse transform (multiply by 1/nm,
 * where nm is the product of the lengths of each dimension, using
 * 'riplfftComplexScale' to obtain original scaling).
 */
void riplfftComplexNDFT(float *data,
	unsigned long *dim,
	unsigned dims,
	riplTransformType type) {

	unsigned idim;
	unsigned long i1, i2, i3, i2rev, i3rev, ip1, ip2, ip3, ifp1, ifp2;
	unsigned long ibit, k1, k2, n, nprev, nrem, ntot;
	float tempi, tempr;
	double theta, wi, wpi, wpr, wr, wtemp;

	RIPL_VALIDATE(data)
	RIPL_VALIDATE(type==ttForward || type==ttInverse)
	RIPL_DEBUG_STATEMENT(
		for (idim=0; idim<dims; idim++) RIPL_VALIDATE_IS_POWER_OF_2(dim[idim])
	)

	for (ntot=1, idim=1; idim<=dims; idim++) ntot*=dim[idim-1];
	nprev=1;
	for (idim=dims; idim>=1; idim--) {
		n=dim[idim-1];
		nrem=ntot/n/nprev;
		ip1=nprev<<1;
		ip2=ip1*n;
		ip3=ip2*nrem;
		i2rev=1;
		for (i2=1; i2<=ip2; i2+=ip1) {
			if (i2<i2rev) {
				for (i1=i2; i1<=i2+ip1-2; i1+=2) {
					for (i3=i1; i3<=ip3; i3+=ip2) {
						i3rev=i2rev+i3-i2;
						SWAP(data[i3-1], data[i3rev-1]);
						SWAP(data[i3], data[i3rev]);
					}
				}
			}
			ibit=ip2>>1;
			while (ibit>=ip1 && i2rev>ibit) {
				i2rev-=ibit;
				ibit>>=1;
			}
			i2rev+=ibit;
		}
		ifp1=ip1;
		while (ifp1<ip2) {
			ifp2=ifp1<<1;
			theta=type*RIPL_M_TWICE_PI/(ifp2/ip1);
			wtemp=sin(0.5*theta);
			wpr=-2.0*wtemp*wtemp;
			wpi=sin(theta);
			wr=1.0;
			wi=0.0;
			for (i3=1; i3<=ifp1; i3+=ip1) {
				for (i1=i3; i1<=i3+ip1-2; i1+=2) {
					for (i2=i1; i2<=ip3; i2+=ifp2) {
						k1=i2;
						k2=k1+ifp1;
						tempr=wr*data[k2-1]-wi*data[k2];
						tempi=wr*data[k2]+wi*data[k2-1];
						data[k2-1]=data[k1-1]-tempr;
						data[k2]=data[k1]-tempi;
						data[k1-1]+=tempr;
						data[k1]+=tempi;
					}
				}
				wtemp=wr;
				wr=wtemp*wpr-wi*wpi+wr;
				wi=wi*wpr+wtemp*wpi+wi;
			}
			ifp1=ifp2;
		}
		nprev*=n;
	}
}

/*
 * Computes the complex discrete Fourier transform of the 'nn'-point
 * 2-dimensional complex data vector stored in 'data'.
 *
 *		            N1-1  N2-1
 *		F(p1,p2) = sigma sigma f(n1,n2)*exp(-j*2*pi/(N1*N2)*(n1+n2)*(p1+p2)
 *		            n1=0  n2=0
 *
 *		For 2-dimensional complex data input f(n1,n2).
 *
 * Set 'type' to 'ttForward' to calculate the forward transform
 * or 'ttInverse' to calculate the inverse transform (multiply by
 * (1/'rows'/'cols') using 'riplfftComplexScale' to obtain original scaling).
 */
void riplfftComplex2DFT(float *data,
	unsigned long rows,
	unsigned long cols,
	riplTransformType type) {

	unsigned long dim[2];

	RIPL_VALIDATE(data)
	RIPL_VALIDATE_IS_POWER_OF_2(rows)
	RIPL_VALIDATE_IS_POWER_OF_2(cols)
	RIPL_VALIDATE(type==ttForward || type==ttInverse)

	dim[0]=rows;
	dim[1]=cols;
	riplfftComplexNDFT(data, dim, 2, type);
}

/*
 * Computes the complex discrete Fourier transform of the 'nn'-point
 * 2-dimensional complex data vector stored in 'data'.
 *
 * See routine 'rlft3' on p528 of Numerical Recipes.
 *
 *		            N1-1  N2-1
 *		F(p1,p2) = sigma sigma f(n1,n2)*exp(-j*2*pi/(N1*N2)*(n1+n2)*(p1+p2)
 *		            n1=0  n2=0
 *
 *		For 2-dimensional real data input f(n1,n2).
 *
 * Set 'type' to 'ttForward' to calculate the forward transform
 * or 'ttInverse' to calculate the inverse transform (multiply by
 * (1/'rows'/'cols') using 'riplfftComplexScale' to obtain original scaling).
 *
 * Output data is stored using following arrangement:
 *
 *		data					contains positive- and negative-frequency complex
 *								components of row-wise frequency and positive
 *								frequencies (not including Nyquist-frequency component)
 *								only in the column direction. Dimensions: 'm' rows by
 *								'n/2' columns (real and imaginary part per frequency).
 *		nyquist				contains the column-wise Nyquist critical frequency
 *								components. Dimensions: 'm'*2.
 */
void riplfftReal2DFT(float *data,
	float *nyquist,
	unsigned long rows,
	unsigned long cols,
	riplTransformType type) {

	unsigned long i2, i3, ii3;
	double theta, wi, wpi, wpr, wr, wtemp;
	float c1, c2, h1r, h1i, h2r, h2i;
	float *ptr1, *ptr2;

	RIPL_VALIDATE(data && nyquist)
	RIPL_VALIDATE_IS_POWER_OF_2(rows)
	RIPL_VALIDATE_IS_POWER_OF_2(cols)
	RIPL_VALIDATE(type==ttForward || type==ttInverse)

	c1=0.5;
	c2=-0.5*type;
	theta=type*(RIPL_M_TWICE_PI/cols);
	wtemp=sin(0.5*theta);
	wpr=-2.0*wtemp*wtemp;
	wpi=sin(theta);
	if (type==ttForward) {
		riplfftComplex2DFT(data, rows, cols>>1, ttForward);
		for (i2=0, ptr1=nyquist, ptr2=data; i2<rows; i2++, ptr1+=2, ptr2+=cols) {
			ptr1[0]=ptr2[0];
			ptr1[1]=ptr2[1];
		}
	}
	wr=1.0;
	wi=0.0;
	for (ii3=0, i3=1; i3<=(cols>>2)+1; i3++, ii3+=2) {
		ptr1=data;
		for (i2=0; i2<rows; i2++, ptr1+=cols) {
			if (i3==1) {
				/* Zero and Nyquist frequencies. */
				ptr2=nyquist+(i2 ? ((rows-i2-1)<<1)+2:0);
				h1r=c1*(ptr1[0]+ptr2[0]);
				h1i=c1*(ptr1[1]-ptr2[1]);
				h2i=c2*(ptr1[0]-ptr2[0]);
				h2r=-c2*(ptr1[1]+ptr2[1]);
				ptr1[0]=h1r+h2r;
				ptr1[1]=h1i+h2i;
				ptr2[0]=h1r-h2r;
				ptr2[1]=h2i-h1i;
			}
			else {
				/* The others. */
				ptr2=data+(i2 ? rows-i2:0)*cols+cols-(i3<<1)+2;
				h1r=c1*(ptr1[ii3]+ptr2[0]);
				h1i=c1*(ptr1[ii3+1]-ptr2[1]);
				h2i=c2*(ptr1[ii3]-ptr2[0]);
				h2r=-c2*(ptr1[ii3+1]+ptr2[1]);
				ptr1[ii3]=h1r+wr*h2r-wi*h2i;
				ptr1[ii3+1]=h1i+wr*h2i+wi*h2r;
				ptr2[0]=h1r-wr*h2r+wi*h2i;
				ptr2[1]=-h1i+wr*h2i+wi*h2r;
			}
		}
		wtemp=wr;
		wr=wtemp*wpr-wi*wpi+wr;
		wi=wi*wpr+wtemp*wpi+wi;
	}
	if (type==ttInverse) riplfftComplex2DFT(data, rows, cols>>1, ttInverse);
}

/*
 * Takes a partial spectrum of real-valued 2D spatial series (typically
 * produced by 'riplfftReal2DFT' and generates the full 2D spectrum
 * compatible with 'riplfftComplex2DFT'.
 *
 * The partial spectrum must correspond to a 2D series of dimensions
 * 'rows' by 'cols'. The output 'spec' must point to a block of
 * size 'rows'*'cols'*2.
 */
void riplfftGenFullSpectrum(float *data,
	float *nyquist,
	unsigned long rows,
	unsigned long cols,
	float *spec) {

	unsigned long i, j, k;
	float *ptr, *outp1, *outp2;

	RIPL_VALIDATE(data && nyquist && spec)
	RIPL_VALIDATE_IS_POWER_OF_2(rows)
	RIPL_VALIDATE_IS_POWER_OF_2(cols)

	ptr=data;
	outp1=spec;
	outp2=spec+rows*cols;
	for (i=0; outp2>=outp1; i++) {
		if (i==0 || outp2==outp1) {
			outp1[0]=ptr[0];
			outp1[1]=ptr[1];
			for (j=cols-1, k=1; j>k; j--, k++) {
				outp1[j<<1]=outp1[k<<1]=ptr[k<<1];
				outp1[(j<<1)+1]=outp1[(k<<1)+1]=ptr[(k<<1)+1];
			}
			outp1[j<<1]=nyquist[i<<1];
			outp1[(j<<1)+1]=nyquist[(i<<1)+1];
		}
		else {
			outp1[0]=outp2[0]=ptr[0];
			outp1[1]=outp2[1]=ptr[1];
			for (j=cols-1, k=1; j>k; j--, k++) {
				outp2[j<<1]=outp1[k<<1]=ptr[k<<1];
				outp2[(j<<1)+1]=outp1[(k<<1)+1]=ptr[(k<<1)+1];
			}
			outp2[j<<1]=outp1[k<<1]=nyquist[i<<1];
			outp2[(j<<1)+1]=outp1[(k<<1)+1]=nyquist[(i<<1)+1];
		}
		outp1+=(cols<<1);
		outp2-=(cols<<1);
		ptr+=cols;
	}
}

