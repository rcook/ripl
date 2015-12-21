#pragma once

#include "riplregistry/ExecuteFunc.h"
#include "riplregistry/HelpFunc.h"
#include "riplregistry/defs.h"
#include <string>

#define RIPL_REGISTER_PLUGIN_OPS_FUNCTION_NAME "__riplRegisterPluginOps"
#define RIPL_REGISTER_PLUGIN_OPS(__registry) \
    extern "C" DLLEXPORT void __riplRegisterPluginOps(Registry& __registry)

class RIPLREGISTRY_API Registry
{
protected:
    Registry();

public:
    virtual void registerOp(
        const char* name,
        const char* description,
        ExecuteFunc executeFunc,
        HelpFunc helpFunc) = 0;
};

using RegisterPluginOpsFunc = void(*)(Registry&);
