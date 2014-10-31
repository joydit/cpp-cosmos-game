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
   Clients::clients_t clients = Clients::Get().getAll();
   int widthClientId = 8, widthUniqueId = 10, widthUsername = 32, widthPing = 8, widthIpAddress = 16;

   Console::Print("Clients Online: %i", clients.size());

   if (clients.size() > 0) //Only print table if a player is online
   {
      //Header
      cout << stringPadding("CID", widthClientId)
           << stringPadding("UID", widthUniqueId)
           << stringPadding("Username", widthUsername)
           << stringPadding("Ping", widthPing)
           << stringPadding("Address", widthIpAddress)
           << endl;

      //Body
      for(Client* client: clients)
      {
         cout << stringPadding(to_string(client->getId()), widthClientId)
              << stringPadding("0", widthUniqueId)
              << stringPadding(client->getUsername(), widthUsername)
              << stringPadding("0", widthPing)
              << stringPadding(client->getIpAddress(), widthIpAddress)
              << endl;
      }
   }
}

void comKick(string identifier, Command::values_t values)
{
   Client* client = Clients::Get().findByString(values[0], values[1]);
   if (client)
      client->kick(values[2]);
}

void Engine::loadCommands()
{
   BaseEngine::loadCommands();

   Console.commands.create("online", comOnline, 0, 0).addAlias("status");
   Console.commands.create("kick", comKick, 2, 3);
}
