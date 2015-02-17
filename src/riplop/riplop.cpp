/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplop.c
 *		Source file for operator stuff.
 *
 *		Version 1.1, last update: 3 February 1998.
 *
 *		History:
 *			3/1/98:			made cosmetic changes to help screens.
 *			24/1/98:		renamed RIPL_PARSEERROR to RIPL_PARAMERROR.
 *			21/1/98:		added RIPL_EXECUTEERROR message box output.
 *			20/1/98:		move many functions into new file 'riplpars.c'.
 *			19/1/98:		insert Watcom compiler preprocessor stuff
 *							(to suppress compiler warnings).
 *			27/11/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include <string.h>
#include <stdio.h>
#include "riplop.h"

/* Constants for local use only. */
#define BUFFER_LEN		2048

/* Return list of all operators and one-line help. */
const char *riplGetOperatorSummary(void) {

    static char buffer[BUFFER_LEN];
    static const char spaces[]="                                        ";
    unsigned i, max_len, len;
    const riplOperator *ptr;
    char *buf_ptr;

    max_len=0;
    for (ptr=riplOperators, i=0; i<riplNumOperators; i++, ptr++) {
        if (strlen(ptr->name)>max_len) max_len=strlen(ptr->name);
    }
    buf_ptr=buffer;
    for (ptr=riplOperators, i=0; i<riplNumOperators; i++, ptr++) {
        len=strlen(ptr->name);
        if (ptr->match_chars) {
            buf_ptr+=sprintf(buf_ptr, "   %.*s(%s)",
                ptr->match_chars, ptr->name, ptr->name+ptr->match_chars);
            len+=2;
        }
        else buf_ptr+=sprintf(buf_ptr, "   %s", ptr->name);
        buf_ptr+=sprintf(buf_ptr, "%.*s%s\n",
            max_len+4-len, spaces, ptr->comment);
    }
    return buffer;
}

/* Display help for a specific operator. */
bool riplOperatorHelp(const char *op) {

    const riplOperator *ptr;
    const char *help_message;
    unsigned i;

    for (ptr=riplOperators, i=0; i<riplNumOperators; i++, ptr++) {
        if (riplMatch(op, ptr->name, ptr->match_chars)) {
            if (ptr->help) {
                help_message=ptr->help();
                if (help_message) {
                    riplMessage(itInfo,
                        RIPL_APPNAME " Version " RIPL_VERSION ", built " RIPL_DATE "\n"
                        RIPL_DESCRIPTION "\n"
                        "Written by " RIPL_AUTHOR "\n\n"
                        "Help for '%s':\n"
                        "Usage: " RIPL_EXENAME " " RIPL_CMDLINE " %s",
                        op, help_message);
                    return true;
                }
            }
            riplMessage(itInfo, "No help available for operator '%s'.\n", op);
            return true;
        }
    }
    riplMessage(itError, "Unrecognized operator '%s'!\n", op);
    return false;
}

/* Execute the next part of the command line. */
unsigned riplOperatorExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap) {

    const riplOperator *ptr;
    int args_read;
    unsigned i;

    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap)
    RIPL_VALIDATE(argv)

    for (ptr=riplOperators, i=0; i<riplNumOperators; i++, ptr++) {
        if (riplMatch(argv[0], ptr->name, ptr->match_chars)) {
            if (ptr->execute) {
                args_read=ptr->execute(argc-1, argv+1,
                    pinputGreyMap, poutputGreyMap);
                if (args_read<=RIPL_FIRSTERRORCODE) {
                    switch (args_read) {
                    case RIPL_EXECUTEERROR:
                        riplMessage(itError,
                            "An error occurred executing operator '%s'!\n",
                            ptr->name);
                        break;
                    case RIPL_USERERROR:
                    case RIPL_PARAMERROR:
                    default:
                        break;
                    }
                    return 0;
                }
                return args_read+1;
            }
            return 1;
        }
    }
    riplMessage(itError, "Unrecognized operator '%s'!\n", argv[0]);
    return 0;
}

