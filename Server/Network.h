#pragma once

#include <thread>
#include <Singleton.h>
#include "../BaseNetwork.h"
#include "Client.h"

class Network: public BaseNetwork, public Singleton<Network>
{
public:
   typedef sf::TcpListener tcp_listener_t;
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
   std::thread thread; //Combine
   volatile bool running;
   tcp_listener_t listener;
   selector_t selector; //Combine in own class with thread (ThreadedSelector)

   void updateThread(selector_t& selector);
   void updateSocketSelector(sf::SocketSelector& selector);

   void handleClientConnect();
   void handleClientDisconnect(Client& client);
   void handleClientUpdate();
   void handleClientReceive(Client& client);

   bool canSocketConnect(sf::TcpSocket& socket);
   bool canClientConnect(Client& client);
};
