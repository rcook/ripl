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

#include "ripldbug.h"
#include "riplop.h"
#include "riplmsg.h"
#include "riplpars.h"
#include <cstdio>
#include <cstring>

using namespace std;

/* Constants for local use only. */
#define BUFFER_LEN		2048

/* Return list of all operators and one-line help. */
const char* riplGetOperatorSummary(const vector<riplOperator>& ops)
{
    static char buffer[BUFFER_LEN];
    static const char spaces[]="                                        ";
    unsigned max_len;
    char *buf_ptr;

    max_len=0;
    for (const auto& op : ops)
    {
        if (strlen(op.name) > max_len)
        {
            max_len = strlen(op.name);
        }
    }

    buf_ptr=buffer;
    for (const auto& op: ops)
    {
        unsigned len = strlen(op.name);

        int prefixLength = 0; // $TODO: Figure this out using trie
        if (prefixLength > 0)
        {
            buf_ptr += sprintf(
                buf_ptr,
                "   %.*s(%s)",
                prefixLength,
                op.name,
                op.name + prefixLength);
            len+=2;
        }
        else
        {
            buf_ptr += sprintf(buf_ptr, "   %s", op.name);
        }

        buf_ptr += sprintf(buf_ptr, "%.*s%s\n", max_len + 4 - len, spaces, op.comment);
    }

    return buffer;
}

/* Display help for a specific operator. */
bool riplOperatorHelp(const vector<riplOperator>& ops, const char *name)
{
    for (const auto& op : ops)
    {
        if (riplMatch(name, op.name, 0/*prefixLength*/))
        {
            if (op.help)
            {
                const char* help_message = op.help();
                if (help_message) {
                    riplMessage(itInfo,
                        RIPL_APPNAME " Version " RIPL_VERSION ", built " RIPL_DATE "\n"
                        RIPL_DESCRIPTION "\n"
                        "Written by " RIPL_AUTHOR "\n\n"
                        "Help for '%s':\n"
                        "Usage: " RIPL_EXENAME " " RIPL_CMDLINE " %s",
                        name,
                        help_message);
                    return true;
                }
            }
            riplMessage(itInfo, "No help available for operator '%s'.\n", name);
            return true;
        }
    }
    riplMessage(itError, "Unrecognized operator '%s'!\n", name);
    return false;
}

/* Execute the next part of the command line. */
unsigned riplOperatorExecute(
    const vector<riplOperator>& ops,
    unsigned argc,
    const char** argv,
    riplGreyMap* input,
    riplGreyMap* output)
{
    int args_read;

    RIPL_VALIDATE_OP_GREYMAPS(input, output)
    RIPL_VALIDATE(argv)

    for (const auto& op : ops)
    {
        if (riplMatch(argv[0], op.name, 0/*prefixLength*/))
        {
            if (op.execute)
            {
                args_read = op.execute(argc - 1, argv + 1, input, output);
                if (args_read <= RIPL_FIRSTERRORCODE)
                {
                    switch (args_read) {
                    case RIPL_EXECUTEERROR:
                        riplMessage(itError,
                            "An error occurred executing operator '%s'!\n",
                            op.name);
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

