#pragma once

#include <Singleton.h>
#include "../BaseEngine.h"

class Engine: public BaseEngine, public Singleton<Engine>
{
public:
   Clients clients;

   Engine();
   ~Engine();
   void init();
   void free();
   bool update();
   void loadEvents();
   void loadCommands();

private:
   void handleExit();
};
