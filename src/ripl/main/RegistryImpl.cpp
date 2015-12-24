#include "RegistryImpl.h"
#include "utillib/validate.h"

using namespace std;

void RegistryImpl::registerOp(
    const char* name,
    const char* description,
    ExecuteFunc executeFunc,
    HelpFunc helpFunc)
{
    XYZ_REQUIRE(m_names.insert(name));

    m_ops.emplace(name, Op(name, description, executeFunc, helpFunc));
}

const Op* RegistryImpl::findOpByPrefix(const string& prefix) const
{
    const auto* name = m_names.findByPrefix(prefix);
    if (!name)
    {
        return nullptr;
    }

    return &m_ops.at(*name);
}
