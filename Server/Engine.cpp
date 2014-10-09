#include "Engine.h"
#include "Config.h"

using namespace std;

///Public
Engine::Engine(): BaseEngine(66)
{
   Events::event_t& event = events.create("CAN_CLIENT_CONNECT", Events::CAN_CLIENT_CONNECT);
   events.exists("CAN_CLIENT_CONNECT");
   events.get(Events::CAN_CLIENT_CONNECT);
}

Engine::~Engine()
{

}

void Engine::init()
{
   BaseEngine::init();

   if (!network.listen())
      throw Error("Failed to bind listen port.");

   network.startThread();
}

void Engine::free()
{
   network.stopThread();

   BaseEngine::free();
}

bool Engine::update()
{
   return true;
}

void Engine::loadEvents()
{
   BaseEngine::loadEvents();
}

///Temporary
void comOnline(string identifier, Command::values_t values)
{
   Console::Print("List clients");
}

void Engine::loadCommands()
{
   BaseEngine::loadCommands();

   Console.commands.create("online", comOnline, 0, 0);
}
