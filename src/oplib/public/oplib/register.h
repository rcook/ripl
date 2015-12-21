#pragma once

#include "riplgrey.h"
#include "riplregistry/ExecuteFunc.h"
#include "riplregistry/HelpFunc.h"
#include <functional>

class Registrar
{
public:
    virtual ~Registrar() = default;

    virtual void registerOp(
        const char* name,
        const char* description,
        ExecuteFunc executeFunc,
        HelpFunc helpFunc) = 0;
};

void oplib_registerOps(Registrar& registrar);
