#include "riplentr.h"

#include "RegistryImpl.h"
#include "ripl.h"
#include "Error.h"
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>

using namespace std;
using namespace ripl;

using ResponseFileArgPtr = unique_ptr<char*, function<void(char**)>>;

static ResponseFileArgPtr parseResponseFile(
    const char* fileName,
    unsigned* argCount)
{
    return ResponseFileArgPtr(
        riplParseResponseFile(fileName, argCount),
        [](char** p) { riplFree(p); });
}

// Execute all operators specified on command line in order
// passing the output from each operator as the input to the next
static bool executePipeline(
    const RegistryImpl& registry,
    unsigned argc,
    const char** argv,
    riplGreyMap *input,
    riplGreyMap *output)
{
    for (; ;)
    {
        const char* command = argv[0];
        const Op* op = registry.findOpByPrefix(command);
        if (!op)
        {
            cerr << "Unrecognized operator \"" << command << "\"" << endl;
            return false;
        }

        RIPL_VALIDATE_OP_GREYMAPS(input, output);
        RIPL_VALIDATE(argv);

        auto result = op->execute(argc - 1, argv + 1, input, output);
        if (result <= RIPL_FIRSTERRORCODE)
        {
            switch (result)
            {
            case RIPL_EXECUTEERROR:
                riplMessage(
                    itError,
                    "An error occurred executing operator '%s'!\n",
                    op->name().data());
                return false;

            case RIPL_USERERROR:
            case RIPL_PARAMERROR:
            default:
                return false;
            }
        }

        argv += result + 1;
        argc -= result + 1;
        if (argc < 1)
        {
            return true;
        }

        input->swap(*output);
    }
}

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
    bool result = executePipeline(
        registry,
        argc - 2,
        const_cast<const char**>(argv + 2),
        &inputGreyMap,
        &outputGreyMap);
    riplSaveImage(argv[1], gfPGMBinary, outputGreyMap);

    // Free images
    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}

void showHelp(const RegistryImpl& registry)
{
    unsigned maximumNameLength = 0;
    for (const auto& pair : registry.ops())
    {
        auto nameLength = pair.second.name().size();
        if (nameLength > maximumNameLength)
        {
            maximumNameLength = nameLength;
        }
    }

    ostringstream os;
    for (const auto& pair : registry.ops())
    {
        os << "  " << left << setw(maximumNameLength) << pair.second.name();
        os << "  " << pair.second.description();
        os << "\n";
    }

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
        "Usage: " RIPL_EXENAME " ? <op>\n",
        os.str().data());
}

void showOpHelp(const Op& op)
{
    auto helpText = op.renderHelp();

    riplMessage(
        itInfo,
        RIPL_APPNAME " Version " RIPL_VERSION ", built " RIPL_BUILD_DATE "\n"
        RIPL_DESCRIPTION "\n"
        "Written by " RIPL_AUTHOR "\n\n"
        "Help for '%s':\n"
        "Usage: " RIPL_EXENAME " " RIPL_CMDLINE " %s",
        op.name().data(),
        helpText.data());
}
