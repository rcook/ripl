#include "Error.h"

using namespace std;
using namespace ripl::error;

Error::Error(const string& message)
    : m_message(message)
{
}

const string& Error::message() const
{
    return m_message;
}

Error ripl::error::InvalidWidth("Invalid width");
Error ripl::error::InvalidHeight("Invalid height");
Error ripl::error::InvalidSize("Invalid size");
