#pragma once

#include "riplregistry/Registry.h"
#include <memory>
#include <string>

class PluginManager
{
public:
    PluginManager(const PluginManager&) = delete;
    PluginManager& operator=(const PluginManager&) = delete;

public:
    PluginManager();
    ~PluginManager();

    bool tryRegisterPluginOps(const std::string& fileName, Registry& registry);

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};
