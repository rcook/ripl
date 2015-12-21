#pragma once

#include "oplib/register.h"
#include <functional>

/** Describes an operator */
struct riplOperator
{
    riplOperator(const char* name_, const char* comment_, ExecuteFunc executeFunc_, HelpFunc helpFunc_)
        : name(name_)
        , comment(comment_)
        , execute(executeFunc_)
        , help(helpFunc_)
    {
    }

    const char* name;
    const char* comment;
    ExecuteFunc execute;
    HelpFunc help;
};

const char* riplGetOperatorSummary(const std::vector<riplOperator>& ops);

bool riplOperatorHelp(
    const std::vector<riplOperator>& ops,
    const char* name);

unsigned riplOperatorExecute(
    const std::vector<riplOperator>& ops,
    unsigned argc,
    const char** argv,
    riplGreyMap* input,
    riplGreyMap* output);
