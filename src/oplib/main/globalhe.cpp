#include "globalhe.h"

#include "misc.h"
#include "register.h"

/* Internal entrypoint. */
bool globalHEApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap) {

    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
    return miscHistogramEQ(pinputGreyMap->data(), poutputGreyMap->data(),
        pinputGreyMap->size());
}

/* Command-line version. */
#ifdef __BORLANDC__
#	pragma argsused
#endif
int globalHEExecute(unsigned argc, const char **argv,
    riplGreyMap *pinputGreyMap, riplGreyMap *poutputGreyMap) {

    if (!globalHEApplyOperator(pinputGreyMap, poutputGreyMap))
        return RIPL_EXECUTEERROR;
    return 0;
}

/* Display help screen. */
const char *globalHEHelp(void) {
    return "globalhe\n\n"
        "   Applies simplified global histogram equalization to the image.\n";
}

OPLIB_REGISTER_OP(
    globalhe,
    "apply global histogram equalization to image",
    globalHEExecute,
    globalHEHelp);
