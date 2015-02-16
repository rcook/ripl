/*
 *		carve.c
 *		Source file for carving operator.
 *
 *		Version 1.1, last update: 24 January 1998.
 *
 *		History:
 *			24/1/98:		introduced RIPL_PARAMERROR.
 *			16/12/97:	updated comments.
 *			15/12/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include <string.h>
#include "carve.h"
#include "misc.h"

/* Some local defines. */
#define MIN_CLASSES				2
#define MAX_CLASSES				RIPL_GREY_LEVELS
#define MIN_CONF					0
#define MAX_CONF					(RIPL_GREY_LEVELS/2)

struct tagequalize_data {
	riplGreyMap *poutput_greymap;
	riplGrey *buffer;
};

/* Prototypes of static functions. */
static riplBool equalize(riplGreyMap *pgreymap,
	unsigned col, unsigned row,
	unsigned width, unsigned height,
	unsigned classno, void *arg);

/* Internal entrypoint. */
riplBool carveApplyOperator(riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap,
	unsigned win_width, unsigned win_height,
	unsigned min_width, unsigned min_height,
	unsigned nclasses, riplGrey conf) {

	riplGrey *boundary;
	unsigned i;
	struct tagequalize_data equalize_data;
	riplBool result;

	RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
	RIPL_VALIDATE(nclasses>=MIN_CLASSES && nclasses<=MAX_CLASSES)
	RIPL_VALIDATE(win_width>0 && win_width<=pinputGreyMap->cols)
	RIPL_VALIDATE(win_height>0 && win_height<=pinputGreyMap->rows)

	boundary=(riplGrey *)riplCalloc(nclasses, sizeof(riplGrey));
	RIPL_VALIDATE(boundary)

	for (i=0; i<nclasses; i++) {
		boundary[i]=(riplGrey)((RIPL_GREY_LEVELS-1)*(i+1)/(nclasses+1));
	}

	equalize_data.poutput_greymap=poutputGreyMap;
	equalize_data.buffer=
		(riplGrey *)riplCalloc(win_width*win_height, sizeof(riplGrey));
	result=miscCarve(pinputGreyMap,
		conf, nclasses, boundary,
		win_width, win_height, min_width, min_height,
		equalize, &equalize_data);

	riplFree(equalize_data.buffer);
	riplFree(boundary);
	return result;
}

#ifdef __BORLANDC__
#	pragma argsused
#endif
static riplBool equalize(riplGreyMap *pgreymap,
	unsigned col,
	unsigned row,
	unsigned width,
	unsigned height,
	unsigned classno,
	void *arg) {

	struct tagequalize_data *pdata=(struct tagequalize_data *)arg;
	unsigned i;

	/* Copy the subimage into the buffer. */
	for (i=0; i<height; i++) {
		memcpy(pdata->buffer+i*width,
			pgreymap->data+(row+i)*pgreymap->cols+col, width*sizeof(riplGrey));
	}

	/* Equalize the pixels. */
	if (!miscHistogramEQ(pdata->buffer, pdata->buffer, width*height)) {
		return false;
	}

	/* Copy the pixels to the output greymap. */
	for (i=0; i<height; i++) {
		memcpy(pdata->poutput_greymap->data+(row+i)
			*pdata->poutput_greymap->cols+col,
			pdata->buffer+i*width, width*sizeof(riplGrey));
	}
	return true;
}


/* Command-line version. */
int carveExecute(unsigned argc,
	const char **argv,
	riplGreyMap *pinputGreyMap,
	riplGreyMap *poutputGreyMap) {

	unsigned win_width, win_height;
	unsigned min_width, min_height;
	unsigned nclasses;
	riplGrey conf;

	if (argc<6) {
		riplMessage(itError, "Incorrect number of parameters!\n"
			"Usage: " RIPL_EXENAME " " RIPL_CMDLINE
			" carve <winw> <winh> <minw> <minh> <classes> <conf>\n");
		return RIPL_PARAMERROR;
	}
	if (!riplArgGet_unsigned(argv[0], &win_width)) {
		riplMessage(itError, "carve: <winw> should be an integer > 0.");
		return RIPL_PARAMERROR;
	}
	if (win_width<1) {
		riplMessage(itError, "carve: <winw> should be an integer > 0.");
		return RIPL_PARAMERROR;
	}
	if (win_width>pinputGreyMap->cols) {
		riplMessage(itError, "carve: <winw> should be less than "
			"number of columns in image.");
		return RIPL_PARAMERROR;
	}
	if (!riplArgGet_unsigned(argv[1], &win_height)) {
		riplMessage(itError, "carve: <winh> should be an integer > 0.");
		return RIPL_PARAMERROR;
	}
	if (win_height<1) {
		riplMessage(itError, "carve: <winh> should be an integer > 0.");
		return RIPL_PARAMERROR;
	}
	if (win_height>pinputGreyMap->rows) {
		riplMessage(itError, "carve: <winw> should be less than "
			"number of rows in image.");
		return RIPL_PARAMERROR;
	}
	if (!riplArgGet_unsigned(argv[2], &min_width)) {
		riplMessage(itError, "carve: <minw> should be an integer > 0.");
		return RIPL_PARAMERROR;
	}
	if (min_width<1) {
		riplMessage(itError, "carve: <minw> should be an integer > 0.");
		return RIPL_PARAMERROR;
	}
	if (min_width>win_width) {
		riplMessage(itError, "carve: <minw> should be less than <winw>.");
		return RIPL_PARAMERROR;
	}
	if (!riplArgGet_unsigned(argv[3], &min_height)) {
		riplMessage(itError, "carve: <minh> should be an integer > 0.");
		return RIPL_PARAMERROR;
	}
	if (min_height<1) {
		riplMessage(itError, "carve: <minh> should be an integer > 0.");
		return RIPL_PARAMERROR;
	}
	if (min_height>win_height) {
		riplMessage(itError, "carve: <minh> should be less than <winh>.");
		return RIPL_PARAMERROR;
	}
	if (!riplArgGet_unsigned(argv[4], &nclasses)) {
		riplMessage(itError, "carve: <classes> should be an integer between "
			"%u and %u.\n", MIN_CLASSES, MAX_CLASSES);
		return RIPL_PARAMERROR;
	}
	if (nclasses<MIN_CLASSES || nclasses>MAX_CLASSES) {
		riplMessage(itError, "carve: <classes> should be an integer between "
			"%u and %u.\n", MIN_CLASSES, MAX_CLASSES);
		return RIPL_PARAMERROR;
	}
	if (!riplArgGet_riplGrey(argv[5], &conf)) {
		riplMessage(itError, "carve: <conf> should be an integer between %u and "
			"%u.\n", MIN_CONF, MAX_CONF);
		return RIPL_PARAMERROR;
	}
#if MIN_CONF==0
	if (conf>MAX_CONF) {
		riplMessage(itError, "carve: <conf> should be an integer between %u and "
			"%u.\n", MIN_CONF, MAX_CONF);
		return RIPL_PARAMERROR;
	}
#else
	if (conf<MIN_CONF || conf>MAX_CONF) {
		riplMessage(itError, "carve: <conf> should be an integer between %u and "
			"%u.\n", MIN_CONF, MAX_CONF);
		return RIPL_PARAMERROR;
	}
#endif
	if (!carveApplyOperator(pinputGreyMap,
		poutputGreyMap,
		win_width,
		win_height,
		min_width,
		min_height,
		nclasses,
		conf))
		return RIPL_EXECUTEERROR;
	return 6;
}

/* Display help screen. */
const char *carveHelp(void) {
	return " carve <winw> <winh> <minw> <minh> <classes> <conf>\n\n"
		"   Carves image up into subimages of dimensions given by\n"
		"   <winw> and <winh> and classifies them on the basis of\n"
		"   their mean grey levels into a number of classes given by\n"
		"   <classes> using the specified confidence interval <conf>.\n"
		"   Subimages outside this interval and subdivided until the\n"
		"   minimum dimensions <minw> and <minh> are reached. Each\n"
		"   subimage is then locally histogram-equalized.\n";
}

