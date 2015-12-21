#include "riplop.h"

#include "Op.h"
#include "ripldbug.h"
#include "riplmsg.h"
#include "riplpars.h"
#include <cstdio>
#include <cstring>

using namespace std;

/* Constants for local use only. */
#define BUFFER_LEN		2048

/* Return list of all operators and one-line help. */
const char* riplGetOperatorSummary(const unordered_map<string, Op>& ops)
{
    static char buffer[BUFFER_LEN];
    static const char spaces[]="                                        ";
    unsigned max_len;
    char *buf_ptr;

    max_len=0;
    for (const auto& pair : ops)
    {
        if (pair.second.name().size() > max_len)
        {
            max_len = pair.second.name().size();
        }
    }

    buf_ptr=buffer;
    for (const auto& pair : ops)
    {
        unsigned len = pair.second.name().size();

        int prefixLength = 0; // $TODO: Figure this out using trie
        if (prefixLength > 0)
        {
            buf_ptr += sprintf(
                buf_ptr,
                "   %.*s(%s)",
                prefixLength,
                pair.second.name().data(),
                pair.second.name().data() + prefixLength);
            len+=2;
        }
        else
        {
            buf_ptr += sprintf(buf_ptr, "   %s", pair.second.name().data());
        }

        buf_ptr += sprintf(buf_ptr, "%.*s%s\n", max_len + 4 - len, spaces, pair.second.description().data());
    }

    return buffer;
}

/* Display help for a specific operator. */
bool riplOperatorHelp(const unordered_map<string, Op>& ops, const char* name)
{
    for (const auto& pair : ops)
    {
        if (riplMatch(name, pair.second.name().data(), 0/*prefixLength*/))
        {
            riplMessage(itInfo,
                RIPL_APPNAME " Version " RIPL_VERSION ", built " RIPL_BUILD_DATE "\n"
                RIPL_DESCRIPTION "\n"
                "Written by " RIPL_AUTHOR "\n\n"
                "Help for '%s':\n"
                "Usage: " RIPL_EXENAME " " RIPL_CMDLINE " %s",
                name,
                pair.second.helpFunc()());
            return true;
        }
    }
    riplMessage(itError, "Unrecognized operator '%s'!\n", name);
    return false;
}

/* Execute the next part of the command line. */
unsigned riplOperatorExecute(
    const unordered_map<string, Op>& ops,
    unsigned argc,
    const char** argv,
    riplGreyMap* input,
    riplGreyMap* output)
{
    int args_read;

    RIPL_VALIDATE_OP_GREYMAPS(input, output)
    RIPL_VALIDATE(argv)

    for (const auto& pair : ops)
    {
        if (riplMatch(argv[0], pair.second.name().data(), 0/*prefixLength*/))
        {
            args_read = pair.second.executeFunc()(argc - 1, argv + 1, input, output);
            if (args_read <= RIPL_FIRSTERRORCODE)
            {
                switch (args_read) {
                case RIPL_EXECUTEERROR:
                    riplMessage(itError,
                        "An error occurred executing operator '%s'!\n",
                        pair.second.name().data());
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
    }
    riplMessage(itError, "Unrecognized operator '%s'!\n", argv[0]);
    return 0;
}
