/*
 *		wavelet6.c
 *		Investigating wavelet decomposition/reconstruction of an image.
 *
 *		Programme part of Ripl test suite.
 *		Uses Ripl Wavelet Transform Toolbox.
 *
 *		WAVELET EXPERIMENT NO. 6
 *		Two-dimensional wavelet transform example. Decomposing and
 *		reconstructing real images. Programme to threshold wavelet coefficients
 *		and reconstruct image.
 *
 *		Ripl Version 1.1, last update: 23 February 1998.
 *
 *		History:
 *			23/2/98:		'ftHaar' renamed 'ftHaar2'.
 *			10/2/98:		modified to use miscRescaleF.
 *			3/2/98:		trivial modifications.
 *			2/2/98:		first version.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#include <stdio.h>
#include <string.h>
#include "riplwave.h"
#include "misc.h"

int main(int argc, char **argv) {

    riplwtFilterType filter_type=ftInvalid;
    riplGreyMap *pgrey_map;
    riplwtFilter filter;
    float *vector;
    unsigned long i, count;
    float threshold;

    /* Get/check command-line arguments. */
    if (argc!=5) {
        riplMessage(itInfo,
            "Usage: wavelet6 <image> <wave-type> <thresh> <output-file>.\n"
            "Takes the specified image <image>, applies the discrete wavelet\n"
            "transform of the type <wave-type> specified ('Daub4', 'Daub12',\n"
            "'Daub20' or 'Haar'), thresholds the coefficients about <thresh>\n"
            "and writes output to the file <output-file>.\n");
        return EXIT_FAILURE;
    }
    if (!riplFileExists(argv[1])) {
        riplMessage(itError, "wavelet6: specified image file does not exist.\n");
        return EXIT_FAILURE;
    }
    if (!strcmp(argv[2], "Daub4")) filter_type=ftDaub4;
    if (!strcmp(argv[2], "Daub12")) filter_type=ftDaub12;
    if (!strcmp(argv[2], "Daub4")) filter_type=ftDaub20;
    if (!strcmp(argv[2], "Haar")) filter_type=ftHaar2;
    if (filter_type==ftInvalid) {
        riplMessage(itError,
            "Usage: wavelet6 <image> <wave-type> <thresh> <output-file>.\n");
        return EXIT_FAILURE;
    }
    if (!riplArgGet_float(argv[3], &threshold)) {
        riplMessage(itError,
            "Usage: wavelet6 <image> <wave-type> <thresh> <output-file>.\n");
        return EXIT_FAILURE;
    }

    /* Try to load input file. */
    pgrey_map=riplLoadImage(argv[1]);
    if (!pgrey_map) {
        riplMessage(itError, "wavelet6: unable to open %s for input.\n",
            argv[1]);
        return EXIT_FAILURE;
    }
    RIPL_VALIDATE_GREYMAP(pgrey_map)
    RIPL_VALIDATE_IS_POWER_OF_2(pgrey_map->rows)
    RIPL_VALIDATE_IS_POWER_OF_2(pgrey_map->cols)

    /* Copy input image grey levels into newly allocated float vector. */
    vector=(float *)riplCalloc(pgrey_map->size, sizeof(float));
    for (i=0; i<pgrey_map->size; i++) vector[i]=(float)pgrey_map->data[i];

    /* Initialize wavelet filter coefficients. */
    riplwtSetupFilter(filter_type, &filter);

    /* Perform wavelet transform. */
    riplwt2DWT(vector,
        pgrey_map->rows,
        pgrey_map->cols,
        ttForward,
        NULL,
        &filter);

    /* Threshold wavelet coefficients. */
    count=0;
    for (i=0; i<pgrey_map->size; i++) {
        if (fabs(vector[i])>=threshold) count++;
        else vector[i]=0.0;
    }

    /* Perform inverse wavelet transform. */
    riplwt2DWT(vector,
        pgrey_map->rows,
        pgrey_map->cols,
        ttInverse,
        NULL,
        &filter);

    /* Scale output to grey levels. */
    miscRescaleF(vector, pgrey_map->data, pgrey_map->size);

    /* Write output to file. */
    if (!riplPBMSaveFile(argv[4], gfPGMBinary, pgrey_map)) {
        riplMessage(itError, "Problem writing output file %s.\n", argv[4]);
    }

    /* Display some information. */
    riplMessage(itInfo, RIPL_APPNAME " Version " RIPL_VERSION "\n"
        "Written by " RIPL_AUTHOR "\n"
        "Experiment WAVELET6, 2/2/98\n"
        "%s Wavelet Reconstruction of Image %s\n"
        "Image dimensions: %lux%lu (%lu pixels)\n"
        "Threshold = %e\n"
        "%lu of %lu basis functions used.\n",
        argv[2], argv[1], pgrey_map->rows, pgrey_map->cols,
        pgrey_map->size, threshold, count, pgrey_map->size);

    /* Tidy up. */
    riplFree(vector);
    riplFree(pgrey_map->data);
    riplFree(pgrey_map);
    return EXIT_SUCCESS;
}

