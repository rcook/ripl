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

#include "ripl.h"
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

static bool execute_arguments(
    const vector<riplOperator>& ops,
    unsigned argc,
    char **argv,
    riplGreyMap* input,
    riplGreyMap *output);

static bool help_arguments(
    const vector<riplOperator>& ops,
    unsigned argc,
    char **argv);

static void general_help(const vector<riplOperator>& ops);

class RegistrarImpl : public Registrar
{
public:
    RegistrarImpl(const RegistrarImpl&) = delete;
    RegistrarImpl& operator=(const RegistrarImpl&) = delete;

public:
    RegistrarImpl() = default;
    ~RegistrarImpl() = default;

    void registerOp(
        const char* name,
        const char* description,
        ExecuteFunc executeFunc,
        HelpFunc helpFunc) override;

    const vector<riplOperator>& ops() const { return m_ops; }

private:
    vector<riplOperator> m_ops;
};

void RegistrarImpl::registerOp(
    const char* name,
    const char* description,
    ExecuteFunc executeFunc,
    HelpFunc helpFunc)
{
    m_ops.emplace_back(name, description, executeFunc, helpFunc);
}

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
    RegistrarImpl registrar;
    oplib_registerOps(registrar);

    ResponseFileArgPtr respArgv;
    if (argc > 0 && **argv == '@')
    {
        // Take command line from a response file
        if (argc > 1)
        {
            general_help(registrar.ops());
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
        general_help(registrar.ops());
        return EXIT_FAILURE;
    }

    if (strcmp(argv[0], "?") == 0)
    {
        // User is requesting some help
        if (argc < 2)
        {
            general_help(registrar.ops());
            return EXIT_FAILURE;
        }
        else
        {
            if (!help_arguments(registrar.ops(), argc - 1, argv + 1))
            {
                return EXIT_FAILURE;
            }
        }
        return EXIT_SUCCESS;
    }

    if (argc < 3)
    {
        general_help(registrar.ops());
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
    bool result = execute_arguments(
        registrar.ops(),
        argc - 2,
        argv + 2,
        &inputGreyMap,
        &outputGreyMap);
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
    
    RegistrarImpl registrar;
    oplib_registerOps(registrar);

    ResponseFileArgPtr respArgv;
    if (argc > 0 && **argv == '@')
    {
        // Take command line from a response file
        if (argc > 1)
        {
            general_help(registrar.ops());
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
        general_help(registrar.ops());
        return EXIT_FAILURE;
    }

    if (strcmp(argv[0], "?") == 0)
    {
        // User is requesting some help
        if (argc < 2)
        {
            general_help(registrar.ops());
            return EXIT_FAILURE;
        }
        else
        {
            riplMessage(
                itInfo,
                RIPL_APPNAME " Version " RIPL_VERSION ", built " RIPL_DATE "\n"
                RIPL_DESCRIPTION "\n"
                "Written by " RIPL_AUTHOR "\n\n");
            if (!help_arguments(registrar.ops(), argc - 1, argv + 1))
            {
                return EXIT_FAILURE;
            }
        }
        return EXIT_SUCCESS;
    }

    // Execute command-line arguments
    if (!execute_arguments(registrar.ops(), argc, argv, pinputGreyMap, poutputGreyMap))
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
static bool execute_arguments(
    const vector<riplOperator>& ops,
    unsigned argc,
    char** argv,
    riplGreyMap *input,
    riplGreyMap *output)
{
    unsigned args_read;

    for (; ;)
    {
        args_read = riplOperatorExecute(
            ops,
            argc,
            const_cast<const char**>(argv),
            input,
            output);
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

        input->swap(*output);
    }
}

/* Displays help screens of operators specified on command line. */
static bool help_arguments(
    const vector<riplOperator>& ops,
    unsigned argc,
    char** argv)
{
    for (unsigned i = 0; i < argc; ++i)
    {
        if (!riplOperatorHelp(ops, argv[i]))
        {
            return false;
        }
    }

    return true;
}

/* Displays general help information. */
static void general_help(const vector<riplOperator>& ops)
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
        "Usage: " RIPL_EXENAME " ? <op>\n", riplGetOperatorSummary(ops));
}
