#pragma once

#include "riplregistry/ExecuteFunc.h"
#include "riplregistry/HelpFunc.h"
#include <vector>

#define OPLIB_REGISTER_OP(name, description, executeFunc, helpFunc) \
    static struct _oplib_##name \
    { \
        _oplib_##name() \
        { \
            ripl::oplib::OpInfoRegistry::registerOp(ripl::oplib::OpInfo( \
                #name, \
                description, \
                executeFunc, \
                helpFunc)); \
        } \
    } \
    _oplib_##name##_instance

namespace ripl { namespace oplib {

    class OpInfo
    {
    public:
        OpInfo() = delete;
        OpInfo(const OpInfo&) = delete;
        OpInfo& operator=(const OpInfo&) = delete;

    public:
        ~OpInfo() = default;

        OpInfo(OpInfo&& other);
        OpInfo(const char* name, const char* description, ExecuteFunc executeFunc, HelpFunc helpFunc);

        const char* name() const { return m_name; }
        const char* description() const { return m_description; }
        ExecuteFunc executeFunc() const { return m_executeFunc; }
        HelpFunc helpFunc() const { return m_helpFunc; }

    private:
        const char* m_name;
        const char* m_description;
        const ExecuteFunc m_executeFunc;
        const HelpFunc m_helpFunc;
    };

    class OpInfoRegistry
    {
    public:
        OpInfoRegistry(const OpInfoRegistry&) = delete;
        OpInfoRegistry& operator=(const OpInfoRegistry&) = delete;

    public:
        OpInfoRegistry() = default;
        ~OpInfoRegistry() = default;

        static void registerOp(OpInfo&& opInfo);

        const std::vector<OpInfo>& opInfos() const { return m_opInfos; }

    private:
        std::vector<OpInfo> m_opInfos;
    };

    template<int(*executeFunc)(riplGreyMap&, const std::vector<std::string>&, const riplGreyMap&)>
    int wrapNewStyleExecuteFunc(unsigned argc, const char** argv, riplGreyMap* input, riplGreyMap* output)
    {
        return executeFunc(*output, std::vector<std::string>(argv, argv + argc), *input);
    }

}} // namespace ripl::oplib
