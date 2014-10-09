#include "Error.h"
#include "Shared/Console.h"

///Public
void Error::Print(const char* message)
{
   Console::Get().print(Console::STREAM_ERROR, message);
}

const char* Error::what() const throw()
{
   return message;
}
