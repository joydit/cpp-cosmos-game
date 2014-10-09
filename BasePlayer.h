/*
   Defines players as a passively associated entity, whether the client controlling that player's objects are connected or not.

   Teams will be highly coupled with the player class; Which may represent multiple player's.
*/

#pragma once

#include "BaseClient.h"

class Client;

class BasePlayer
{
public:
   typedef std::string name_t;
   typedef uint32_t unique_id_t;

   BasePlayer(unique_id_t uniqueId, name_t name);
   virtual void setName(name_t name);
   virtual name_t getName();
   unique_id_t getUniqueId();
   virtual void setUniqueId(unique_id_t accountId);
   virtual bool isOnline() = 0;

private:
   name_t name;
   unique_id_t uniqueId;
};
