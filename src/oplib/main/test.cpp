#include "test.h"

#include "register.h"

/* Internal entrypoint. */
bool testApplyOperator(riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap) {

    unsigned i;

    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)

    for (i=0; i<100; i++) {
        riplMessage(itConsole, "Hello!!! i = %u\n", i);
    }

    return true;
}

/* Command-line version. */
#ifdef __BORLANDC__
#	pragma argsused
#endif
int testExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap) {

    if (!testApplyOperator(pinputGreyMap, poutputGreyMap))
        return RIPL_EXECUTEERROR;
    return 0;
}

/* Display help screen. */
const char *testHelp(void) {
    return " test\n\n"
        "   Test RConsole.\n";
}

OPLIB_REGISTER_OP(
    test,
    "test RConsole",
    testExecute,
    testHelp);
