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
    void registerOp(const std::string& name, OpFunc func) override;
    const std::unordered_map<std::string, Op>& ops() const { return m_ops; }
    int invoke(const std::string& command, int x, int y) const;

private:
    Trie<char, std::string> m_names;
    std::unordered_map<std::string, Op> m_ops;
};
