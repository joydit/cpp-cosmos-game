/*
   Provides a class wrapper for single instanced objects.
*/

#pragma once

#include <assert.h>
#include <stdexcept>
#include <typeinfo>
//#include <cxxabi.h>

template <typename T>
   class Singleton
{
public:
   Singleton()
   {
      Instance = (T*)this;
      //abi::__cxa_demangle(typeid(T).name(), 0, 0, NULL);
   }

   static T& Get()
   {
      if (!ExistsInstance())
         throw std::runtime_error(std::string() + "Un-Instantiated singleton instance for type '" + typeid(T).name() + "'");

      return *Instance;
   }

protected:
   static void Set(T* instance)
   {
      Singleton::Instance = instance;
   }

private:
   static T* Instance;

   static bool ExistsInstance()
   {
      return Instance != NULL;
   }
};

template <typename T>
   T* Singleton<T>::Instance = NULL;
