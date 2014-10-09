#include <assert.h>
#include "Player.h"

///Public
Player::Player(Client* client, name_t name): BasePlayer(1, name)
{
   setClient(client);
}

bool Player::isOnline()
{
   return hasClient();
}

bool Player::hasClient()
{
   return client != NULL;
}

Client& Player::getClient()
{
   assert(hasClient());
   return *client;
}

///Private
void Player::setClient(Client* client)
{
   this->client = client;
}
