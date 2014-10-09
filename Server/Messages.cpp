#include "Messages.h"
#include "Client.h"

using namespace std;

void Messages::packInitialise(sf::Packet& packet)
{
   sendable.packInitialise(packet);
   receivable.packInitialise(packet);
}

//Functions
void Test(Client& client, sf::Packet packet)
{
   cout << "Called test " << endl;
}

void Messages::load()
{
   sendable.add("INITIAL_RESPOND", Server::INITIAL_RESPOND, 0);
   sendable.add("INITIALISE", Server::INITIALISE, 0);
   sendable.add("UDP_CHECK", Server::UDP_CHECK, 0);

   receivable.add("INITIAL", Client::INITIAL, ::Client::RecInitial, 4);
   receivable.add("INITIALISE", Client::INITIALISE, ::Client::RecInitialise, 2);
   receivable.add("TEST", Test, 4);

   sendable.flushPending();
   receivable.flushPending();
}
