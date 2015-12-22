#include "add.h"

#include "register.h"
#include "riplfilt.h"
#include "riplmsg.h"
#include "riplpars.h"

using namespace std;
using namespace ripl;

namespace ripl { namespace oplib {

    void add(riplGreyMap& output, const riplGreyMap& input0, const riplGreyMap& input1, float weight)
    {
        RIPL_VALIDATE_ARG_NAME(input0.hasSameDimensionsAs(output), "input0");
        RIPL_VALIDATE_ARG_NAME(input1.hasSameDimensionsAs(output), "input1");

        auto input0Iter = input0.begin();
        auto input1Iter = input1.begin();
        auto outputIter = output.begin();
        for (image_size_t i = 0; i < input0.size(); i++, ++input0Iter, ++input1Iter, ++outputIter)
        {
            auto value0 = *input0Iter;
            auto value1 = *input1Iter;
            float temp = value0 + weight * (value1);

            if (temp < 0)
            {
                *outputIter = 0;
            }
            else if (temp > RIPL_MAX_GREY)
            {
                *outputIter = RIPL_MAX_GREY;
            }
            else
            {
                *outputIter = static_cast<riplGrey>(temp);
            }
        }
    }

    static int addExecute(riplGreyMap& output, const vector<string>& args, const riplGreyMap& input)
    {
        if (args.size() < 2)
        {
            riplMessage(itError, "Incorrect number of parameters!\n"
                "Usage: " RIPL_EXENAME " " RIPL_CMDLINE " add <weight> <file>\n");
            return RIPL_PARAMERROR;
        }

        const auto& weightStr = args[0];
        const auto& fileName = args[1];

        float weight;
        if (!riplArgGet_float(weightStr.data(), &weight))
        {
            riplMessage(itError, "add: <weight> should be a float.\n");
            return RIPL_PARAMERROR;
        }

        auto otherInput = riplLoadImage(fileName.data());

        add(output, input, otherInput, weight);
        return RIPL_SUCCESS;
    }

    static const char* addHelp()
    {
        return
            " add <weight> <file-name>\n\n"
            "   Adds a version of the image in <file-name> weighted by the\n"
            "   factor <weight> to the input image pixel by pixel. The\n"
            "   input image and image in <file-name> must have the same\n"
            "   pixel dimensions.\n";
    }

    OPLIB_REGISTER_OP(
        add,
        "add images together pixel by pixel",
        wrapNewStyleExecuteFunc<addExecute>,
        addHelp);

}} // namespace ripl::oplib
