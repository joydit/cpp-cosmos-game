/*
   Maintains a list of all the players and provides find facilities.
*/

#pragma once

#include <list>
#include "BasePlayer.h"

class Player;

class BasePlayers
{
public:
   typedef std::list<Player*> players_t;
   enum filter_e { FILTER_BOTH, FILTER_ONLINE, FILTER_OFFLINE };

   BasePlayers();
   Player* findByUniqueId(BasePlayer::unique_id_t uniqueId, filter_e filter);
   Player* findByName(BasePlayer::name_t name, filter_e filter);

protected:
   players_t online, offline;

   template <typename T> Player* find(std::function<bool(Player*&)> funct, T compare, filter_e filter);
};
