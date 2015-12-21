#pragma once

#include "riplregistry/ExecuteFunc.h"
#include "riplregistry/HelpFunc.h"
#include "riplregistry/Registry.h"

class Op
{
public:
    Op() = delete;
    Op(const Op&) = delete;
    Op& operator=(const Op&) = delete;

public:
    Op(Op&& other);
    Op(const std::string& name, const std::string& description, ExecuteFunc executeFunc, HelpFunc helpFunc);

    const std::string& name() const { return m_name; }
    const std::string& description() const { return m_description; }
    int execute(unsigned argc, const char** argv, riplGreyMap* input, riplGreyMap* output) const;
    std::string renderHelp() const;

private:
    const std::string m_name;
    const std::string m_description;
    const ExecuteFunc m_executeFunc;
    const HelpFunc m_helpFunc;
};
