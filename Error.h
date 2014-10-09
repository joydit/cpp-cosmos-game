/*
   Throwable error class with recovery levels and formattable messages with values.
   Additionally providing a method to print an error to the correct console stream.
*/

#pragma once

#include <stdio.h>
#include <exception>
#include <typeinfo>
#include <string>

class Error: public std::exception
{
public:
   enum recover_e { RECOVER_NOTHING, RECOVER_SAVE };

   template <typename... Args>
      static void Print(const char* message, Args... args);
   static void Print(const char* message);

   template <typename... Args>
      Error(const char* message, Args... args);
   template <typename... Args>
      Error(const char* message, recover_e recover, Args... args);
   const char* what() const throw();

private:
   char message[256];
};

///Public
template <typename... Args>
   void Error::Print(const char* message, Args... args)
{
   char buffer[256];
   sprintf(buffer, message, args...);
   Print(buffer);
}

template <typename... Args>
   Error::Error(const char* message, Args... args): Error(message, RECOVER_NOTHING, args...)
{

}

template <typename... Args>
   Error::Error(const char* message, recover_e recover, Args... args)
{
   sprintf(this->message, message, args...);
}
