#include "spectrum.h"

#include "alloc.h"
#include "riplfft.h"
#include <math.h>

/* Internal entrypoint. */
bool spectrumApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    bool subtract_mean) {

    float *data, *nyquist, *ptr1, *ptr2;
    float mean, max_abs;
    unsigned long i, j, k, r1, r2;
    riplGrey *pgrey1, *pgrey2;

    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
    RIPL_VALIDATE_IS_POWER_OF_2(pinputGreyMap->height())
    RIPL_VALIDATE_IS_POWER_OF_2(pinputGreyMap->width())

    /* Allocate float vectors for input/output to/from FFT routine. */
    data=(float *)riplCalloc(pinputGreyMap->size(), sizeof(float));
    RIPL_VALIDATE(data)
    nyquist=(float *)riplCalloc(pinputGreyMap->height()<<1, sizeof(float));
    RIPL_VALIDATE(nyquist)

    /* Calculate average pixel intensity. */
    mean=0.0;
    if (subtract_mean) {
        pgrey1=pinputGreyMap->data();
        for (i=pinputGreyMap->size(); i>0; i--) mean+=(float)(*pgrey1++);
        mean/=pinputGreyMap->size();
    }

    /* Put grey levels in float vector after subtracting mean. */
    ptr1=data;
    pgrey1=pinputGreyMap->data();
    for (i=pinputGreyMap->size(); i>0; i--) *ptr1++=(float)(*pgrey1++)-mean;

    /* Perform the FFT operation. */
    riplfftReal2DFT(data, nyquist,
        pinputGreyMap->height(), pinputGreyMap->width(), ttForward);

    /* Calculate squared absolute values and find maximum. */
    max_abs=0.0;
    /* Nyquist critical frequency part of spectrum. */
    r1=(pinputGreyMap->height()>>1)+1;
    for (i=0, ptr1=ptr2=nyquist; i<r1; i++, ptr2++, ptr1+=2) {
        ptr2[0]=ptr1[0]*ptr1[0]+ptr1[1]*ptr1[1];
        if (ptr2[0]>max_abs) max_abs=ptr2[0];
    }
    /* Zero and positive frequency half of spectrum. */
    r2=pinputGreyMap->height()*(pinputGreyMap->width()>>1);
    for (i=0, ptr1=ptr2=data; i<r2; i++, ptr2++, ptr1+=2) {
        ptr2[0]=ptr1[0]*ptr1[0]+ptr1[1]*ptr1[1];
        if (ptr2[0]>max_abs) max_abs=ptr2[0];
    }

    /* Rescale absolute values and copy spectrum into greymap. */
    ptr1=data;
    pgrey1=poutputGreyMap->data();
    pgrey2=poutputGreyMap->data()+poutputGreyMap->size();
    for (i=0; pgrey2>=pgrey1; i++) {
        if (i==0 || pgrey2==pgrey1) {
            pgrey1[0]=(riplGrey)(RIPL_MAX_GREY*sqrt(ptr1[0]/max_abs));
            for (j=poutputGreyMap->width()-1, k=1; j>k; j--, k++) {
                pgrey1[j]=pgrey1[k]=
                    (riplGrey)(RIPL_MAX_GREY*sqrt(ptr1[k]/max_abs));
            }
            pgrey1[j]=nyquist[i];
        }
        else {
            pgrey1[0]=pgrey2[0]=
                (riplGrey)(RIPL_MAX_GREY*sqrt(ptr1[0]/max_abs));
            for (j=poutputGreyMap->width()-1, k=1; j>k; j--, k++) {
                pgrey2[j]=pgrey1[k]=
                    (riplGrey)(RIPL_MAX_GREY*sqrt(ptr1[k]/max_abs));
            }
            pgrey2[j]=pgrey1[k]=nyquist[i];
        }
        pgrey1+=poutputGreyMap->width();
        pgrey2-=poutputGreyMap->width();
        ptr1+=(pinputGreyMap->width()>>1);
    }

    /* Deallocate FFT input/output vectors. */
    riplFree(nyquist);
    riplFree(data);
    return true;
}

/* Command-line version. */
int spectrumExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap) {

    bool subtract_mean=false;
    bool was_argument=false;

    if (argc>0) {
        if (riplArgGet_bool(argv[0], &subtract_mean)) was_argument=true;
    }
    if (!RIPL_IS_POWER_OF_2(pinputGreyMap->height())
        || !RIPL_IS_POWER_OF_2(pinputGreyMap->width())) {
        riplMessage(itError, "Number of rows and columns in input\n"
            "image must be integer powers of 2.\n");
        return RIPL_USERERROR;
    }
    if (!spectrumApplyOperator(pinputGreyMap,
        poutputGreyMap,
        subtract_mean))
        return RIPL_EXECUTEERROR;
    return was_argument ? 1:0;
}

/* Display help screen. */
const char *spectrumHelp(void) {
    return " spectrum (<sub-mean>)\n\n"
        "   Fourier-transforms input image and writes the grey-level\n"
        "   normalized absolute-value spectrum to the specified output\n"
        "   file. The optional <sub-mean> flag should be a boolean value\n"
        "   indicating whether the mean of the data should be subtracted\n"
        "   from it before taking the FFT (default=false).\n";
}

