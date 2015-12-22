#include "zero.h"

#include "register.h"

/* Internal entrypoint. */
bool zeroApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    riplGrey threshold) {

    const riplGrey *inP=pinputGreyMap->data();
    riplGrey *outP=poutputGreyMap->data();
    unsigned i;

    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)

    for (i=0; i<pinputGreyMap->size(); i++, inP++, outP++) {
        if (*inP<threshold) *outP=0;
        else *outP=*inP;
    }
    return true;
}

/* Command-line version. */
int zeroExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap) {
    riplGrey threshold;

    if (argc<1) {
        riplMessage(itError, "Incorrect number of parameters!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE " zero <level>\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_riplGrey(argv[0], &threshold)) {
        riplMessage(itError, "zero: <level> should be a grey level.\n");
        return RIPL_PARAMERROR;
    }
    if (!zeroApplyOperator(pinputGreyMap, poutputGreyMap, threshold))
        return RIPL_EXECUTEERROR;
    return 1;
}

/* Display help screen. */
const char *zeroHelp(void) {
    return " zero <level>\n\n"
        "   Sets all pixel grey levels below <level> to zero while\n"
        "   maintaining the grey level of pixels equal to or above\n"
        "   the threshold <level>.\n";
}

OPLIB_REGISTER_OP(
    zero,
    "apply zeroing operator to image",
    zeroExecute,
    zeroHelp);
