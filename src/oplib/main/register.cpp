#include "register.h"
#include "riplregistry/Registry.h"
#include <memory>

using namespace std;
using namespace ripl::oplib;

unique_ptr<OpInfoRegistry> g_opInfoRegistry;

RIPL_REGISTER_PLUGIN_OPS(registry)
{
    if (g_opInfoRegistry)
    {
        for (const auto& opInfo : g_opInfoRegistry->opInfos())
        {
            registry.registerOp(
                opInfo.name(),
                opInfo.description(),
                opInfo.executeFunc(),
                opInfo.helpFunc());
        }
    }
}

OpInfo::OpInfo(OpInfo&& other)
    : m_name(std::move(other.m_name))
    , m_description(std::move(other.m_description))
    , m_executeFunc(std::move(other.m_executeFunc))
    , m_helpFunc(std::move(other.m_helpFunc))
{
}

OpInfo::OpInfo(const char* name, const char* description, ExecuteFunc executeFunc, HelpFunc helpFunc)
    : m_name(name)
    , m_description(description)
    , m_executeFunc(executeFunc)
    , m_helpFunc(helpFunc)
{
}

void OpInfoRegistry::registerOp(OpInfo&& opInfo)
{
    if (!g_opInfoRegistry)
    {
        g_opInfoRegistry = make_unique<OpInfoRegistry>();
    }

    g_opInfoRegistry->m_opInfos.emplace_back(std::move(opInfo));
}

