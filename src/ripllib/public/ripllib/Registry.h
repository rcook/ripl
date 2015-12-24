#pragma once

#include "common.h"
#include "ripllib/ExecuteFunc.h"
#include "ripllib/HelpFunc.h"
#include <string>

#define RIPL_REGISTER_PLUGIN_OPS_FUNCTION_NAME "__riplRegisterPluginOps"
#define RIPL_REGISTER_PLUGIN_OPS(__registry) \
    extern "C" DLLEXPORT void __riplRegisterPluginOps(Registry& __registry)

class Registry
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
