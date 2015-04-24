#pragma once

#include <thread>
#include <Singleton.h>
#include "../BaseNetwork.h"
#include "ReusableTcpListener.h"
#include "Client.h"

class Clients;

class Network: public BaseNetwork, public Singleton<Network>
{
   friend class Clients;

public:
   typedef ReusableTcpListener tcp_listener_t;
   typedef sf::SocketSelector selector_t;
   typedef uint8_t socketselector_id_t; //Threaded socket selectors enabling player counts higher than the 1024 Linux limit

   static const int MAX_SELECTOR_PLAYERS;

   Network();
   ~Network();
   void startThread();
   void stopThread();
   bool listen(port_t portTcp = 0);
   int getOnlineCount();

private:
   std::thread threadTcp, threadUdp; //Combine
   volatile bool running;
   tcp_listener_t listener;
   selector_t selector; //Combine in own class with thread (ThreadedSelector)

   void updateThreadTcp(selector_t& selector);
   void updateThreadUdp();
   void updateSocketSelector(sf::SocketSelector& selector);

   void handleClientConnect();
   void handleClientUpdateTcp();
   void handleClientReceiveTcp(Client& client);

   bool canSocketConnect(sf::TcpSocket& socket);
   bool canClientConnect(Client& client);
};
