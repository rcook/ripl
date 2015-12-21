#pragma once

#include "Op.h"
#include "riplregistry/Registry.h"
#include "utillib/Trie.h"
#include <unordered_map>

class RegistryImpl : public Registry
{
public:
    RegistryImpl() = default;

public:
    void registerOp(
        const char* name,
        const char* description,
        ExecuteFunc executeFunc,
        HelpFunc helpFunc) override;

    const std::unordered_map<std::string, Op>& ops() const { return m_ops; }

    int execute(
        const std::string& command,
        unsigned argc,
        const char** argv,
        riplGreyMap* input,
        riplGreyMap* output) const;

private:
    Trie<char, std::string> m_names;
    std::unordered_map<std::string, Op> m_ops;
};
