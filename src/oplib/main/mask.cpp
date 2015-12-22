#include "mask.h"

#include "register.h"

/* Internal entrypoint. */
bool maskApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    riplGrey mask) {
    const riplGrey *inP=pinputGreyMap->data();
    riplGrey *outP=poutputGreyMap->data();
    unsigned i;
    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
    for (i=0; i<pinputGreyMap->size(); i++, inP++, outP++) *outP=*inP & mask;
    return true;
}

/* Command-line version. */
int maskExecute(unsigned argc, const char **argv,
    riplGreyMap *pinputGreyMap, riplGreyMap *poutputGreyMap) {
    riplGrey mask;
    if (argc<1) {
        riplMessage(itError, "Incorrect number of parameters!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE
            " mask <bit-pat>\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_riplGrey(argv[0], &mask)) {
        riplMessage(itError, "mask: <bit-pat> should be a grey level.\n");
        return RIPL_PARAMERROR;
    }
    if (!maskApplyOperator(pinputGreyMap,
        poutputGreyMap,
        mask))
        return RIPL_EXECUTEERROR;
    return 1;
}

/* Display help screen. */
const char *maskHelp(void) {
    return "mask <bit-pat>\n\n"
        "   Logically ANDs each pixel of the image with the specified\n"
        "   mask <bit-pat>.\n";
}

OPLIB_REGISTER_OP(
    mask,
    "apply mask to image",
    maskExecute,
    maskHelp);
