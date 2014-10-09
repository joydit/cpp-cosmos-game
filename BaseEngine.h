/*
   Initialises and instantiates all engine components and the handling of the program logic.
*/

#pragma once

#include "Console.h"
#include "Global.h"
#include "Error.h"
#include "Shared/Config.h"
#include "Shared/Network.h"
#include "Shared/Client.h"
#include "Shared/Clients.h"
#include "Shared/Players.h"
#include "Shared/Events.h"

class BaseEngine
{
public:
   static ::Console Console; //Change this to a server specific Console based on ConsoleBase (mutex&, fuction)
   Config config;
   Network network;
   Players players;
   Events events;
   static std::mutex mtxGame; ///Temporary - Move to game class

   static BaseEngine& Init();
   static void Free();

   BaseEngine(int tickRate);
   ~BaseEngine();
   virtual void init();
   virtual void free(); //Allows the base class to destruct first
   virtual bool update();
   void updateThread();
   void updateSleep();
   virtual void loadEvents();
   virtual void loadCommands();
   virtual void loadMessages();
   virtual void setTickRate(int tickRate);
   virtual int getTickRate();
   virtual void exit();

protected:
   static volatile bool Running;

   bool willExit; //Async exit

private:
   int tickRateWait; //Actual rate
};
