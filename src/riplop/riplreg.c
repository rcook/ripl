/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplreg.c
 *		Source file for operator-registration functions.
 *		THIS IS WHERE NEW OPERATORS SHOULD BE REGISTERED---NOWHERE ELSE!
 *
 *		Version 1.1, last update: 9 May 1998.
 *
 *		History:
 *			9/5/98:		added 'siahe2' operator.
 *			16/2/98:		added 'phasesynth' and 'magsynth' operators.
 *			10/2/98:		added 'wmra' operator.
 *			2/2/98:		added 'wavethresh' operator.
 *			20/1/98:		changes reflecting changes to 'riplop'.
 *			16/12/97:	latest update.
 *			24/11/97:	previous major update.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include "riplop.h"
/* Headers of operators to register. */
#include "add.h"
#include "ahe.h"
#include "binmorph.h"
#include "carve.h"
#include "conv.h"
#include "dllconv.h"
#include "fftfilt.h"
#include "gauss.h"
#include "glmorph.h"
#include "globalhe.h"
#include "magsynth.h"
#include "marrhild.h"
#include "mask.h"
#include "median.h"
#include "phsynth.h"
#include "plane.h"
#include "siahe.h"
#include "siahe2.h"
#include "sobel.h"
#include "spectrum.h"
#include "thresh.h"
#include "test.h"
#include "wmra.h"
#include "wvthrsh.h"
#include "zero.h"

extern const riplOperator riplOperators[]={
    {"add", 1, "add images together pixel by pixel",
        addExecute, addHelp},
    {"ahe", 2, "perform adaptive histogram equalization",
        aheExecute, aheHelp},
    {"binmorph", 3, "apply binary morphological operators",
        binmorphExecute, binmorphHelp},
    {"carve", 3, "carve image up",
        carveExecute, carveHelp},
    {"conv", 3, "perform 2D convolution",
        convExecute, convHelp},
    {"dllconv", 3, "perform 2D convolution (alternative syntax)",
        dllconvExecute, dllconvHelp},
    {"fftfilt", 4, "apply FFT-based filter",
        fftfiltExecute, fftfiltHelp},
    {"gaussian", 4, "perform Gaussian smoothing",
        gaussExecute, gaussHelp},
    {"glmorph", 5, "apply grey-level morphological operators",
        glmorphExecute, glmorphHelp},
    {"globalhe", 3, "apply global histogram equalization to image",
        globalHEExecute, globalHEHelp},
    {"magsynth", 6, "perform Fourier-magnitude-only synthesis",
        magsynthExecute, magsynthHelp},
    {"marrhildreth", 4, "perform Marr-Hildreth edge detection",
        marrhildExecute, marrhildHelp},
    {"mask", 3, "apply mask to image",
        maskExecute, maskHelp},
    {"median", 3, "apply median filter",
        medianExecute, medianHelp},
    {"phasesynth", 6, "perform Fourier-phase-only synthesis",
        phasesynthExecute, phasesynthHelp},
    {"plane", 3, "extract bitplane from image",
        planeExecute, planeHelp},
    {"siahe", 0, "apply subimage-based AHE to image",
        siaheExecute, siaheHelp},
    {"siahe2", 0, "apply extended raised-cosine subimage AHE to image",
        siahe2Execute, siahe2Help},
    {"sobel", 3, "perform Sobel edge detection",
        sobelExecute, sobelHelp},
    {"spectrum", 4, "obtain normalized spectrum of image",
        spectrumExecute, spectrumHelp},
    {"test", 3, "test RConsole",
        testExecute, testHelp},
    {"threshold", 5, "threshold image",
        thresholdExecute, thresholdHelp},
    {"wavethresh", 6, "reconstruct after wavelet thresholding",
        wavethreshExecute, wavethreshHelp},
    {"wmra", 3, "wavelet-based multiresolution analysis",
        wmraExecute, wmraHelp},
    {"zero", 3, "apply zeroing operator to image",
        zeroExecute, zeroHelp}
};

extern const unsigned
    riplNumOperators=sizeof(riplOperators)/sizeof(*riplOperators);

