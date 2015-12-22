#include "magsynth.h"

#include "alloc.h"
#include "riplfft.h"
#include "misc.h"
#include "register.h"
#include <math.h>
#include <stdio.h>

/* Internal entrypoint. */
bool magsynthApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap) {

    float *data, *nyquist, *ptr;
    unsigned long i;
    riplGrey *pgrey;
    float val;

    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
    RIPL_VALIDATE_IS_POWER_OF_2(pinputGreyMap->height())
    RIPL_VALIDATE_IS_POWER_OF_2(pinputGreyMap->width())

    /* Allocate FFT input matrices. */
    data=(float *)riplCalloc(pinputGreyMap->size(), sizeof(float));
    RIPL_VALIDATE(data)
    nyquist=(float *)riplCalloc(pinputGreyMap->height()<<1, sizeof(float));
    RIPL_VALIDATE(nyquist)

    /* Copy grey levels into FFT input matrix. */
    ptr=data;
    pgrey=pinputGreyMap->data();
    for (i=pinputGreyMap->size(); i; i--, ptr++, pgrey++) {
        ptr[0]=(float)pgrey[0];
    }

    /* Calculate 2D FFT of grey levels. */
    riplfftReal2DFT(data,
        nyquist,
        pinputGreyMap->height(),
        pinputGreyMap->width(),
        ttForward);

    /* Replace coefficients by their magnitudes. */
    ptr=data;
    for (i=pinputGreyMap->size(); i; i-=2, ptr+=2) {
        val=sqrt(ptr[0]*ptr[0]+ptr[1]*ptr[1]);
        ptr[0]=val;
        ptr[1]=0.0;
    }

    /* Calculate inverse FFT. */
    riplfftReal2DFT(data,
        nyquist,
        pinputGreyMap->height(),
        pinputGreyMap->width(),
        ttInverse);

    /* Scale output to grey levels. */
    miscRescaleF(data, poutputGreyMap->data(), poutputGreyMap->size());

    /* Deallocate input matrices. */
    riplFree(nyquist);
    riplFree(data);
    return true;
}

/* Command-line execution version. */
#ifdef __BORLANDC__
#	pragma argsused
#endif
int magsynthExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap) {

    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)

    /* Ensure image is of suitable dimensions. */
    if (!RIPL_IS_POWER_OF_2(pinputGreyMap->height())
        || !RIPL_IS_POWER_OF_2(pinputGreyMap->width())) {
        riplMessage(itError, "magsynth: Number of rows and columns in input\n"
            "image must be integer powers of 2.\n");
        return RIPL_USERERROR;
    }

    /* Apply operator. */
    if (!magsynthApplyOperator(pinputGreyMap,
        poutputGreyMap))
        return RIPL_EXECUTEERROR;
    return 0;
}

/* Display help screen. */
const char *magsynthHelp(void) {
    return " magsynth\n\n"
        "   Fourier-transforms input image and performs magnitude-only\n"
        "   synthesis by discarding the phase content of the Fourier\n"
        "   coefficients\n";
}

OPLIB_REGISTER_OP(
    magsynth,
    "perform Fourier-magnitude-only synthesis",
    magsynthExecute,
    magsynthHelp);
