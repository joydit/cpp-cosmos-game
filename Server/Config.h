#pragma once

#include "../BaseConfig.h"

class Config: public BaseConfig, public Singleton<Config>
{
public:
   Config();
   ~Config();
};
