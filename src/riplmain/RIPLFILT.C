/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplfilt.c
 *		Source file for graphic import/export filters and support functions.
 *
 *		Version 1.1, last update: 20 January 1998.
 *
 *		History:
 *			20/1/98:		version 1.1.
 *			27/11/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include <stdlib.h>
#include <stdio.h>
#include "riplfilt.h"
#include "ripldbug.h"
#include "riplpbm.h"

/* Load specified image converting it to greyscale if required. */
riplGreyMap *riplLoadImage(const char *pfileName) {

	riplGraphicFormat graphic_format;

	RIPL_VALIDATE(pfileName)

	/* Read format of image file. */
	graphic_format=riplReadGraphicFormat(pfileName);
	if (graphic_format==gfInvalid) return NULL;

	/* Load file using appropriate filter---return pointer to it. */
	switch (graphic_format) {
		case gfPBMASCII:
		case gfPGMASCII:
		case gfPPMASCII:
		case gfPBMBinary:
		case gfPGMBinary:
		case gfPPMBinary:
			return riplPBMLoadFile(pfileName, graphic_format);
		default:
			return NULL;
	}
}

/* Save specified image under specified filename in format given. */
riplBool riplSaveImage(const char *pfileName,
	riplGraphicFormat graphicFormat,
	const riplGreyMap *pgreyMap) {

	RIPL_VALIDATE(pfileName)
	RIPL_VALIDATE(graphicFormat!=gfInvalid)
	RIPL_VALIDATE_GREYMAP(pgreyMap)

	/* Save file using appropriate filter. */
	switch (graphicFormat) {
		case gfPBMASCII:
		case gfPGMASCII:
		case gfPPMASCII:
		case gfPBMBinary:
		case gfPGMBinary:
		case gfPPMBinary:
			return riplPBMSaveFile(pfileName, graphicFormat, pgreyMap);
		default:
			return false;
	}
}

/* Determine the graphic-file format of the specified file. */
riplGraphicFormat riplReadGraphicFormat(const char *pfileName) {
	FILE *pfile;
	char id;

	RIPL_VALIDATE(pfileName)

	/* Open file and test first byte. */
	pfile=fopen(pfileName, "rb");
	RIPL_VALIDATE(pfile)
	id=(char)fgetc(pfile);
	if (id!='P') {
		fclose(pfile);
		return gfInvalid;
	}
	id=(char)fgetc(pfile);
	fclose(pfile);
	/* It's a P, therefore it's likely to be PBM/PGM/PPM. */
	switch (id) {
		case '1':
			return gfPBMASCII;
		case '2':
			return gfPGMASCII;
		case '3':
			return gfPPMASCII;
		case '4':
			return gfPBMBinary;
		case '5':
			return gfPGMBinary;
		case '6':
			return gfPPMBinary;
		default:
			return gfInvalid;
	}
}

