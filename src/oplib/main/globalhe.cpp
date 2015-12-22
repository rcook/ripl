#include "globalhe.h"

#include "misc.h"
#include "register.h"
#include "validate.h"

namespace ripl { namespace oplib {

    void globalHE(riplGreyMap& output, const riplGreyMap& input)
    {
        RIPL_VALIDATE_ARG_NAME(input.hasSameDimensionsAs(output), "input");

        RIPL_REQUIRE(
            miscHistogramEQ(input.data(), output.data(), output.size()),
            error::InvalidOperation);
    }

    int globalHEExecute(riplGreyMap& output, const std::vector<std::string>& args, const riplGreyMap& input)
    {
        if (!args.empty())
        {
            riplMessage(
                itError,
                "Incorrect number of parameters!\n"
                "Usage: " RIPL_EXENAME " " RIPL_CMDLINE " nop\n");
            return RIPL_PARAMERROR;
        }

        globalHE(output, input);
        return RIPL_SUCCESS;
    }

    const char* globalHEHelp()
    {
        return "globalhe\n\n"
            "   Applies simplified global histogram equalization to the image.\n";
    }

    OPLIB_REGISTER_OP(
        globalhe,
        "apply global histogram equalization to image",
        wrapNewStyleExecuteFunc<globalHEExecute>,
        globalHEHelp);

}} // namespace ripl::oplib

