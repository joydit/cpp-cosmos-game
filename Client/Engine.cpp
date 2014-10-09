#include "Engine.h"
#include "Config.h"

///Public
Engine::Engine(): BaseEngine(66)
{

}

Engine::~Engine()
{

}

void Engine::init()
{
   BaseEngine::init();
}

bool Engine::update()
{
   if (!BaseEngine::update())
      return false;

   if (!network.update())
      return false; //Call exit upon server disconnection

   return true;
}

void Engine::loadEvents()
{
   BaseEngine::loadEvents();
}

///Temporary
void Engine::loadCommands()
{
   BaseEngine::loadCommands();
}

///Private
void Engine::handleExit()
{

}
