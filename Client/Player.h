#pragma once

#include "../BasePlayer.h"

class Player: public BasePlayer
{
public:
   Player(name_t name);
   bool isOnline();
};
