/*
 *		test.c
 *		Test RConsole.
 *
 *		Version 1.1, last update: 24 January 1998.
 *
 *		History:
 *			24/1/98:		introduced RIPL_PARAMERROR.
 *			16/12/97:	updated comments.
 *			6/12/97:		first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include "test.h"

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

