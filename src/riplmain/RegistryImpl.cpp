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

int RegistryImpl::execute(
    const string& command,
    unsigned argc,
    const char** argv,
    riplGreyMap* input,
    riplGreyMap* output) const
{
    const auto* name = m_names.findByPrefix(command);
    XYZ_REQUIRE(name);

    const auto& op = m_ops.at(*name);
    return op.executeFunc()(argc, argv, input, output);
}
