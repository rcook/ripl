/*
 *		wavelet5.c
 *		Investigating wavelet decomposition of an image.
 *
 *		Programme part of Ripl test suite.
 *		Uses Ripl Wavelet Transform Toolbox.
 *
 *		WAVELET EXPERIMENT NO. 5
 *		Two-dimensional wavelet transform example. Decomposing and
 *		reconstructing real images. Programme to output bucketed-histogram
 *		table of wavelet coefficients.
 *
 *		Ripl Version 1.1, last update: 23 February 1998.
 *
 *		History:
 *			23/2/98:		'ftHaar' renamed 'ftHaar2'.
 *			3/2/98:		trivial modifications.
 *			2/2/98:		first version.
 *
 *		Copyright © 1998, Richard A. Cook.
 */
#include <stdio.h>
#include <string.h>
#include "riplwave.h"

int main(int argc, char **argv) {

    riplwtFilterType filter_type=ftInvalid;
    unsigned buckets, index;
    riplGreyMap *pgrey_map;
    riplwtFilter filter;
    FILE *pfile;
    float *vector;
    unsigned long i, j;
    float min, max, step, temp;
    unsigned long *histogram;

    /* Get/check command-line arguments. */
    if (argc!=5) {
        riplMessage(itInfo,
            "Usage: wavelet5 <image> <wave-type> <nbuckets> <bucket-file>.\n"
            "Takes the specified image <image>, applies the discrete wavelet\n"
            "transform of the type <wave-type> specified ('Daub4', 'Daub12',\n"
            "'Daub20' or 'Haar') and outputs the <nbuckets>-bucket histogram\n"
            "of wavelet coefficients to the specified file <bucket-file>.\n");
        return EXIT_FAILURE;
    }
    if (!riplFileExists(argv[1])) {
        riplMessage(itError, "wavelet5: specified image file does not exist.\n");
        return EXIT_FAILURE;
    }
    if (!strcmp(argv[2], "Daub4")) filter_type=ftDaub4;
    if (!strcmp(argv[2], "Daub12")) filter_type=ftDaub12;
    if (!strcmp(argv[2], "Daub4")) filter_type=ftDaub20;
    if (!strcmp(argv[2], "Haar")) filter_type=ftHaar2;
    if (filter_type==ftInvalid) {
        riplMessage(itError,
            "Usage: wavelet5 <image> <wave-type> <nbuckets> <bucket-file>.\n");
        return EXIT_FAILURE;
    }
    if (!riplArgGet_unsigned(argv[3], &buckets)) {
        riplMessage(itError,
            "Usage: wavelet5 <image> <wave-type> <nbuckets> <bucket-file>.\n");
        return EXIT_FAILURE;
    }

    /* Try to load input file. */
    pgrey_map=riplLoadImage(argv[1]);
    if (!pgrey_map) {
        riplMessage(itError, "wavelet5: unable to open %s for input.\n",
            argv[1]);
        return EXIT_FAILURE;
    }
    RIPL_VALIDATE_GREYMAP(pgrey_map)
    RIPL_VALIDATE_IS_POWER_OF_2(pgrey_map->rows)
    RIPL_VALIDATE_IS_POWER_OF_2(pgrey_map->cols)

    /* Try to open output file. */
    pfile=fopen(argv[4], "wt");
    if (!pfile) {
        riplMessage(itError, "wavelet5: unable to open %s for output.\n",
            argv[4]);
        return EXIT_FAILURE;
    }

    /* Copy input image grey levels into newly allocated float vector. */
    vector=(float *)riplCalloc(pgrey_map->size, sizeof(float));
    RIPL_VALIDATE(vector)
    for (i=0; i<pgrey_map->size; i++) vector[i]=(float)pgrey_map->data[i];

    /* Initialize wavelet filter coefficients. */
    riplwtSetupFilter(filter_type, &filter);

    /* Perform wavelet transform. */
    riplwt2DWT(vector, pgrey_map->rows, pgrey_map->cols, ttForward, NULL, &filter);

    /* Find minimum and maximum coefficient values and calculate step. */
    min=max=fabs(vector[0]);
    for (i=1; i<pgrey_map->size; i++) {
        temp=fabs(vector[i]);
        if (temp<min) min=temp;
        if (temp>max) max=temp;
    }
    step=(max-min)/buckets;

    /* Allocate histogram vector. */
    histogram=(unsigned long *)riplCalloc(buckets, sizeof(unsigned long));
    for (i=0; i<buckets; i++) histogram[i]=0;

    /* Form histogram. */
    for (i=0; i<pgrey_map->size; i++) {
        index=(unsigned)((fabs(vector[i])-min)/step);
        if (index>=buckets) index=buckets-1;
        histogram[index]++;
    }

    /* Write histogram contents out to file. */
    fprintf(pfile, RIPL_APPNAME " Version " RIPL_VERSION "\n"
        "Written by " RIPL_AUTHOR "\n"
        "Experiment WAVELET5, 2/2/98\n"
        "%s Wavelet Coefficient Histogram of Image %s\n"
        "Image dimensions: %lux%lu (%lu pixels)\n"
        "Number of buckets: %u\n\n"
        "Bucket       Threshold   Count  CumSum\n",
        argv[2], argv[1], pgrey_map->rows, pgrey_map->cols,
        pgrey_map->size, buckets);
    for (i=0, j=pgrey_map->size, temp=min; i<buckets;
        i++, temp+=step) {
        fprintf(pfile, "%6u  %14e  %6lu  %6lu\n", i, temp, histogram[i], j);
        j-=histogram[i];
    }

    /* Tidy up. */
    riplFree(histogram);
    riplFree(vector);
    riplFree(pgrey_map->data);
    riplFree(pgrey_map);
    fclose(pfile);
    return EXIT_SUCCESS;
}

