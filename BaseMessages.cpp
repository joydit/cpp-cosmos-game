#include "BaseMessages.h"

using namespace std;

///Public
//Messages
sf::Packet BaseMessages::create(id_t msgId)
{
   sf::Packet packet;
   packet << msgId;

   return packet;
}

sf::Packet BaseMessages::create(name_t msgName)
{
   if (!sendable.exists(msgName))
      Console::PrintError("Message name doesn't exist '%s'", msgName.c_str());

   return create(sendable.get(msgName).getId());
}

//BaseMessageAttribs
BaseMessages::BaseMessageAttribs::BaseMessageAttribs(attribs_t attribs): id(0)
{
   setAttribs(attribs);
}

void BaseMessages::BaseMessageAttribs::setAttribs(attribs_t attribs)
{
   this->attribs = attribs;
}

bool BaseMessages::BaseMessageAttribs::hasAttrib(attrib_t attrib)
{
   return attribs.test(attrib);
}

BaseMessages::attribs_value_t BaseMessages::BaseMessageAttribs::getAttribs()
{
   return attribs.to_ulong();
}

void BaseMessages::BaseMessageAttribs::setId(id_t id)
{
   this->id = id;
}

BaseMessages::id_t BaseMessages::BaseMessageAttribs::getId()
{
   return this->id;
}

//MessageAttribs
BaseMessages::MessageAttribs::MessageAttribs(name_t eventName, attribs_t attribs): BaseMessageAttribs(attribs), eventName(eventName)
{

}

BaseMessages::MessageAttribs::name_t BaseMessages::MessageAttribs::getEventName()
{
   return eventName;
}

bool BaseMessages::MessageAttribs::hasEventName(name_t eventName)
{
   return eventName == this->eventName;
}

//MessageAttribsEvent
BaseMessages::MessageAttribsEvent::MessageAttribsEvent(name_t eventName, funct_t funct, attribs_t attribs): Event(eventName), BaseMessageAttribs(attribs)
{
   if (funct != NULL)
      add("initial", (void*)funct);
}

BaseMessages::MessageAttribsEvent::MessageAttribsEvent(name_t eventName, attribs_t attribs): MessageAttribsEvent(eventName, NULL, attribs)
{

}

//MessagesAttribsEvent
void BaseMessages::MessagesAttribsEvent::add(event_name_t eventName, id_t msgId, funct_t funct, attribs_t attribs)
{
   BaseMessagesAttribs::add(new message_attribs_t(eventName, funct, attribs), msgId);
}

void BaseMessages::MessagesAttribsEvent::add(event_name_t eventName, funct_t funct, attribs_t attribs)
{
   BaseMessagesAttribs::add(new message_attribs_t(eventName, funct, attribs)); //Place in pending until it receives an id
}

