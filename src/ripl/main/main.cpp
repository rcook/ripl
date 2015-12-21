#include "PluginManager.h"
#include "RegistryImpl.h"
#include "riplentr.h"
#include "plugin_helper.h"
#include "utillib/OSError.h"
#include "utillib/fs.h"
#include <iostream>
#include <queue>
#include <string>

using namespace std;

enum class CommandMode
{
    Invalid,
    ShowHelp,
    RunOperator
};

static bool isDebugCommand(const string& arg)
{
    return arg.compare("--debug") == 0;
}

static bool isPluginDirCommand(const string& arg)
{
    return arg.compare("--plugin-dir") == 0;
}

static bool isHelpCommand(const string& arg)
{
    return arg.compare("-h") == 0 ||
        arg.compare("--help") == 0 ||
        arg.compare("-?") == 0 ||
        arg.compare("?") == 0;
}

static int executeArgs(const string& executableFileName, queue<string>& args)
{
    string pluginDir;
    auto commandMode = CommandMode::Invalid;

    for (; !args.empty(); args.pop())
    {
        const auto& arg = args.front();

        if (isPluginDirCommand(arg))
        {
            args.pop();
            pluginDir = args.front();
        }
        else if (isHelpCommand(arg))
        {
            args.pop();
            commandMode = CommandMode::ShowHelp;
            break;
        }
        else
        {
            commandMode = CommandMode::RunOperator;
            break;
        }
    }

    vector<string> remainingArgs;
    for (; !args.empty(); args.pop())
    {
        remainingArgs.emplace_back(args.front());
    }

    if (pluginDir.empty())
    {
        pluginDir = joinPaths(getDir(executableFileName), "plugins");
    }

    PluginManager pluginManager;
    RegistryImpl registry;
    for (const auto& fileName : getCandidatePluginFileNames(pluginDir))
    {
        pluginManager.tryRegisterPluginOps(fileName, registry);
    }

    switch (commandMode)
    {
    case CommandMode::ShowHelp:
        if (remainingArgs.empty())
        {
            showHelp(registry);
        }
        else
        {
            for (const auto& name : remainingArgs)
            {
                auto iter = registry.ops().find(name);
                if (iter == registry.ops().end())
                {
                    cerr << "Unrecognized operator \"" << name << "\"" << endl;
                    return EXIT_FAILURE;
                }
                else
                {
                    showOpHelp(iter->second);
                }
            }
        }
        return EXIT_SUCCESS;

    case CommandMode::RunOperator:
        return runMain(registry, remainingArgs);

    default:
        showHelp(registry);
        return EXIT_FAILURE;
    }
}

int main(int argc, char* argv[])
{
    string executableFileName(argv[0]);
    queue<string> args;
    for (int i = 1; i < argc; ++i)
    {
        args.emplace(argv[i]);
    }

    if (!args.empty() && isDebugCommand(args.front()))
    {
        args.pop();
        executeArgs(executableFileName, args);
    }
    else
    {
        try
        {
            executeArgs(executableFileName, args);
        }
        catch (const OSError& ex)
        {
            cout
                << "Unhandled OS error:" << endl
                << "  code: " << ex.code() << endl
                << "  osFunctionName: " << ex.osFunctionName() << endl
                << "  message: " << ex.what() << endl;
        }
        catch (const runtime_error& ex)
        {
            cout << "Unhandled exception: " << ex.what() << endl;
        }
    }
}
