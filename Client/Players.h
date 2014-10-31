#pragma once

#include <Singleton.h>
#include "../BasePlayers.h"

class Players: public BasePlayers, public Singleton<Players>
{
public:
   Players();
};
