#pragma once

#include <Singleton.h>
#include "../BaseEngine.h"
#include "Network.h"
#include "Client.h"

class Engine: public BaseEngine, public Singleton<Engine>
{
public:
   Client client;

   Engine();
   ~Engine();
   void init();
   bool update();
   void loadEvents();
   void loadCommands();

private:
   void handleExit();
};
