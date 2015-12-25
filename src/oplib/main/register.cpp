#include "register.h"
#include "ripllib/Registry.h"

using namespace std;
using namespace ripl::oplib;

struct OpInfo
{
    const char* name;
    const char* description;
    const ExecuteFunc executeFunc;
    const HelpFunc helpFunc;
};

// MSVC won't let us manage the vector<OpInfo> as a global due
// to initialization order, so we wrap it in this class instead
// and dynamically allocate and free it
class OpInfoContainer
{
public:
    OpInfoContainer(const OpInfoContainer&) = delete;
    OpInfoContainer& operator=(const OpInfoContainer&) = delete;

public:
    OpInfoContainer() = default;

    ~OpInfoContainer()
    {
        delete m_opInfos;
        m_opInfos = nullptr;
    }

    vector<OpInfo>& opInfos()
    {
        if (!m_opInfos)
        {
            m_opInfos = new vector<OpInfo>;
        }

        return *m_opInfos;
    }

private:
    vector<OpInfo>* m_opInfos;
} g_container;

namespace ripl { namespace oplib {

    void registerOp(
        const char* name,
        const char* description,
        ExecuteFunc executeFunc,
        HelpFunc helpFunc)
    {
        g_container.opInfos().emplace_back(OpInfo
        {
            name,
            description,
            executeFunc,
            helpFunc
        });
    }

}} // namespace ripl::oplib

RIPL_REGISTER_PLUGIN_OPS(registry)
{
    for (const auto& opInfo : g_container.opInfos())
    {
        registry.registerOp(
            opInfo.name,
            opInfo.description,
            opInfo.executeFunc,
            opInfo.helpFunc);
    }
}
