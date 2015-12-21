#include "Op.h"

using namespace std;

Op::Op(Op&& other)
    : m_name(move(other.m_name))
    , m_description(move(other.m_description))
    , m_executeFunc(move(other.m_executeFunc))
    , m_helpFunc(move(other.m_helpFunc))
{
}

Op::Op(const string& name, const string& description, ExecuteFunc executeFunc, HelpFunc helpFunc)
    : m_name(name)
    , m_description(description)
    , m_executeFunc(executeFunc)
    , m_helpFunc(helpFunc)
{
}
