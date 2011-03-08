/*
 *		riplwave.c
 *		Source file for Ripl Wavelet Transform Toolbox.
 *
 *		Version 1.1, last update: 23 February 1998.
 *
 *		History:
 *			23/2/98:		added some more filter types from Lewis95.
 *			23/2/98:		added some more Daubechies coefficients.
 *			23/2/98:		revised copyright message.
 *			31/1/98:		rewrote riplwtPartialWT and a few other bits.
 *			30/1/98:		added Haar wavelet.
 *			30/1/98:		slight modifications.
 *			25/1/98:		first implemented.
 *
 *		Several functions based loosely on the specified functions
 *		published in Press92.
 *
 *		Coefficients from Press92, Chui97 and Lewis95.
 *
 *		Parts copyright © 1998, Richard A. Cook.
 */
#include "riplwave.h"

/*
 * Applies a one-dimensional discrete wavelet transform to the
 * 'n'-point data set 'data'. A pointer to a function implementing
 * the underlying wavelet filter 'pfiltFunc' must be supplied. 'arg' is
 * a pointer to any arbitrary object required as an argument to 'pfiltFunc'.
 *
 * Setting 'type' to 'ttForward' will apply the forward transform
 * while 'ttInverse' will perform the inverse operation.
 *
 * If NULL is specified for 'pfiltFunc' then 'riplwtPartialWT' is used by
 * default.
 */
void riplwt1DWT(float *data,
	unsigned long n,
	riplTransformType type,
	riplwtFiltFunc pfiltFunc,
	void *arg) {

	unsigned long i;

	RIPL_VALIDATE(data)
	RIPL_VALIDATE_IS_POWER_OF_2(n)
	RIPL_VALIDATE(type==ttForward || type==ttInverse)

	if (!pfiltFunc) pfiltFunc=riplwtFilterFunc;
	if (n<4) return;
	if (type==ttForward)
		for (i=n; i>=4; i>>=1) pfiltFunc(data, i, type, arg);
	else for (i=4; i<=n; i<<=1) pfiltFunc(data, i, type, arg);
}

/*
 * Applies an 'ndim'-dimensional discrete wavelet transform to the data
 * set 'data' whose dimensions are passed in 'nn'. A pointer to a function
 * implementing the underlying wavelet filter 'pfiltFunc' must be supplied.
 * 'arg' is a pointer to any arbitrary object required as an argument to
 * 'pfiltFunc'.
 *
 * Setting 'type' to 'ttForward' will apply the forward transform
 * while 'ttInverse' will perform the inverse operation.
 *
 * If NULL is specified for 'pfiltFunc' then 'riplwtPartialWT' is used by
 * default.
 */
void riplwtNDWT(float *data,
	unsigned long *nn,
	unsigned ndim,
	riplTransformType type,
	riplwtFiltFunc pfiltFunc,
	void *arg) {

	unsigned long i1, i2, i3, k, n, nnew, nprev, nt, ntot;
	unsigned idim;
	float *wksp;

	RIPL_VALIDATE(data && nn)
	RIPL_VALIDATE(type==ttForward || type==ttInverse)
	RIPL_DEBUG_STATEMENT(
		for (idim=0; idim<ndim; idim++) RIPL_VALIDATE_IS_POWER_OF_2(nn[idim])
	)

	if (!pfiltFunc) pfiltFunc=riplwtFilterFunc;
	for (idim=0, ntot=1; idim<ndim; idim++) ntot*=nn[idim];
	wksp=(float *)riplCalloc(ntot, sizeof(float));
	RIPL_VALIDATE(wksp)
	nprev=1;
	for (idim=0; idim<ndim; idim++) {
		n=nn[idim];
		nnew=n*nprev;
		if (n>4) {
			for (i2=0; i2<ntot; i2+=nnew) {
				for (i1=0; i1<nprev; i1++) {
					for (k=0, i3=i1+i2; k<n; k++, i3+=nprev) wksp[k]=data[i3];
					if (type==ttForward) {
						for (nt=n; nt>=4; nt>>=1) pfiltFunc(wksp, nt, type, arg);
					}
					else {
						for (nt=4; nt<=n; nt<<=1) pfiltFunc(wksp, nt, type, arg);
					}
					for (k=0, i3=i1+i2; k<n; k++, i3+=nprev) data[i3]=wksp[k];
				}
			}
		}
		nprev=nnew;
	}
	riplFree(wksp);
}

/*
 * Applies a 2-dimensional discrete wavelet transform to the data set
 * 'data' of dimensions 'rows' by 'cols'. A pointer to a function
 * implementing the underlying wavelet filter 'pfiltFunc' must be supplied.
 * 'arg' is a pointer to any arbitrary object required as an argument to
 * 'pfiltFunc'.
 *
 * Setting 'type' to 'ttForward' will apply the forward transform
 * while 'ttInverse' will perform the inverse operation.
 *
 * If NULL is specified for 'pfiltFunc' then 'riplwtPartialWT' is used by
 * default.
 */
void riplwt2DWT(float *data,
	unsigned long rows,
	unsigned long cols,
	riplTransformType type,
	riplwtFiltFunc pfiltFunc,
	void *arg) {

	unsigned long nn[2];

	RIPL_VALIDATE(data)
	RIPL_VALIDATE_IS_POWER_OF_2(rows)
	RIPL_VALIDATE_IS_POWER_OF_2(cols)
	RIPL_VALIDATE(type==ttForward || type==ttInverse)

	if (!pfiltFunc) pfiltFunc=riplwtFilterFunc;
	nn[0]=rows;
	nn[1]=cols;
	riplwtNDWT(data, nn, 2, type, pfiltFunc, arg);
}

/*
 * General-purpose partial wavelet filter function. Performs filtering
 * using the coefficients provided in the 'wfilt' structure. Forward or
 * inverse filtering is specified by 'type' while the 'n'-point data
 * is pointed to by 'data'.
 */
void riplwtFilterFunc(float *data,
	unsigned long len,
	riplTransformType type,
	void *arg) {

	float *temp;
	unsigned long half=len>>1, i1, i2, i3, i4, i5;
	const riplwtFilter *pfilt=(riplwtFilter *)arg;

	RIPL_VALIDATE(data)
	RIPL_VALIDATE_IS_POWER_OF_2(len)
	RIPL_VALIDATE(type==ttForward || type==ttInverse)
	RIPL_VALIDATE(pfilt)

	if (len<4) return;

	/* Allocate and initialize temporary vector. */
	temp=(float *)riplCalloc(len, sizeof(float));
	RIPL_VALIDATE(temp)
	for (i1=0; i1<len; i1++) temp[i1]=0.0;

	if (type==ttForward) {
		/* Forward transform---perform permuted convolution. */
		for (i1=0, i2=0; i1<len; i1+=2, i2++) {
			for (i3=0, i4=pfilt->ncoeffs-1; i3<pfilt->ncoeffs; i3++, i4--) {
				i5=(i1+i3)%len;
				temp[i2]+=pfilt->coeff[i3]*data[i5];
				if (i3%2) temp[i2+half]-=pfilt->coeff[i4]*data[i5];
				else temp[i2+half]+=pfilt->coeff[i4]*data[i5];
			}
		}
	}
	else {
		/* Inverse transform---perform permuted transposed convolution. */
		for (i1=0, i2=0; i1<len; i1+=2, i2++) {
			for (i3=0, i4=pfilt->ncoeffs-1; i3<pfilt->ncoeffs; i3++, i4--) {
				i5=(i1+i3)%len;
				temp[i5]+=pfilt->coeff[i3]*data[i2];
				if (i3%2) temp[i5]-=pfilt->coeff[i4]*data[i2+half];
				else temp[i5]+=pfilt->coeff[i4]*data[i2+half];
			}
		}
	}
	for (i1=0; i1<len; i1++) data[i1]=temp[i1];
	riplFree(temp);
}

/*
 * Sets up a Daubechies 4-, 12- or 20-coefficient or Haar wavelet filter by
 * filling the 'riplwtFilter' structure pointed to by 'wfilt'. This object
 * should be passed as the 'arg' argument to the 'riplwtPartialWT' wavelet
 * transform function to apply the transform.
 */
#ifndef SQRT15
#	define SQRT15		3.87298334620741688517927
#endif
void riplwtSetupFilter(riplwtFilterType coeffs,
	riplwtFilter *pfilt) {

	static float
		battle_lemarie24[24]={
			RIPL_M_SQRT2*-0.002, RIPL_M_SQRT2*-0.003, RIPL_M_SQRT2*0.006,
			RIPL_M_SQRT2*0.006, RIPL_M_SQRT2*-0.013, RIPL_M_SQRT2*-0.012,
			RIPL_M_SQRT2*0.030, RIPL_M_SQRT2*0.023, RIPL_M_SQRT2*-0.078,
			RIPL_M_SQRT2*-0.035, RIPL_M_SQRT2*0.307, RIPL_M_SQRT2*0.542,
			RIPL_M_SQRT2*0.307, RIPL_M_SQRT2*-0.035, RIPL_M_SQRT2*-0.078,
			RIPL_M_SQRT2*0.023, RIPL_M_SQRT2*0.030, RIPL_M_SQRT2*-0.012,
			RIPL_M_SQRT2*-0.013, RIPL_M_SQRT2*0.006, RIPL_M_SQRT2*0.006,
			RIPL_M_SQRT2*-0.003, RIPL_M_SQRT2*-0.002, 0.0},
		coiflet6[6]={
			RIPL_M_SQRT2*(SQRT15-3)/32.0, RIPL_M_SQRT2*(1-SQRT15)/32.0,
			RIPL_M_SQRT2*(6-2*SQRT15)/32.0, RIPL_M_SQRT2*(2*SQRT15+6)/32.0,
			RIPL_M_SQRT2*(SQRT15+13)/32.0, RIPL_M_SQRT2*(9-SQRT15)/32.0},
		coiflet12[12]={
			0.0011945726958388, -0.01284557955324, 0.024804330519353,
			0.050023519962135, -0.15535722285996, -0.071638282295294,
			0.57046500145033, 0.75033630585287, 0.28061165190244,
			-0.0074103835186718, -0.014611552521451, -0.0013587990591632},
		coiflet18[18]={
			-0.0016918510194918, -0.00348787621998426, 0.019191160680044,
			0.021671094636352, -0.098507213321468, -0.056997424478478,
			0.45678712217269, 0.78931940900416, 0.38055713085151,
			-0.070438748794943, -0.056514193868065, 0.036409962612716,
			0.0087601307091635, -0.011194759273835, -0.0019213354141368,
			0.0020413809772660, 0.00044583039753204, -0.00021625727664696},
		daub4[4]={
			0.4829629131445341, 0.8365163037378079,
			0.2241438680420134, -0.1294095225512604},
		daub6[6]={
			0.332670552950, 0.806891509311, 0.459877502118,
			-0.135011020010, -0.085441273882, 0.035226291882},
		daub8[8]={
			0.230377813309, 0.714846570553, 0.6308807667930,
			-0.027983769417, -0.187034811719, 0.030841381836,
			0.032883011667, -0.010597401785},
		daub10[10]={
			0.1601023979741929, 0.6038292697971895, 0.7243085284377726,
			0.1384281459013203, -0.2422948870663823, -0.0322448695846381,
			0.0775714938400459, -0.0062414902127983, -0.0125807519990820,
			0.0033357252854738},
		daub12[12]={
			0.1115407433501095, 0.4946238903984533, 0.7511339080210959,
			0.3152503517091982, -0.2262646939654400, -0.1297668675672625,
			0.0975016055873225, 0.0275228655303053, -0.0315820393184862,
			0.0005538422011614, 0.0047772575119455, -0.0010773010853085},
		daub14[14]={
			0.077852054085, 0.396539319482, 0.729132090846, 0.469782287405,
			-0.143906003929, -0.224036184994, 0.071309219267, 0.080612609151,
			-0.038029936935, -0.016574541631, 0.012550998556, 0.000429577973,
			-0.001801640704, 0.000353713800},
		daub16[16]={
			0.054415842243, 0.312871590914, 0.675630736297, 0.585354683654,
			-0.015829105256, -0.284015542962, 0.000472484574, 0.128747426620,
			-0.017369301002, -0.044088258931, 0.018981027917, 0.008746094047,
			-0.004870352993, -0.000391740373, 0.000675449406, -0.000117476784},
		daub18[18]={
			0.038077947364, 0.243834674613, 0.604823123690, 0.657288078051,
			0.133197385825, -0.293273783279, -0.096840783223, 0.148540749338,
			0.030725681479, -0.067632829061, 0.000250947115, 0.022361662124,
			-0.004723204758, -0.004281503682, 0.001847646883, 0.000230385764,
			-0.000251963189, 0.000039347320},
		daub20[20]={
			0.026670057901, 0.188176800078, 0.527201188932, 0.688459039454,
			0.281172343661, -0.249846424327, -0.195946274377, 0.127369340336,
			0.093057364604, -0.071394147166, -0.029457536822, 0.033212674059,
			0.003606553567, -0.010733175483, 0.001395351747, 0.001992405295,
			-0.000685856695, -0.000116466855, 0.000093588670, -0.000013264203},
		haar2[2]={
			RIPL_M_SQRT_2, RIPL_M_SQRT_2};

	RIPL_VALIDATE(pfilt)

	switch (coeffs) {
	case ftBattleLemarie24:
		pfilt->ncoeffs=24;
		pfilt->coeff=battle_lemarie24;
		break;
	case ftCoiflet6:
		pfilt->ncoeffs=6;
		pfilt->coeff=coiflet6;
		break;
	case ftCoiflet12:
		pfilt->ncoeffs=12;
		pfilt->coeff=coiflet12;
		break;
	case ftCoiflet18:
		pfilt->ncoeffs=18;
		pfilt->coeff=coiflet18;
		break;
	case ftDaub4:
		pfilt->ncoeffs=4;
		pfilt->coeff=daub4;
		break;
	case ftDaub6:
		pfilt->ncoeffs=6;
		pfilt->coeff=daub6;
		break;
	case ftDaub8:
		pfilt->ncoeffs=8;
		pfilt->coeff=daub8;
		break;
	case ftDaub10:
		pfilt->ncoeffs=10;
		pfilt->coeff=daub10;
		break;
	case ftDaub12:
		pfilt->ncoeffs=12;
		pfilt->coeff=daub12;
		break;
	case ftDaub14:
		pfilt->ncoeffs=14;
		pfilt->coeff=daub14;
		break;
	case ftDaub16:
		pfilt->ncoeffs=16;
		pfilt->coeff=daub16;
		break;
	case ftDaub18:
		pfilt->ncoeffs=18;
		pfilt->coeff=daub18;
		break;
	case ftDaub20:
		pfilt->ncoeffs=20;
		pfilt->coeff=daub20;
		break;
	case ftHaar2:
		pfilt->ncoeffs=2;
		pfilt->coeff=haar2;
		break;
	default:
		RIPL_VALIDATE(coeffs<=ftInvalid || coeffs>=ftAlsoInvalid)
		return;
	}
}

