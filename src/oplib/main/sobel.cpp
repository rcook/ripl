#include "sobel.h"

#include "alloc.h"
#include "misc.h"
#include <stdlib.h>

/* Internal entrypoint. */
bool sobelApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    unsigned neighbourhood,
    riplGrey threshold,
    bool overlay) {

    unsigned window_side=neighbourhood*2+1,
        buf_size=window_side*window_side,
        subneighbours=window_side*(window_side-1)/2,
        row, col, index, i, j,
        ave_above, ave_below, diff, max_diff,
        offset=neighbourhood*pinputGreyMap->width();
    auto buf = makeArray<riplGrey>(buf_size);
    riplGrey* inP = pinputGreyMap->data() + offset;
    riplGrey* outP = poutputGreyMap->data() + offset;
    riplGrey* ptr;

    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
    RIPL_VALIDATE(neighbourhood>0)

    /* Set output image to all-black (grey level = 0). */
    miscSetImage(poutputGreyMap, 0);

    /* Do it! */
    for (row=neighbourhood;
        row<pinputGreyMap->height()-neighbourhood;
        row++, inP+=pinputGreyMap->width(), outP+=pinputGreyMap->width()) {
        for (col=neighbourhood; col<pinputGreyMap->width()-neighbourhood; col++) {
            /* Fetch pixel values in neighbourhood around current pixel. */
            ptr=inP-offset+col-neighbourhood;
            for (index=0, i=0; i<window_side; i++, ptr+=pinputGreyMap->width()) {
                for (j=0; j<window_side; j++) buf[index++]=ptr[j];
            }
            /* Find the maximum absolute difference of the cross-line averages. */
            max_diff=0;
            /* Line AEI (-45deg diagonal). */
            for (ave_above = 0, i = 0, ptr = buf.get();
                i<window_side-1; i++, ptr+=window_side) {
                for (j=i+1; j<window_side; j++) ave_above+=ptr[j];
            }
            for (ave_below = 0, i = 1, ptr = buf.get() + window_side;  i< window_side;
                i++, ptr+=window_side) {
                for (j=0; j<i; j++) ave_below+=ptr[j];
            }
            max_diff = abs(static_cast<long>(ave_above - ave_below));
            /* Line BEH (vertical). */
            for (ave_above = 0, i = 0, ptr = buf.get(); i < window_side;
                i++, ptr+=window_side) {
                for (j=neighbourhood+1; j<window_side; j++) ave_above+=ptr[j];
            }
            for (ave_below=0, i=0, ptr = buf.get(); i < window_side;
                i++, ptr+=window_side) {
                for (j=0; j<neighbourhood; j++) ave_below+=ptr[j];
            }
            diff = abs(static_cast<long>(ave_above - ave_below));
            if (diff>max_diff) max_diff=diff;
            /* Line CEG (-135deg diagonal). */
            for (ave_above = 0, i = 1, ptr = buf.get() + window_side; i < window_side;
                i++, ptr+=window_side) {
                for (j=window_side-i; j<window_side; j++) ave_above+=ptr[j];
            }
            for (ave_below = 0, i = 0, ptr = buf.get(); i < window_side - 1;
                i++, ptr+=window_side) {
                for (j=0; j<window_side-1-i; j++) ave_below+=ptr[j];
            }
            diff = abs(static_cast<long>(ave_above - ave_below));
            if (diff>max_diff) max_diff=diff;
            /* Line DEF	(horizontal). */
            for (ave_above = 0, i = 0, ptr = buf.get(); i < neighbourhood;
                i++, ptr+=window_side) {
                for (j=0; j<window_side; j++) ave_above+=ptr[j];
            }
            for (ave_below = 0, i = neighbourhood + 1, ptr = buf.get() + i * window_side;
                i<window_side; i++, ptr+=window_side) {
                for (j=0; j<window_side; j++) ave_below+=ptr[j];
            }
            diff = abs(static_cast<long>(ave_above - ave_below));
            if (diff>max_diff) max_diff=diff;
            max_diff/=subneighbours;
            /*
             * If maximum difference is above threshold then set pixel
             * to white (grey level RIPL_MAX_GREY).
             */
            if (max_diff>=threshold) outP[col]=RIPL_MAX_GREY;
            else {
                /* If overlay mode on, then copy input to output. */
                if (overlay) outP[col]=inP[col];
            }
        }
    }

    return true;
}

/* Command-line version. */
int sobelExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap) {

    unsigned neighbourhood;
    riplGrey threshold;
    bool overlay;

    if (argc<3) {
        riplMessage(itError, "Incorrect number of parameters!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE
            " sobel <neigh> <thresh> <over>\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_unsigned(argv[0], &neighbourhood)) {
        riplMessage(itError, "sobel: <neigh> should be an integer >= 0.\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_riplGrey(argv[1], &threshold)) {
        riplMessage(itError, "sobel: <thresh> should be a byte.\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_bool(argv[2], &overlay)) {
        riplMessage(itError, "sobel: <over> should be a boolean.\n");
        return RIPL_PARAMERROR;
    }
    if (!sobelApplyOperator(pinputGreyMap,
        poutputGreyMap,
        neighbourhood,
        threshold,
        overlay)) return RIPL_EXECUTEERROR;
    return 3;
}

/* Display help screen. */
const char *sobelHelp(void) {
    return "sobel <neigh> <thresh> <over>\n\n"
        "   Performs Sobel edge detection with a neighbourhood size\n"
        "   <neigh> (window size=<neigh>*2+1) with a threshold <thresh>.\n"
        "   The original image is overlaid is <over> is TRUE.\n";
}

