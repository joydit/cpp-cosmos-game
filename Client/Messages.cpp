#include "Messages.h"
#include "Client.h"

using namespace std;

///Public
void Messages::unpackInitialise(sf::Packet& packet)
{
   receivable.unpackInitialise(packet);
   sendable.unpackInitialise(packet);
}

//Functions
void Messages::load()
{
   sendable.add("INITIAL", Client::INITIAL);
   sendable.add("INITIALISE", Client::INITIALISE);
   sendable.add("TEST");

   receivable.add("INITIAL_RESPOND", Server::INITIAL_RESPOND, ::Client::RecInitialRespond);
   receivable.add("INITIALISE", Server::INITIALISE, ::Client::RecInitialise);
   receivable.add("UDP_CHECK", Server::UDP_CHECK, ::Client::RecUdpCheck); ///Temporary - Add function
   receivable.add("DISCONNECT_MESSAGE", Server::DISCONNECT_DIALOGUE, ::Client::RecDialogue);
}
