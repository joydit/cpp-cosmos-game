#pragma once

#include <Singleton.h>
#include "../BaseMessages.h"

class Messages: public BaseMessages, public Singleton <Messages>
{
public:
   void packInitialise(sf::Packet& packet);
   void load();
};
