#include <Engine.h>
#include "Network.h"
#include "Clients.h"
#include "Config.h"

using namespace std;

#ifdef WIN32
   #define MAX_SELECTOR_PLAYERS 64
#elseif
   #define MAX_SELECTOR_PLAYERS 1024
#endif

const int Network::MAX_SELECTOR_PLAYERS = MAX_SELECTOR_PLAYERS;

///Public
Network::Network(): running(false)
{

}

Network::~Network()
{
   selector.clear();
}

void Network::startThread()
{
   if (!running)
   {
      running = true;
      thread = std::thread(&Network::updateThread, this, ref(selector));
   }
}

void Network::stopThread()
{
   if (running)
   {
      running = false; //Will terminate thread when safe
      thread.join(); //Wait until this thread has terminated

      Console::PrintInfo("Network Thread Stopped Gracefully.");
   }
}

bool Network::listen(port_t portTcp)
{
   if (portTcp != 0)
      setPortTcp(portTcp);

   //Listen on port, if it's taken/error then try next
   bool first = true;
   while (listener.listen(getPortTcp()) != sf::Socket::Done)
   {
      if (!first)
      {
         Error::Print("Port '%u' may already be in use. Attempting with new port.", getPortTcp());
         first = false;
      }

      setPortTcp(getPortTcp() + 1);
   }

   Console::Print("Listening on port '%i'.", getPortTcp());
   selector.add(listener);

   return true;
}

int Network::getOnlineCount()
{
   return GetEngine().clients.getCount();
}

///Private
void Network::updateThread(selector_t& selector)
{
   sf::Time selectorWaitTime = sf::milliseconds(Config::Get().settings.get("network", "selectorWaitMs", 1000));

   //Make the selector wait for data on any socket
   while (running)
   {
      if (selector.wait(selectorWaitTime))
      {
         if (Engine::mtxGame.try_lock())
         {
            GetEngine().clients.cleanup();

            //Test the listener
            if (selector.isReady(listener))
            {
               handleClientConnect();
            }
            else
            {
               handleClientUpdate();
            }

            Engine::mtxGame.unlock();
         }
      }
   }
}

void Network::handleClientConnect()
{
   //The listener is ready: there is a pending connection
   sf::TcpSocket* ptr_socket = new sf::TcpSocket;
   if (listener.accept(*ptr_socket) == sf::Socket::Done)
   {
      sf::TcpSocket& socket = *ptr_socket;

      if (canSocketConnect(socket))
      {
         //Add the new client to the clients list
         Client* client = new Client(ptr_socket, "NoName");

         selector.add(socket); // Add the new client to the selector so that we will be notified when he sends something

         GetEngine().clients.handleConnect(client);
      }
   }
   else
   {
      //Error, we won't get a new connection, delete the socket
      delete ptr_socket;

      Console::PrintError("Socket error.");
   }
}

void Network::handleClientDisconnect(Client& client)
{
   GetEngine().clients.handleDisconnect(&client);
}

void Network::handleClientUpdate()
{
   //The listener socket is not ready, test all other sockets (the clients)
   for(Client*& client: GetEngine().clients.getAll())
   {
      //Check if disconnected here, if so, remove
      handleClientReceive(*client);
   }
}

void Network::handleClientReceive(Client& client)
{
   auto& socket = client.getSocket();
   if (selector.isReady(socket))
   {
      //The client has sent some data
      sf::Packet packet;
      switch(socket.receive(packet))
      {
      case sf::Socket::Done: //Message
         handleClientReceivePacket(client, packet);
         break;

      case sf::Socket::Disconnected:
         handleClientDisconnect(client); //Remove from list and free
         break;

      case sf::Socket::Error:
         Console::PrintError("Socket error.");
         break;

      case sf::Socket::NotReady:
        break;
      }
   }
}

bool Network::canSocketConnect(sf::TcpSocket& socket)
{
   return true;
}

bool Network::canClientConnect(Client& client)
{
   return true; //Accept connection (banned, too many connection attempts etc)
}
