#pragma once

#include "Op.h"
#include "riplregistry/Registry.h"
#include "utillib/Trie.h"
#include <map>

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

    const std::map<std::string, Op>& ops() const { return m_ops; }

    const Op* findOpByPrefix(const std::string& prefix) const;

private:
    Trie<char, std::string> m_names;
    std::map<std::string, Op> m_ops;
};
