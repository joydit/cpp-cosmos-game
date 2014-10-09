/*
   Generic function delegate.

   Functions are stored typeless, parameters and return type's are deduced from template arguments provided when called.
*/

#pragma once

class Function
{
public:
   Function(void* funct = NULL): funct(funct)
   {

   }

   Function& operator= (const Function& other)
   {
      funct = other.funct;
   }

   Function& operator= (const void* funct)
   {
      this->funct = funct;
   }

   template <typename T, typename... Args>
      T call(Args... args)
   {
      T (*funct)(Args...);
      funct = this->funct;
      funct(args...);
   }

public:
   void* funct;
};
