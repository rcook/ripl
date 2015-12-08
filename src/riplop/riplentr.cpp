/*
 *		RIPL---Richard's Image-Processing Library.
 *		Written by Richard Cook.
 *
 *		riplentr.c
 *		Main source file for main Ripl entrypoint.
 *
 *		Version 1.1, last update: 3 February 1998.
 *
 *		History:
 *			3/1/98:			made cosmetic changes to help screens.
 *			24/1/98:		fixed response file bug.
 *			24/1/98:		renamed RIPL_PARSEERROR to RIPL_PARAMERROR.
 *			20/1/98:		version 1.1 (renamed riplentr).
 *			31/12/97:	modified included file list.
 *			16/12/97:	modified execute_arguments.
 *			27/11/97:	last major update.
 *
 *		Copyright © 1997/8, Richard A. Cook.
 */

#include "riplop.h"
#include "Error.h"
#include <cstring>
#include <functional>
#include <memory>

using namespace std;
using namespace ripl;

/* Prototypes of static functions. */
using ResponseFileArgPtr = unique_ptr<char*, function<void(char**)>>;

static ResponseFileArgPtr parseResponseFile(const char* fileName, unsigned* argCount);

static bool execute_arguments(unsigned argc, char **argv, riplGreyMap *pinputGreyMap, riplGreyMap *poutputGreyMap);

static bool help_arguments(unsigned argc, char **argv);

static void general_help();

/*
 *		riplMain1
 *		Main (command-line) entrypoint for programme.
 *		Function parses command-line arguments and response-file
 *		input as necessary. It also loads and saves the input and
 *		output image files. DO NOT PASS EXECUTABLE NAME AS
 *		FIRST ARGUMENT!
 */
int riplMain1(unsigned argc, char **argv)
{
    ResponseFileArgPtr respArgv;
    if (argc > 0 && **argv == '@')
    {
        // Take command line from a response file
        if (argc > 1)
        {
            general_help();
            return EXIT_FAILURE;
        }

        if (!riplFileExists(*argv + 1))
        {
            riplMessage(itError, "Response file %s does not exist!\n", *argv + 1);
            return EXIT_FAILURE;
        }

        respArgv = parseResponseFile(*argv + 1, &argc);
        if (!respArgv)
        {
            riplMessage(itError,
                "Error reading response file %s!\n"
                "[File error, line too long or file too long]\n", *argv + 1);
            return EXIT_FAILURE;
        }

        argv = respArgv.get();
    }

    if (argc == 0)
    {
        // No arguments supplied
        general_help();
        return EXIT_FAILURE;
    }

    if (strcmp(argv[0], "?") == 0)
    {
        // User is requesting some help
        if (argc < 2)
        {
            general_help();
            return EXIT_FAILURE;
        }
        else
        {
            if (!help_arguments(argc - 1, argv + 1))
            {
                return EXIT_FAILURE;
            }
        }
        return EXIT_SUCCESS;
    }

    if (argc < 3)
    {
        general_help();
        return EXIT_FAILURE;
    }

    if (!riplFileExists(argv[0]))
    {
        riplMessage(itError, "Image file %s does not exist!\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Load input image
    auto inputGreyMap = riplLoadImage(argv[0]);

    // Allocate another image of the same size as the input
    riplGreyMap outputGreyMap(inputGreyMap.width(), inputGreyMap.height());

    // Execute command-line arguments
    bool result = execute_arguments(argc - 2, argv + 2, &inputGreyMap, &outputGreyMap);
    riplSaveImage(argv[1], gfPGMBinary, outputGreyMap);

    // Free images
    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}

/*
 *		riplMain2
 *		Alternative entrypoint for programme.
 *		Function parses command-line arguments and response-file
 *		input as necessary. It also loads and saves the input and
 *		output image files. Input and output greymaps are passed
 *		as additional arguments. DO NOT PASS EXECUTABLE NAME AS
 *		FIRST ARGUMENT!
 */
int riplMain2(unsigned argc, char** argv, riplGreyMap* pinputGreyMap, riplGreyMap* poutputGreyMap)
{
    RIPL_VALIDATE_OP_GREYMAPS(pinputGreyMap, poutputGreyMap);
    
    ResponseFileArgPtr respArgv;
    if (argc > 0 && **argv == '@')
    {
        // Take command line from a response file
        if (argc > 1)
        {
            general_help();
            return EXIT_FAILURE;
        }

        if (!riplFileExists(*argv + 1))
        {
            riplMessage(itError, "Response file %s does not exist!\n", *argv + 1);
            return EXIT_FAILURE;
        }

        respArgv = parseResponseFile(*argv + 1, &argc);
        if (!respArgv)
        {
            riplMessage(itError,
                "Error reading response file %s!\n"
                "[File error, line too long or file too long]\n", *argv + 1);
            return EXIT_FAILURE;
        }

        argv = respArgv.get();
    }

    if (argc == 0)
    {
        // No arguments supplied
        general_help();
        return EXIT_FAILURE;
    }

    if (strcmp(argv[0], "?") == 0)
    {
        // User is requesting some help
        if (argc < 2)
        {
            general_help();
            return EXIT_FAILURE;
        }
        else
        {
            riplMessage(
                itInfo,
                RIPL_APPNAME " Version " RIPL_VERSION ", built " RIPL_DATE "\n"
                RIPL_DESCRIPTION "\n"
                "Written by " RIPL_AUTHOR "\n\n");
            if (!help_arguments(argc - 1, argv + 1))
            {
                return EXIT_FAILURE;
            }
        }
        return EXIT_SUCCESS;
    }

    // Execute command-line arguments
    if (!execute_arguments(argc, argv, pinputGreyMap, poutputGreyMap))
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static ResponseFileArgPtr parseResponseFile(
    const char* fileName,
    unsigned* argCount)
{
    return ResponseFileArgPtr(
        riplParseResponseFile(fileName, argCount),
        [](char** p) { riplFree(p); });
}

/*
 *		Function takes the remaining command-line arguments and
 *		parses them further---executing the relevant operators,
 *		passing pointers to the input and output greymaps.
 *		This function also multiple operators to be specified on
 *		the single command line and passes the output of each operator
 *		as the input to the input of the next, allowing several
 *		transforms to be applied to a single image.
 */
static bool execute_arguments(unsigned argc, char** argv, riplGreyMap* pinputGreyMap, riplGreyMap* poutputGreyMap)
{
    unsigned args_read;

    while (1)
    {
        args_read = riplOperatorExecute(
            argc,
            const_cast<const char**>(argv),
            pinputGreyMap,
            poutputGreyMap);
        if (args_read < 1)
        {
            return false;
        }

        argv += args_read;
        argc -= args_read;
        if (argc < 1)
        {
            return true;
        }

        pinputGreyMap->swap(*poutputGreyMap);
    }
}

/* Displays help screens of operators specified on command line. */
static bool help_arguments(unsigned argc, char** argv)
{
    for (unsigned i = 0; i < argc; ++i)
    {
        if (!riplOperatorHelp(argv[i]))
        {
            return false;
        }
    }

    return true;
}

/* Displays general help information. */
static void general_help()
{
    riplMessage(
        itInfo,
        RIPL_APPNAME " Version " RIPL_VERSION ", built " RIPL_DATE "\n"
        RIPL_DESCRIPTION "\n"
        "Written by " RIPL_AUTHOR "\n"
        "Signal Processing and Communications Group\n"
        "Cambridge University Engineering Department\n\n"
        "Usage: " RIPL_EXENAME " <input file> <output file> "
        "<op> (<args>) ...\n"
        "Or:    " RIPL_EXENAME " @<response file>\n\n"
        "Where <op> is one of the following:\n"
        "%s\n"
        "For help on a specific operation:\n\n"
        "Usage: " RIPL_EXENAME " ? <op>\n", riplGetOperatorSummary());
}
