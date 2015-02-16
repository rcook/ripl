/*
 *		ahe.c
 *		Source file for adaptive histogram equalization based on JASHE.
 *
 *		Version 1.1, last update: 3 May 1998.
 *
 *		History:
 *			3/5/98:		minor bug fix (hamming option not correctly parsed)
 *			31/1/98:		mathematical constants renamed.
 *			24/1/98:		introduced RIPL_PARAMERROR.
 *			23/1/98:		minor modification: M_PI --> RIPL_PI etc.
 *			24/12/97:	updated on-line info.
 *			16/12/97:	updated comments.
 *			15/12/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include <string.h>
#include <math.h>
#include <float.h>
#include "ahe.h"

/* Local defines, enums and structs. */
#define BSHIFT								(BIG_TMP_BITS-HLB)
#define NOT_FILTVAL						(ldexp(255.0, HLB-8-1))
#define AFT_FILTVAL						(ldexp(255.0, BIG_TMP_BITS-8))
#define BIG_TMP_BITS						(31)
#define HLB									(15)
#define OUT_BITS							(8)
#define ACC_BITS							(14)
#define MAX_BIT_SHIFT					(32)
#define MAX_TERMS							(200)
enum tagterm_type {
	ttSin,
	ttCos
};
struct taghe_series_data {
	float factor;
	float factorX;		/* 2-he_fraction. */
	float gammaVal;	/* gamma(heFactorX). */
	float twoPower;	/* 2^heFactorX. */
	float sinVal;		/* sin( heFactorX*pi/2). */
	float baseMag;		/* (1st coeff)/heTwoPower. */
};

/* Prototypes of static functions. */
static void initialize_series(const aheConfig *pconfig);
static void he_series_init(struct taghe_series_data *pdata,
	float factor);
static float he_series_value(struct taghe_series_data *pdata,
	unsigned i);
static void set_tables(const aheConfig *pconfig,
	unsigned term_num,
	enum tagterm_type tt);
static void accumulate(void);
static void filter(riplGrey *inImgPtr,
	riplBigGreyS *outImgPtr,
	long *lTable,
	unsigned cols,
	unsigned rows,
	unsigned width,
	unsigned height);
static int parse_arguments(aheConfig *pconfig,
	unsigned argc,
	const char **argv);

/* Static data object. */
static struct {
	riplGreyMap *pinput_greymap;
	riplGreyMap *poutput_greymap;
	riplMidGreyS *pmid_image;
	riplBigGreyS *pbig_image;
	long *not_filt;
	long *filt;
	float *he_series, *cum_series, *addback_series;
	float he_fraction;

	int filtShifts, notFiltShifts;
	float filtFactor, notFiltFactor;
	int accShift;
} data;

/* Internal entrypoint. */
riplBool aheApplyOperator(riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap,
	const aheConfig *pconfig) {

	unsigned i;
	riplMidGreyS temp, offset;
	riplMidGreyS *accP;
	riplGrey *outP;
	unsigned r, c;

	RIPL_VALIDATE(pinputGreyMap->rows>=6 && pinputGreyMap->cols>=6)
	RIPL_VALIDATE(pconfig->win_w<=pinputGreyMap->cols)
	RIPL_VALIDATE(pconfig->win_h<=pinputGreyMap->rows)

	data.pinput_greymap=pinputGreyMap;
	data.poutput_greymap=poutputGreyMap;

	/* Allocate memory for accumulator images, series and filter series. */
	data.he_series=(float *)riplCalloc(pconfig->num_terms, sizeof(float));
	RIPL_VALIDATE(data.he_series)
	data.cum_series=(float *)riplCalloc(pconfig->num_terms, sizeof(float));
	RIPL_VALIDATE(data.cum_series)
	data.addback_series=(float *)riplCalloc(pconfig->num_terms, sizeof(float));
	RIPL_VALIDATE(data.addback_series)
	data.filt=(long *)riplCalloc(RIPL_GREY_LEVELS , sizeof(long));
	RIPL_VALIDATE(data.filt)
	data.not_filt=(long *)riplCalloc(RIPL_GREY_LEVELS , sizeof(long));
	RIPL_VALIDATE(data.not_filt)
	data.pbig_image=(riplBigGreyS *)riplCalloc((data.pinput_greymap->cols+1)
		*(data.pinput_greymap->rows+1), sizeof(riplBigGreyS));
	RIPL_VALIDATE(data.pbig_image)
	data.pmid_image=(riplMidGreyS *)riplCalloc(data.pinput_greymap->size,
		sizeof(riplMidGreyS));
	RIPL_VALIDATE(data.pmid_image)
	memset(data.pmid_image, 0, data.pinput_greymap->size*sizeof(riplMidGreyS));

	/* Initialize series. */
	initialize_series(pconfig);

	/* Perform HE and copy result to output greymap object. */
	for (i=0; i<pconfig->num_terms; i++) {
		set_tables(pconfig, i, ttCos);
		if (data.accShift<MAX_BIT_SHIFT) {
			filter(data.pinput_greymap->data,
				data.pbig_image,
				data.filt,
				data.pinput_greymap->cols,
				data.pinput_greymap->rows,
				pconfig->win_w/2, pconfig->win_h/2);
			accumulate();
		}
		set_tables(pconfig, i, ttSin);
		if (data.accShift<MAX_BIT_SHIFT) {
			filter(data.pinput_greymap->data,
				data.pbig_image,
				data.filt,
				data.pinput_greymap->cols,
				data.pinput_greymap->rows,
				pconfig->win_w/2, pconfig->win_h/2);
			accumulate();
		}
	}
	offset=(riplMidGreyS)ldexp(255-((double)255.0-2*pconfig->inoffset)
		*pconfig->addback_fraction+1, ACC_BITS-OUT_BITS);
	accP=data.pmid_image;
	outP=data.poutput_greymap->data;
	for (r=0; r<data.poutput_greymap->rows; r++) {
		for (c=0; c<data.poutput_greymap->cols; c++, accP++) {
			temp=(riplMidGreyS)((*accP+offset)>>(ACC_BITS-OUT_BITS+1));
			if (temp>255) temp=255;
			else if (temp<0) temp=0;
			*outP++=(riplGrey)temp;
		}
	}
	
	/* Deallocate all temporary/series memory. */
	riplFree(data.addback_series);
	riplFree(data.cum_series);
	riplFree(data.he_series);
	riplFree(data.pmid_image);
	riplFree(data.pbig_image);
	riplFree(data.not_filt);
	riplFree(data.filt);
	return true;
}

/* Set up HE series vectors. */
static void initialize_series(const aheConfig *pconfig) {

	unsigned i;
	double temp;
	struct taghe_series_data he_data;

	/* Cumulation function series. */
	for (i=0; i<pconfig->num_terms; i++) data.cum_series[i]=1.0;

	/* HE series. */
	if (pconfig->he_type==htFraction) {
		he_series_init(&he_data, pconfig->he_fraction);
		for (i=0; i<pconfig->num_terms; i++) {
			data.he_series[i]=he_series_value(&he_data, i);
		}
	}
	else {
		for (i=0; i<pconfig->num_terms; i++) {
			data.he_series[i]=4/RIPL_M_PI/(2*i+1);
		}
	}

	/* Hamming windowing. */
	if (pconfig->hamming_window==true) {
		for (i=0; i<pconfig->num_terms; i++) {
			data.cum_series[i]*=
				(1.0+0.46*cos(RIPL_M_PI/(2*pconfig->num_terms)*(2*i+1)));
		}
	}

	/* Gaussian blurring. */
	if (pconfig->gauss_mode==true) {
		temp=-0.125*RIPL_M_PI*RIPL_M_PI/RIPL_MAX_GREY/RIPL_MAX_GREY
			*pconfig->prescale_val*pconfig->prescale_val
			*pconfig->gauss_width*pconfig->gauss_width;
		for (i=0; i<pconfig->num_terms; i++)
			data.cum_series[i]*=exp(temp*(2*i+1)*(2*i+1));
	}


	/* Addback series. */
	switch (pconfig->addback_mode) {
		case atZeroInput:
			for (i=0; i<pconfig->num_terms; i++) {
				data.addback_series[i]=data.he_series[i]
					*pconfig->addback_fraction;
			}
			break;
		case atLocalMean:
			for (i=0, temp=1; i<pconfig->num_terms; i++, temp*=-1) {
				data.addback_series[i]=temp*pconfig->addback_fraction
					*16/RIPL_M_PI/RIPL_M_PI/(2*i+1)/(2*i+1);
			}
			break;
		default:
			break;
	}

	/* Apply coefficient window. */
	for (i=0; i<pconfig->num_terms; i++) {
		data.he_series[i]=data.cum_series[i]
			*data.he_series[i];
	}

	/* Addback cumulation function stuff. */
	if (pconfig->addback_cum==true) {
		for (i=0; i<pconfig->num_terms; i++) {
			data.addback_series[i]*=data.cum_series[i];
		}
	}
}

/* Initialize signed power-law HE series. */
static void he_series_init(struct taghe_series_data *pdata,
	float factor) {

	float x, g;
	float omega_sq=RIPL_M_PI*RIPL_M_PI/4;
	float cstep, cprod, csum;

	pdata->factor=factor;
	pdata->factorX=2-factor;
	x=pdata->factorX-1;
	g=((0.035868343*x-0.193527818)*x+0.482199394)*x;
	g=((g-0.756704078)*x+0.918206857)*x-0.897056937;
	g=((g*x+0.988205891)*x-0.577191652)*x+1;
	pdata->gammaVal=g;
	pdata->twoPower=pow(2.0, pdata->factorX);
	cstep=pdata->factorX;
	cprod=1.0/cstep;
	csum=cprod;
	while (fabs(cprod)>10.0*FLT_EPSILON*fabs(csum)) {
		cstep+=1.0;
		cprod=-cprod*omega_sq/cstep;
		cstep+=1.0;
		cprod/=cstep;
		csum+=cprod;
	}
	pdata->baseMag=fabs(csum);
	pdata->sinVal=sin(RIPL_M_PI*0.5*pdata->factorX);
}

/* Calculate single series value. */
static float he_series_value(struct taghe_series_data *pdata,
	unsigned i) {

	float p1r, p1i, p2r, p2i, q1r,
		q1i, q2r, q2i, omega, cold,
		cnew, tolval, n, normfact;

	if (i==1) return pdata->baseMag*pdata->twoPower;
	tolval=10.0*FLT_EPSILON*pdata->baseMag;
	omega=((float)i-0.5)*RIPL_M_PI;
	p1r=0.0;
	p1i=0.0;
	p2r=1.0;
	p2i=0.0;
	q1r=1.0;
	q1i=0.0;
	q2r=0.0;
	q2i=omega;
	cold=0.0;
	cnew=1.0;
	n=1.0;
	normfact=1.0;

	while (fabs(cold-cnew)>tolval && n<300.0) {
		cold=cnew;
		p1r=q1r+p1r*(n-pdata->factorX)*normfact;
		p1i=q1i+p1i*(n-pdata->factorX)*normfact;
		p2r=q2r+p2r*(n-pdata->factorX)*normfact;
		p2i=q2i+p2i*(n-pdata->factorX)*normfact;
		q2r=-p2i*omega+n*q2r;
		q2i=p2r*omega+n*q2i;
		normfact=1.0/sqrt(q2r*q2r+q2i*q2i);
		q2r=q2r*normfact;
		q2i=q2i*normfact;
		q1r=(-p1i*omega+n*q1r)*normfact;
		q1i=(p1r*omega+n*q1i)*normfact;
		cnew=(q1r*q2r+q1i*q2i)/(q2r*q2r+q2i*q2i);
		n+=1.0;
	}
	return (1.0-2.0*fmod((double)i, 2.0))*cnew
		+pdata->sinVal*pdata->gammaVal
		*pow(omega, -pdata->factorX)*pdata->twoPower;
}

/* Set up filter tables. */
static void set_tables(const aheConfig *pconfig,
	unsigned term_num,
	enum tagterm_type tt) {

	float area;
	int filtShifts, notFiltShifts, ab_shifts;
	float filtFactor, notFiltFactor, ab_factor;
	int r;
	float wave_factor;

	area=(2.0*(unsigned)(pconfig->win_w/2)+1)*(2.0*(unsigned)(pconfig->win_h/2)+1);
	filtFactor=frexp(1.0/area, &filtShifts)*AFT_FILTVAL;
	filtShifts=-filtShifts;
	notFiltFactor=frexp(((double)data.he_series[term_num])
		*255.0/AFT_FILTVAL/NOT_FILTVAL+FLT_MIN, &notFiltShifts)*NOT_FILTVAL;
	ab_factor=frexp(((double) data.addback_series[term_num])
		*255.0/AFT_FILTVAL/NOT_FILTVAL+FLT_MIN, &ab_shifts)*NOT_FILTVAL;

	wave_factor=(2*term_num+1)
		*RIPL_M_PI/RIPL_MAX_GREY*pconfig->prescale_val/2;
	switch (tt) {
		case ttCos:
			for (r=0; r<RIPL_GREY_LEVELS; r++) {
				data.filt[r]=filtFactor
					*cos((r-pconfig->inoffset)*wave_factor);
				data.not_filt[r]=notFiltFactor
					*sin((r-pconfig->inoffset)*wave_factor);
			}
			break;
		case ttSin:
			if (ab_shifts<notFiltShifts) {
				ab_factor=ldexp(ab_factor,ab_shifts-notFiltShifts);
			}
			else {
				notFiltFactor=ldexp(notFiltFactor,notFiltShifts-ab_shifts);
				notFiltShifts=ab_shifts;
			}
			for (r=0; r<RIPL_GREY_LEVELS; r++) {
				data.not_filt[r]=-1*((long)(notFiltFactor
					*(cos((r-pconfig->inoffset)*wave_factor))-ab_factor));
				data.filt[r]=filtFactor
					*sin((r-pconfig->inoffset)*wave_factor);
			}
			break;
		default:
			break;
	}
	data.filtFactor=filtFactor;
	data.filtShifts=filtShifts;
	data.notFiltFactor=notFiltFactor;
	data.notFiltShifts=notFiltShifts;
	data.accShift=(-notFiltShifts-BSHIFT+OUT_BITS-ACC_BITS);
}

/* Accumulate. */
static void accumulate(void) {

	unsigned i;
	riplGrey *igP;
	riplMidGreyS *agP;
	riplBigGreyS *bgP;
	long temp, offset;
	int shift;

	shift=data.accShift;
	offset=1<<(data.accShift-1);

	bgP=data.pbig_image;
	agP=data.pmid_image;
	igP=data.pinput_greymap->data;
	for (i=0; i<data.pinput_greymap->size; i++) {
		temp=data.not_filt[*igP]*(*bgP>>BSHIFT)+offset;
		*agP+=(riplMidGreyS)(temp>>shift);
		bgP++;
		agP++;
		igP++;
	}
}

/* Perform filtering based on look-up tables. */
static void filter(riplGrey *inImgPtr, riplBigGreyS *outImgPtr,
	long *lTable,
	unsigned cols,
	unsigned rows,
	unsigned width,
	unsigned height) {

	unsigned r, c, k;
	int second_shifts, first_shifts;
	riplBigGreyS shifted_table[RIPL_GREY_LEVELS];
	riplBigGreyS accumX;
	riplBigGreyS *oagP;
	riplBigGreyS accum;
	riplBigGreyS *obgP, *iagP;
	riplGrey *iigP;

	/*
	 * The amount of down-shifting in preparation for the first
	 * pass is analogous to that for filtShifts.  However, some of
	 * scaling down has already been done in constructing the
	 * table.
	 */
	frexp((2*width+1.0)*((double)data.filtFactor)/AFT_FILTVAL,
		&first_shifts);

	/* The shifts required before the second (vertical) pass. */
	second_shifts=data.filtShifts -first_shifts;
	for (r=0; r<=RIPL_MAX_GREY; r++) {
		shifted_table[r]=lTable[r]>>first_shifts;
	}

	iigP=inImgPtr;
	iagP=outImgPtr+cols;
	for (c=0; c<rows*cols; c++) {
		*iagP++=shifted_table[*iigP++];
	}

	for (r=0; r<rows; r++) {
		/*
		 * Copy row of input image using lookup table to output array,
		 * but starting at location (1,1).
		 *
		 * Add up first (2*width+1) columns in each row.
		 * Put in zeroth column.  This is the output for the first
		 * width+1 columns (0 to width).
		 */
		obgP=&outImgPtr[(r+1)*cols];
		iagP=obgP;
		accum=0;

		for (c=2*width+1; c>0; c--) accum+=*iagP++;
		accumX=accum;

		/*
		 * MAIN SCAN ACROSS.
		 * iagP is already pointing at the next one to add.
		 */
		for (c=cols-2*width-1; c>0; c--) {
			accum+=(*iagP++)-*obgP;
			*obgP++=accum;
		}
		iagP=obgP-1;

		/*
		 * SHIFT AND STORE.
		 * First the last output into the last w+1 entries.
		 */
		accum=*iagP>>second_shifts;

		obgP=&outImgPtr[(r+2)*cols-1];
		for (c=width+1; c>0; c--) *obgP--=accum;

		/* Then cols-2*width-2 more. */
		iagP--;
		for (c=cols-2*width-2; c>0; c--) *obgP--=(*iagP--)>>second_shifts;

		/* Then the first/final w+1. */
		accum=accumX>>second_shifts;
		for (c=width+1; c>0; c--) *obgP--=accum;
	}

	/*
	 * FILTER DOWN COLUMNS
	 * Sum first 2*height+1 rows.
	 */
	memset(outImgPtr, 0, cols*sizeof(riplBigGreyS));

	obgP=outImgPtr;
	for (c=cols; c>0; c--) {
		accum=0;
		iagP=obgP+cols;
		for (r=2*height+1; r>0; r--) {
			accum+=*iagP;
			iagP+=cols;
		}
		*obgP++=accum;
	}
	iagP=&outImgPtr[(2*height+2)*cols];

	/*
	 * MAIN SCAN DOWN
	 * iagP already points to next to add, obgP to in/output.
	 * Just do one big loop instead of nesting.
	 */
	oagP=outImgPtr;
	for (k=cols*(rows-2*height-1); k>0; k--) {
		accum=(*oagP++)+(*iagP++)-*obgP;
		*obgP++=accum;
	}
      
	/*
	 * STORE BY COPYING
	 * First the last output into the last h entries.
	 */
	obgP=&outImgPtr[(rows-1)*cols];
	for (r=height; r!=0; r--, obgP-=cols) {
		memcpy(obgP, oagP, cols*sizeof(riplBigGreyS));
	}
	/* Middle section. */
	memmove(&outImgPtr[height*cols],
		outImgPtr, cols*(rows-2*height)*sizeof(riplBigGreyS));
	/* And first/final rows. */
	for (r=1; r<height; r++) {
		memcpy(&outImgPtr[r*cols], outImgPtr, cols*sizeof(riplBigGreyS));
	}
}


/* Command-line version. */
int aheExecute(unsigned argc,
	const char **argv,
	riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap) {

	int parse_result;
	aheConfig config;

	RIPL_VALIDATE(pinputGreyMap->rows>=6 && pinputGreyMap->cols>=6)

	if (argc<1) {
		riplMessage(itError, "Syntax error!\n"
			"Usage: " RIPL_EXENAME " " RIPL_CMDLINE " ahe <parameters>\n");
		return RIPL_PARAMERROR;
	}

	/* Set up default configuration and parse command line. */
	aheInitializeConfig(&config);
	parse_result=parse_arguments(&config, argc, argv);
	if (parse_result<0) return parse_result;
	/* Default prescale value. */
	if (config.prescale_mode==false) {
		if (config.he_type==htStandard) config.prescale_val=1.7;
		else config.prescale_val=1.0;
	}
	/* Default addback. */
	if (config.addback_mode==atNone) {
		config.addback_mode=atLocalMean;
		config.addback_fraction=1-config.he_fraction;
	}
	if (!aheApplyOperator(pinputGreyMap, poutputGreyMap, &config))
		return RIPL_EXECUTEERROR;
	return parse_result;
}

/* Initialize a configuration structure to default values. */
#define MIDGREY 127.5
void aheInitializeConfig(aheConfig *pconfig) {

	RIPL_VALIDATE(pconfig)

	pconfig->num_terms=20;
	pconfig->win_w=pconfig->win_h=20;
	pconfig->hamming_window=false;
	pconfig->prescale_mode=false;
	pconfig->prescale_val=0.0;
	pconfig->he_type=htStandard;
	pconfig->he_fraction=1.0;
	pconfig->addback_mode=atNone;
	pconfig->addback_fraction=0.0;
	pconfig->addback_cum=true;
	pconfig->gauss_mode=false;
	pconfig->gauss_width=0.0;
	pconfig->inoffset=MIDGREY;
}

/* Parse command-line arguments. */
static int parse_arguments(aheConfig *pconfig,
	unsigned argc,
	const char **argv) {

	unsigned argn=0;
	riplBool finished=false;

	if (argc<1) {
		riplMessage(itInfo, aheHelp());
		return RIPL_PARAMERROR;
	}
	do {
		if (riplMatch(argv[argn], "-hamming", 1)) {
			pconfig->hamming_window=true;
			argn++;
		}
		else if (riplMatch(argv[argn], "-prescale", 2)) {
			if (argc-argn<2) {
				riplMessage(itError, "ahe: <prescale> requires a value!\n");
				return RIPL_PARAMERROR;
			}
			/* Prescale. */
			pconfig->prescale_mode=true;
			if (!riplArgGet_float(argv[argn+1], &pconfig->prescale_val)) {
				riplMessage(itError,
					"ahe: <prescale> should be a float between 0 and 2.\n");
				return RIPL_PARAMERROR;
			}
			if (pconfig->prescale_val<0 || pconfig->prescale_val>2) {
				riplMessage(itError,
					"ahe: <prescale> should be a float between 0 and 2.\n");
				return RIPL_PARAMERROR;
			}
			argn+=2;
		}
		else if (riplMatch(argv[argn], "-addback", 2)) {
			if (argc-argn<2) {
				riplMessage(itError, "ahe: <addback> requires a value!\n");
				return RIPL_PARAMERROR;
			}
			/* Zero-input addback. */
			if (pconfig->addback_mode!=atNone) {
				riplMessage(itError,
					"ahe: Addback type specified more than once!\n");
				return RIPL_PARAMERROR;
			}
			pconfig->addback_mode=atZeroInput;
			if (!riplArgGet_float(argv[argn+1], &pconfig->addback_fraction)) {
				riplMessage(itError,
					"ahe: <addback> should be a float between 0 and 1.\n");
				return RIPL_PARAMERROR;
			}
			if (pconfig->addback_fraction<0.0 || pconfig->addback_fraction>1.0) {
				riplMessage(itError,
					"ahe: <addback> should be a float between 0 and 1.\n");
				return RIPL_PARAMERROR;
			}
			argn+=2;
		}
		else if (riplMatch(argv[argn], "-localmean", 2)) {
			if (argc-argn<2) {
				riplMessage(itError, "ahe: <localmean> requires a value!\n");
				return RIPL_PARAMERROR;
			}
			/* Local-mean addback. */
			if (pconfig->addback_mode!=atNone) {
				riplMessage(itError,
					"ahe: Addback type specified more than once!\n");
				return RIPL_PARAMERROR;
			}
			pconfig->addback_mode=atLocalMean;
			if (!riplArgGet_float(argv[argn+1], &pconfig->addback_fraction)) {
				riplMessage(itError,
					"ahe: <localmean> should be a float between 0 and 1.\n");
				return RIPL_PARAMERROR;
			}
			if (pconfig->addback_fraction<0.0 || pconfig->addback_fraction>1.0) {
				riplMessage(itError,
					"ahe: <localmean> should be a float between 0 and 1.\n");
				return RIPL_PARAMERROR;
			}
			argn+=2;
		}
		else if (riplMatch(argv[argn], "-onlyheseries", 2)) {
			pconfig->addback_cum=false;
			argn++;
		}
		else if (riplMatch(argv[argn], "-gauss", 2)) {
			if (argc-argn<2) {
				riplMessage(itError, "ahe: <gauss> requires a value!\n");
				return RIPL_PARAMERROR;
			}
			/* Gaussian smoothing. */
			pconfig->gauss_mode=true;
			if (!riplArgGet_float(argv[argn+1], &pconfig->gauss_width)) {
				riplMessage(itError,
					"ahe: <gaussw> must be nonnegative float.\n");
				return RIPL_PARAMERROR;
			}
			if (pconfig->gauss_width<0.0) {
				riplMessage(itError,
					"ahe: <gaussw> must be nonnegative float.\n");
				return RIPL_PARAMERROR;
			}
			argn+=2;
		}
		else if (riplMatch(argv[argn], "-inoffset", 2)) {
			if (argc-argn<2) {
				riplMessage(itError, "ahe: <inoffset> requires a value!\n");
				return RIPL_PARAMERROR;
			}
			/* Input offset. */
			if (!riplArgGet_float(argv[argn+1], &pconfig->inoffset)) {
				riplMessage(itError,
					"ahe: <inoffset> should be float between 0 and %u.\n",
					RIPL_MAX_GREY);
				return RIPL_PARAMERROR;
			}
			argn+=2;
		}
		else if (riplMatch(argv[argn], "-fraction", 2)) {
			if (argc-argn<2) {
				riplMessage(itError, "ahe: <fraction> requires a value!\n");
				return RIPL_PARAMERROR;
			}
			/* Partial histogram equalization. */
			pconfig->he_type=htFraction;
			if (!riplArgGet_float(argv[argn+1], &pconfig->he_fraction)) {
				riplMessage(itError, "ahe: <fraction> should be a float "
					"between 0 and 1.\n");
				return RIPL_PARAMERROR;
			}
			if (pconfig->he_fraction<0.0 || pconfig->he_fraction>1.0) {
				riplMessage(itError, "ahe: <fraction> should be a float "
					"between 0 and 1.\n");
				return RIPL_PARAMERROR;
			}
			argn+=2;
		}
		else if (riplMatch(argv[argn], "-win", 2)) {
			if (argc-argn<4) {
				riplMessage(itError, "ahe: <win> requires three parameters!\n");
				return RIPL_PARAMERROR;
			}
			/* Window size and number of terms. */
			if (!riplArgGet_unsigned(argv[argn+1], &pconfig->win_w)) {
				riplMessage(itError, "ahe: <win-width> must be integer >= 3.\n");
				return RIPL_PARAMERROR;
			}
			if (pconfig->win_w<3) {
				riplMessage(itError, "ahe: <win-width> must be integer >= 3.\n");
				return RIPL_PARAMERROR;
			}
			if (!riplArgGet_unsigned(argv[argn+2], &pconfig->win_h)) {
				riplMessage(itError, "ahe: <win-height> must be integer >= 3.\n");
				return RIPL_PARAMERROR;
			}
			if (pconfig->win_h<3) {
				riplMessage(itError, "ahe: <win-height> must be integer >= 3.\n");
				return RIPL_PARAMERROR;
			}
			if (!riplArgGet_unsigned(argv[argn+3], &pconfig->num_terms)) {
				riplMessage(itError,
					"ahe: <num-terms> must be integer between 1 and %u.\n",
					MAX_TERMS);
				return RIPL_PARAMERROR;
			}
			if (pconfig->num_terms<1 || pconfig->num_terms>MAX_TERMS) {
				riplMessage(itError,
					"ahe: <num-terms> must be integer between 1 and %u.\n",
					MAX_TERMS);
				return RIPL_PARAMERROR;
			}
			argn+=4;
		}
		else finished=true;
	} while (argc>argn && !finished);

	return argn;
}

/* Return help information. */
const char *aheHelp(void) {
	return "ahe <options>\n\n"
		"Applies Fourier-series based adaptive histogram equalization to\n"
		"image. The options must be from the following:\n\n"
		"   -win <win-width> <win-height> <num-terms>\n"
		"   -fraction <value>\n"
		"   -hamming\n"
		"   -prescale <value>\n"
		"   -addback <fraction> | -localmean <fraction>\n"
		"   -onlyheseries\n"
		"   -inoffset <value>\n"
		"   -gauss <value>\n\n"
		"Based on JASHE programme written by J. Alex Stark.\n";
}

