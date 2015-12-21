#include "riplentr.h"

#include "RegistryImpl.h"
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
    const unordered_map<string, Op>& ops,
    unsigned argc,
    char **argv,
    riplGreyMap* input,
    riplGreyMap *output);

int runMain(const RegistryImpl& registry, const vector<string>& args)
{
    using CharPtr = char*;

    // $TODO: Eliminate this ridiculous code
    vector<string> argsCopy(args);
    unique_ptr<CharPtr[]> pointers = unique_ptr<CharPtr[]>(new CharPtr[argsCopy.size()]);
    for (size_t i = 0; i < argsCopy.size(); ++i)
    {
        pointers[i] = const_cast<CharPtr>(args[i].data());
    }

    unsigned argc = argsCopy.size();
    char** argv = pointers.get();

    ResponseFileArgPtr respArgv;
    if (argc > 0 && **argv == '@')
    {
        // Take command line from a response file
        if (argc > 1)
        {
            showHelp(registry);
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

    if (argc < 3)
    {
        showHelp(registry);
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
        registry.ops(),
        argc - 2,
        argv + 2,
        &inputGreyMap,
        &outputGreyMap);
    riplSaveImage(argv[1], gfPGMBinary, outputGreyMap);

    // Free images
    return result ? EXIT_SUCCESS : EXIT_FAILURE;
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
    const unordered_map<string, Op>& ops,
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

void showHelp(const RegistryImpl& registry)
{
    const char* opSummary = riplGetOperatorSummary(registry.ops());

    riplMessage(
        itInfo,
        RIPL_APPNAME " Version " RIPL_VERSION ", built " RIPL_BUILD_DATE "\n"
        RIPL_DESCRIPTION "\n"
        "Written by " RIPL_AUTHOR "\n\n"
        "Usage: " RIPL_EXENAME " <input file> <output file> "
        "<op> (<args>) ...\n"
        "Or:    " RIPL_EXENAME " @<response file>\n\n"
        "Where <op> is one of the following:\n"
        "%s\n"
        "For help on a specific operation:\n\n"
        "Usage: " RIPL_EXENAME " ? <op>\n", opSummary);
}

void showOpHelp(const Op& op)
{
    const char* helpText = op.helpFunc()();

    riplMessage(
        itInfo,
        RIPL_APPNAME " Version " RIPL_VERSION ", built " RIPL_BUILD_DATE "\n"
        RIPL_DESCRIPTION "\n"
        "Written by " RIPL_AUTHOR "\n\n"
        "Help for '%s':\n"
        "Usage: " RIPL_EXENAME " " RIPL_CMDLINE " %s",
        op.name().data(),
        helpText);
}
