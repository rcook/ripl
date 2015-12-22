#include "nop.h"

#include "register.h"
#include <cstring>

using namespace std;
using namespace ripl;

namespace ripl { namespace oplib {

    void nop(riplGreyMap& output, const riplGreyMap& input)
    {
        RIPL_VALIDATE_ARG_NAME(input.hasSameDimensionsAs(output), "input");

        // Just copy straight from input to output
        memcpy(
            output.data(),
            input.data(),
            input.size() * sizeof(riplGrey));
    }

    int nopExecute(riplGreyMap& output, const vector<string>& args, const riplGreyMap& input)
    {
        if (!args.empty())
        {
            riplMessage(
                itError,
                "Incorrect number of parameters!\n"
                "Usage: " RIPL_EXENAME " " RIPL_CMDLINE " nop\n");
            return RIPL_PARAMERROR;
        }

        nop(output, input);
        return RIPL_SUCCESS;
    }

    const char* nopHelp()
    {
        return " nop\n\n"
            "   Do nothing: copy input image to output.\n";
    }

    OPLIB_REGISTER_OP(
        nop,
        "no-op filter",
        wrapNewStyleExecuteFunc<nopExecute>,
        nopHelp);

}} // namespace ripl::oplib

