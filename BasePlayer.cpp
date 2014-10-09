#include "BasePlayer.h"

using namespace std;

///Public
BasePlayer::BasePlayer(unique_id_t uniqueId, name_t name)
{
   setUniqueId(uniqueId);
   setName(name);
}

void BasePlayer::setName(name_t name)
{
   this->name = name;
}

BasePlayer::name_t BasePlayer::getName()
{
   return name;
}

BasePlayer::unique_id_t BasePlayer::getUniqueId()
{
   return uniqueId;
}

void BasePlayer::setUniqueId(unique_id_t uniqueId)
{
   this->uniqueId = uniqueId;
}

