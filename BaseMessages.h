/*
   Maintains all message meta-data and distributes them to client's upon connecting.

   This enables the calling of events upon receiving a message as well as using UDP when appropriate for a particular message.
*/

#pragma once

#include <bitset>
#include <stdint.h>
#include <SFML/Network.hpp>
#include <Console.h>
#include "Event.h"
#include "BaseEvents.h"
#include "Error.h"

class Client;

class BaseMessages
{
public:
   typedef uint8_t attrib_t;
   typedef uint8_t attribs_value_t;
   typedef std::bitset<sizeof(attribs_value_t) * 8> attribs_t; //Ensure full use of the intended value's upper limit
   typedef void (*funct_t)(Client& client, sf::Packet packet);
   typedef uint16_t id_t;
   typedef Event<void> event_t;
   typedef typename event_t::name_t name_t;

   struct Server
   {
      enum msg_e { RESERVED, INITIAL_RESPOND, INITIALISE, UDP_CHECK, DISCONNECT_DIALOGUE };
   };

   struct Client
   {
      enum msg_e { RESERVED, INITIAL, INITIALISE, UDP_CHECK_RESPOND };
   };

   struct Attribs
   {
      enum attrib_e { RELIABLE = 1 << 1, DROP = 1 << 2 };
   };

   virtual void load() = 0;

protected:
   class BaseMessageAttribs
   {
   public:
      typedef uint8_t attrib_t;
      typedef uint8_t attribs_value_t;
      typedef std::bitset<sizeof(attribs_value_t) * 8> attribs_t; //Ensure full use of the intended value's upper limit

      BaseMessageAttribs(attribs_t attribs);
      void setAttribs(attribs_t attribs);
      bool hasAttrib(attrib_t attrib);
      attribs_value_t getAttribs();
      void setId(id_t id);
      id_t getId();

   private:
      attribs_t attribs;
      id_t id;
   };

   class MessageAttribs: public BaseMessageAttribs
   {
   public:
      typedef BaseMessages::name_t name_t;

      MessageAttribs(name_t eventName, attribs_t attribs);
      name_t getEventName();
      bool hasEventName(name_t eventName);

   private:
      name_t eventName;
   };

   class MessageAttribsEvent: public BaseMessageAttribs, public event_t
   {
   public:
      MessageAttribsEvent(name_t eventName, funct_t funct, attribs_t attribs = 0);
      MessageAttribsEvent(name_t eventName, attribs_t attribs = 0);
   };

   template <typename T>
      class BaseMessagesAttribs: public BaseEvents<T>
   {
   public:
      typedef T message_attribs_t;
      typedef typename message_attribs_t::name_t event_name_t;
      typedef typename message_attribs_t::attribs_t attribs_t;
      typedef std::list<message_attribs_t*> pending_t;

      ~BaseMessagesAttribs();
      void add(message_attribs_t* messageAttribs, id_t msgId = 0);
      void add(event_name_t eventName, id_t msgId, attribs_t attribs = 0);
      void add(event_name_t eventName, attribs_t attribs = 0); //Used for non-engine message handling
      void setAttribs(id_t msgId, attribs_t attribs = 0);
      void movePending(event_name_t eventName, id_t msgId);
      void flushPending();

      void packPending(sf::Packet& packet);
      void unpackPending(sf::Packet& packet);
      void packAttributes(sf::Packet& packet);
      void unpackAttributes(sf::Packet& packet);
      void packInitialise(sf::Packet& packet);
      void unpackInitialise(sf::Packet& packet);

   private:
      pending_t pending;
      sf::Packet pendingPacket; //Contains a snapshot of all unid'd packets
   };

   class MessagesAttribs: public BaseMessagesAttribs<MessageAttribs>
   {
   public:
   };

   class MessagesAttribsEvent: public BaseMessagesAttribs<MessageAttribsEvent>
   {
   public:
      void add(event_name_t eventName, id_t msgId, funct_t funct, attribs_t attribs = 0);
      void add(event_name_t eventName, funct_t funct, attribs_t attribs = 0);
   };

public:
   MessagesAttribs sendable;
   MessagesAttribsEvent receivable; //We want to call an event upon receiving an associated message

   virtual sf::Packet create(id_t msgId);
   virtual sf::Packet create(name_t msgName);
};

///Public
template <typename T>
   BaseMessages::BaseMessagesAttribs<T>::~BaseMessagesAttribs()
{

}

template <typename T>
   void BaseMessages::BaseMessagesAttribs<T>::add(message_attribs_t* messageAttribs, id_t msgId)
{
   if (msgId == 0)
      pending.push_back(messageAttribs); //All messages without an id are pending
   else
   {
      this->place(messageAttribs, msgId);
      messageAttribs->setId(msgId);
   }
}

template <typename T>
   void BaseMessages::BaseMessagesAttribs<T>::add(event_name_t eventName, id_t msgId, attribs_t attribs)
{
   add(new message_attribs_t(eventName, attribs), msgId);
}

template <typename T>
   void BaseMessages::BaseMessagesAttribs<T>::add(event_name_t eventName, attribs_t attribs)
{
   add(new message_attribs_t(eventName, attribs));
}

template <typename T>
   void BaseMessages::BaseMessagesAttribs<T>::setAttribs(id_t msgId, attribs_t attribs)
{
   if (this->exists(msgId))
   {
      message_attribs_t& messageAttribs = this->get(msgId);
      if (attribs.to_ulong() != 0)
         messageAttribs.setAttribs(attribs);
   }
   else
      Console::PrintError("No message id instantiated for '%i'", msgId);
}

template <typename T>
   void BaseMessages::BaseMessagesAttribs<T>::movePending(event_name_t eventName, id_t msgId)
{
   typename pending_t::iterator it = this->find(pending, eventName); //Check for any pending
   if (it != pending.end())
   {
      add(*it, msgId);
      pending.erase(it);
   }
   else
      Console::PrintError("No pending message id exists for '%i'", msgId);
}

template <typename T>
   void BaseMessages::BaseMessagesAttribs<T>::flushPending()
{
   pendingPacket.clear();

   pendingPacket << (id_t)pending.size();

   for (message_attribs_t* messageAttribs: pending)
   {
      pendingPacket << (id_t)this->getAll().place_back(messageAttribs);
      pendingPacket << messageAttribs->getEventName();
   }

   pending.clear();
}

template <typename T>
   void BaseMessages::BaseMessagesAttribs<T>::packPending(sf::Packet& packet)
{
   packet.append(pendingPacket.getData(), pendingPacket.getDataSize());
}

template <typename T>
   void BaseMessages::BaseMessagesAttribs<T>::unpackPending(sf::Packet& packet)
{
   id_t msgIdMax;
   id_t msgId;
   typename message_attribs_t::name_t msgName;

   packet >> msgIdMax;

   for(id_t msgId = 0; msgId < msgIdMax; msgId++)
   {
      packet >> msgId;
      packet >> msgName;

      movePending(msgName, msgId); //Move from pending and associate with id
   }

   if (pending.size() > 0)
      Console::PrintError("Redundant messages still pending in client");
}

template <typename T>
   void BaseMessages::BaseMessagesAttribs<T>::packAttributes(sf::Packet& packet)
{
   bool first = true;
   for (message_attribs_t* messageAttribs: this->getAll())
   {
      if (first) //Tell it how many potential messages there are
      {
         packet << (id_t)this->getAll().size();
         first = false;
      }
      else if (messageAttribs)
      {
         packet << (attribs_value_t)messageAttribs->getAttribs();
      }
      else //No message data, there will likely never be message id gaps
      {
         packet << (attribs_value_t)0;
      }
   }
}

template <typename T>
   void BaseMessages::BaseMessagesAttribs<T>::unpackAttributes(sf::Packet& packet)
{
   id_t msgIdMax;
   attribs_value_t attribs;

   packet >> msgIdMax;
   for(id_t msgId = 1; msgId < msgIdMax; msgId++)
   {
      packet >> attribs;
      setAttribs(msgId, attribs);
   }
}

template <typename T>
   void BaseMessages::BaseMessagesAttribs<T>::packInitialise(sf::Packet& packet)
{
   packPending(packet);
   packAttributes(packet);
}

template <typename T>
   void BaseMessages::BaseMessagesAttribs<T>::unpackInitialise(sf::Packet& packet)
{
   unpackPending(packet);
   unpackAttributes(packet);
}
