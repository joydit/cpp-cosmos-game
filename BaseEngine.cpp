#include <signal.h>
#include <Console.h>
#include "BaseEngine.h"
#include "Shared/Engine.h"
#include "Global.h"

using namespace std;

Console BaseEngine::Console;
std::mutex BaseEngine::mtxGame;
volatile bool BaseEngine::Running = true;

///Public
BaseEngine* Instance = NULL;
BaseEngine& BaseEngine::Init()
{
   if (Instance == NULL)
   {
      Instance = new Engine();
      Instance->init();

      return *Instance;
   }
}

void BaseEngine::Free()
{
   if (Instance)
   {
      Instance->free();
      delete Instance;
      Instance = NULL;
   }
}

BaseEngine::BaseEngine(int tickRate): willExit(false)
{
   setTickRate(tickRate);

   Running = true;
   Console.startThread();
}

BaseEngine::~BaseEngine()
{

}

void BaseEngine::init()
{
   loadEvents();
   loadCommands();
   loadMessages();
}

void BaseEngine::free()
{
   Console.stopThread();
   Running = false;
}

bool BaseEngine::update()
{
   return true;
}

void BaseEngine::updateThread()
{
   while(Running)
   {
      mtxGame.lock();

      if (!update())
         exit();

      ///Temporary - Game update here

      mtxGame.unlock();

      updateSleep();
   }
}

void BaseEngine::updateSleep()
{
   sleep(tickRateWait);
}

void BaseEngine::loadEvents()
{

}

///Temporary
void comExit(string identifier, Command::values_t values)
{
   Engine::Get().exit();
}

void comTerminate(string identifier, Command::values_t values)
{
   raise(SIGTERM);
}

void BaseEngine::loadCommands()
{
   Console.commands.create("exit", comExit, 0, 0).addAlias("quit", "q");
   Console.commands.create("terminate", comTerminate); //Unsafe complete termination
}

void BaseEngine::loadMessages()
{
   network.messages.load();
}

void BaseEngine::setTickRate(int tickRate)
{
   tickRateWait = 1000 / tickRate;
}

int BaseEngine::getTickRate()
{
   return 1000 / tickRateWait;
}

void BaseEngine::exit()
{
   Running = false;
   fclose(stdin); //Stop accepting any more input which may halt the termination
}
