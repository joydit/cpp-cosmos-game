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
   listener.free();
}

void Network::startThread()
{
   if (!running)
   {
      running = true;
      threadTcp = std::thread(&Network::updateThreadTcp, this, ref(selector));
      threadUdp = std::thread(&Network::updateThreadUdp, this);
   }
}

void Network::stopThread()
{
   if (running)
   {
      Clients::Get().disconnectAll("Server was shut down.");

      running = false;
      threadTcp.join();
      threadUdp.join();

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
   return Clients::Get().getCount();
}

///Private
void Network::updateThreadTcp(selector_t& selector)
{
   sf::Time receiveWaitSelectorMs = sf::milliseconds(Config::Get().settings.get("network", "receiveWaitSelectorMs", 1000));

   //Make the selector wait for data on any socket
   while (running)
   {
      if (selector.wait(receiveWaitSelectorMs))
      {
         if (Engine::mtxGame.try_lock())
         {
            //Test the listener
            if (selector.isReady(listener))
            {
               handleClientConnect();
            }
            else
            {
               handleClientUpdateTcp();
            }

            Clients::Get().handleCleanup();

            Engine::mtxGame.unlock();
         }
      }
   }
}

void Network::updateThreadUdp()
{
   sf::Time receiveBlockUdpMs = sf::milliseconds(Config::Get().settings.get("network", "receiveBlockUdpMs", 1000));
   char buffer[1024];
   std::size_t received = 0;
   sf::IpAddress sender;
   unsigned short port;
   sf::UdpSocket socket;

   socket.bind(getPortUdp());

   while (running)
   {
      sleep(receiveBlockUdpMs);

      if (socket.receive(buffer, sizeof(buffer), received, sender, port) == sf::Socket::Done) ///Temporary - Verify client auth here aswell
      {
         Engine::mtxGame.lock();
         //handleClientReceivePacket(client, packet);
         Engine::mtxGame.unlock();
      }
   }
}

void Network::handleClientConnect()
{
   //The listener is ready: there is a pending connection
   sf::TcpSocket* ptr_socket = new sf::TcpSocket;
   if (listener.accept(*ptr_socket) == sf::Socket::Done)
   {
      if (canSocketConnect(*ptr_socket))
      {
         //Add the new client to the clients list
         Client* client = new Client(ptr_socket);

         client->handleConnect();
      }
   }
   else
   {
      //Error, we won't get a new connection, delete the socket
      delete ptr_socket;

      Console::PrintError("Socket error.");
   }
}

void Network::handleClientUpdateTcp()
{
   //The listener socket is not ready, test all other sockets (the clients)
   for(Client*& client: Clients::Get().getAll())
   {
      //Check if disconnected here, if so, remove
      handleClientReceiveTcp(*client);
   }
}

void Network::handleClientReceiveTcp(Client& client)
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
         client.disconnect(); //Remove from list and free
         break;

      case sf::Socket::Error:
         Console::PrintError("Socket error.");
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
