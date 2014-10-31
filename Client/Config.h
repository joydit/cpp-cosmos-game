#pragma once

#include <Singleton.h>
#include "../BaseConfig.h"

class Config: public BaseConfig, public Singleton<Config>
{
public:
   Config();
   ~Config();
};
