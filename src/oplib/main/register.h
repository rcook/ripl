#pragma once

#include "ripllib/ExecuteFunc.h"
#include "ripllib/HelpFunc.h"
#include <vector>

#define OPLIB_REGISTER_OP(name, description, executeFunc, helpFunc) \
    static struct _oplib_##name \
    { \
        _oplib_##name() \
        { \
            ripl::oplib::registerOp( \
                #name, \
                description, \
                executeFunc, \
                helpFunc); \
        } \
    } \
    _oplib_##name##_instance

namespace ripl { namespace oplib {

    void registerOp(
        const char* name,
        const char* description,
        ExecuteFunc executeFunc,
        HelpFunc helpFunc);

    template<int(*executeFunc)(riplGreyMap&, const std::vector<std::string>&, const riplGreyMap&)>
    int wrapNewStyleExecuteFunc(unsigned argc, const char** argv, riplGreyMap* input, riplGreyMap* output)
    {
        return executeFunc(*output, std::vector<std::string>(argv, argv + argc), *input);
    }

}} // namespace ripl::oplib
