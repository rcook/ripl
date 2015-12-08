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

Error ripl::error::OutOfMemory("Out of memory");
Error ripl::error::InvalidOperation("Invalid operation");
Error ripl::error::FileNotFound("File not found");
Error ripl::error::EndOfFile("End of file encountered");
Error ripl::error::IOError("File I/O error");
Error ripl::error::ImageTooBig("Image too big");
Error ripl::error::InvalidWidth("Invalid width");
Error ripl::error::InvalidHeight("Invalid height");
Error ripl::error::InvalidSize("Invalid size");
