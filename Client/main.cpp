#include <signal.h>
#include "Engine.h"

using namespace std;

void onExit();
void onSignal(int value);

int main()
{
   atexit(onExit);
   signal(SIGINT, onSignal);

   try
   {
      Engine::Init();
      Engine& engine = Engine::Get();

      if (engine.network.connect("", 0))
         engine.client.sendInitial("username", "password"); //Send login and any other details for handshake
      else
         engine.exit(); //Exit on failure

      engine.updateThread();
   }
   catch(std::exception& exception)
   {
      Console::PrintError(exception.what());
   }

   onExit();

   return EXIT_SUCCESS;
}

void onExit()
{
   Engine::Free();
}

void onSignal(int value)
{
   switch(value)
   {
   case 2:
      onExit();
      break;

   default:
      Console::PrintError("Unknown Signal '%i'", value);
   }
}
