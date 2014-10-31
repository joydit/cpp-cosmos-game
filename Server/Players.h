#pragma once

#include <list>
#include <Singleton.h>
#include "../BasePlayers.h"
#include "Player.h"

class Players: public BasePlayers, public Singleton<Players>
{
public:
   Players();
};
