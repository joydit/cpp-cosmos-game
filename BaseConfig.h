/*
   Loads all config files to be easily accessible for engine initialisation.
*/

#pragma once

#include <Singleton.h>
#include <INI.h>

class BaseConfig: public Singleton<BaseConfig>
{
public:
   typedef INI<> console_t;
   typedef INI<> settings_t;

   console_t console;
   settings_t settings;

   BaseConfig();
   ~BaseConfig();
};
