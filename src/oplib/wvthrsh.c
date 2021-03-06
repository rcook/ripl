/*
 *		wvthrsh.c
 *		Source file for wavelet-thresholding operator.
 *
 *		Version 1.1, last update: 23 February 1998.
 *
 *		History:
 *			23/2/98:		added support for additional wavelet types.
 *			10/2/98:		modified to use miscRescaleF.
 *			2/2/98:		first implemented.
 *
 *		Copyright � 1998, Richard A. Cook.
 */
#include "wvthrsh.h"
#include "misc.h"

/* Internal entrypoint. */
riplBool wavethreshApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    riplwtFilterType filter_type,
    float threshold) {

    riplwtFilter filter;
    float *vector;
    unsigned long i;

    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
    RIPL_VALIDATE_IS_POWER_OF_2(pinputGreyMap->rows)
    RIPL_VALIDATE_IS_POWER_OF_2(pinputGreyMap->cols)

    /* Copy input image grey levels into newly allocated float vector. */
    vector=(float *)riplCalloc(pinputGreyMap->size, sizeof(float));
    for (i=0; i<pinputGreyMap->size; i++)
        vector[i]=(float)pinputGreyMap->data[i];

    /* Initialize wavelet filter coefficients. */
    riplwtSetupFilter(filter_type, &filter);

    /* Perform wavelet transform. */
    riplwt2DWT(vector,
        pinputGreyMap->rows,
        pinputGreyMap->cols,
        ttForward,
        NULL,
        &filter);

    /* Threshold wavelet coefficients. */
    for (i=0; i<pinputGreyMap->size; i++) {
        if (fabs(vector[i])<threshold) vector[i]=0.0;
    }

    /* Perform inverse wavelet transform. */
    riplwt2DWT(vector,
        pinputGreyMap->rows,
        pinputGreyMap->cols,
        ttInverse,
        NULL,
        &filter);

    miscRescaleF(vector, poutputGreyMap->data, poutputGreyMap->size);

    /* Tidy up. */
    riplFree(vector);
    return true;
}

/* Command-line version. */
int wavethreshExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap) {

    float threshold;
    riplwtFilterType filter_type=ftInvalid;

    /* Get/check command-line arguments. */
    if (argc<2) {
        riplMessage(itError, "Incorrect number of parameters!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE
            " wavethresh <wave-type> <coeff-thresh>\n");
        return RIPL_PARAMERROR;
    }
    if (riplMatch(argv[0], "battle", 5)) filter_type=ftBattleLemarie24;
    if (riplMatch(argv[0], "coif6", 5)) filter_type=ftCoiflet6;
    if (riplMatch(argv[0], "coif12", 5)) filter_type=ftCoiflet12;
    if (riplMatch(argv[0], "coif18", 5)) filter_type=ftCoiflet18;
    if (riplMatch(argv[0], "daub4", 5)) filter_type=ftDaub4;
    if (riplMatch(argv[0], "daub6", 5)) filter_type=ftDaub6;
    if (riplMatch(argv[0], "daub8", 5)) filter_type=ftDaub8;
    if (riplMatch(argv[0], "daub10", 5)) filter_type=ftDaub10;
    if (riplMatch(argv[0], "daub12", 6)) filter_type=ftDaub12;
    if (riplMatch(argv[0], "daub14", 6)) filter_type=ftDaub14;
    if (riplMatch(argv[0], "daub16", 6)) filter_type=ftDaub16;
    if (riplMatch(argv[0], "daub18", 6)) filter_type=ftDaub18;
    if (riplMatch(argv[0], "daub20", 6)) filter_type=ftDaub20;
    if (riplMatch(argv[0], "haar", 4)) filter_type=ftHaar2;
    if (filter_type==ftInvalid) {
        riplMessage(itError,
            "wavethresh: <wave-type> should specify wavelet type.\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_float(argv[1], &threshold)) {
        riplMessage(itError,
            "wavethresh: <coeff-thresh> should be a float value.\n");
        return RIPL_PARAMERROR;
    }

    /* Ensure image is of suitable dimensions. */
    if (!RIPL_IS_POWER_OF_2(pinputGreyMap->rows)
        || !RIPL_IS_POWER_OF_2(pinputGreyMap->cols)) {
        riplMessage(itError, "wavethresh: Number of rows and columns in input\n"
            "image must be integer powers of 2.\n");
        return RIPL_USERERROR;
    }

    /* Apply the operator to the image. */
    if (!wavethreshApplyOperator(pinputGreyMap,
        poutputGreyMap,
        filter_type,
        threshold))
        return RIPL_EXECUTEERROR;
    return 2;
}

/* Display help screen. */
const char *wavethreshHelp(void) {
    return " wavethresh <wave-type> <coeff-thresh>\n\n"
        "   Applies discrete wavelet transform of type <wave-type> to image,\n"
        "   thresholds resulting coefficients using threshold <coeff-thresh>\n"
        "   and then performs an inverse discrete wavelet transform to produce\n"
        "   an image based on a reduced number of the appropriate type of\n"
        "   wavelet basis functions.\n\n"
        "   Supported filter types for <wave-type> are:\n"
        "      battle     Battle-Lemarie filter.\n"
        "      coifx      Coiflet x-coefficient filter, x in {6, 12, 18}.\n"
        "      daubx      Daubechies x-coefficient filter, x in {4, 6, 8, 10,\n"
        "                 12, 14, 16, 18, 20}.\n"
        "      haar       Haar filter.\n";
}

