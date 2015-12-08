/*
 *		add.c
 *		Source file for adding operator.
 *
 *		Version 1.1, last update: 24 January 1998.
 *
 *		History:
            24/1/98:		introduced RIPL_PARAMERROR.
 *			16/12/97:	updated comments.
 *			15/12/97:	first implemented.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */
#include "add.h"

void addImages(
    riplGreyMap& output,
    const riplGreyMap& input0,
    const riplGreyMap& input1,
    float weight)
{
    const riplGrey* p0 = input0.data;
    const riplGrey* p1 = input1.data;
    riplGrey* outPtr = output.data;
    for (decltype(input0.size) i = 0; i < input0.size; i++, ++p0, ++p1, ++outPtr)
    {
        auto value0 = *p0;
        auto value1 = *p1;
        float temp = value0 + weight * (value1);

        if (temp < 0)
        {
            *outPtr = 0;
        }
        else if (temp > RIPL_MAX_GREY)
        {
            *outPtr = RIPL_MAX_GREY;
        }
        else
        {
            *outPtr = static_cast<riplGrey>(temp);
        }
    }
}

/* Internal entrypoint. */
bool addApplyOperator(
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap,
    float weight,
    const char* fileName)
{
    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap);
    RIPL_VALIDATE_ARG_NAME(fileName, "fileName");

    if (!riplFileExists(fileName))
    {
        riplMessage(itError, "add: Input file %s does not exist!\n");
        return false;
    }

    // Load input image
    auto addGreyMap = riplLoadImage(fileName);
    if (addGreyMap.cols != pinputGreyMap->cols || addGreyMap.rows!=pinputGreyMap->rows)
    {
        riplMessage(itError, "add: Images do not have the same dimensions!\n");
        return false;
    }

    /* Images are valid. */
    addImages(*poutputGreyMap, *pinputGreyMap, addGreyMap, weight);

    /* Deallocate greymap and its data. */
    riplFree(addGreyMap.data);
    return true;
}

/* Command-line version. */
int addExecute(unsigned argc,
    const char **argv,
    riplGreyMap *pinputGreyMap,
    riplGreyMap *poutputGreyMap) {

    float weight;

    if (argc<2) {
        riplMessage(itError, "Incorrect number of parameters!\n"
            "Usage: " RIPL_EXENAME " " RIPL_CMDLINE " add <weight> <file>\n");
        return RIPL_PARAMERROR;
    }
    if (!riplArgGet_float(argv[0], &weight)) {
        riplMessage(itError, "add: <weight> should be a float.\n");
        return RIPL_PARAMERROR;
    }
    if (!addApplyOperator(pinputGreyMap,
        poutputGreyMap,
        weight,
        argv[1]))
        return RIPL_EXECUTEERROR;
    return 2;
}

/* Display help screen. */
const char *addHelp(void) {
    return " add <weight> <file>\n\n"
        "   Adds a version of the image in <file> weighted by the\n"
        "   factor <weight> to the input image pixel by pixel. The\n"
        "   input image and image in <file> must have the same\n"
        "   pixel dimensions.\n";
}

