#pragma once

#include <Singleton.h>
#include "../BaseMessages.h"

class Messages: public BaseMessages, public Singleton <Messages>
{
public:
   void unpackInitialise(sf::Packet& packet);
   void load();
};
