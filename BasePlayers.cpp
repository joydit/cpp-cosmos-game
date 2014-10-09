#include <algorithm>
#include "BasePlayers.h"
#include "Shared/Player.h"

using namespace std;

///Public
BasePlayers::BasePlayers()
{

}

Player* BasePlayers::findByUniqueId(BasePlayer::unique_id_t uniqueId, filter_e filter)
{
   auto funct = [uniqueId](Player*& player) {
      return player->getUniqueId() == uniqueId;
   };

   return find(funct, uniqueId, filter);
}

Player* BasePlayers::findByName(BasePlayer::name_t name, filter_e filter)
{
   auto funct = [name](Player*& player) {
      return player->getName() == name;
   };

   return find(funct, name, filter);
}

///Protected
template <typename T>
   Player* BasePlayers::find(std::function<bool(Player*&)> funct, T compare, filter_e filter)
{
   if (filter == FILTER_ONLINE || filter == FILTER_BOTH)
      find_if(online.begin(), online.end(), funct);

   if (filter == FILTER_OFFLINE || filter == FILTER_BOTH)
      find_if(offline.begin(), offline.end(), funct);

   return NULL;
}
