/*
   Handles the creation of commands with min/max parameter count and parsing of command lines.
*/

#pragma once

#include <map>
#include "Command.h"
#include "BaseEvents.h"

class Commands: public BaseEvents<Command>
{
public:
   typedef std::map<std::string, Command*> commands_t;

   Commands();
   Command& create(Command::name_t name, Command::funct_t funct, Command::values_count_t valuesMin = 0, Command::values_count_t valuesMax = 0);
   bool run(std::string line);

private:
   enum quote_e { QUOTE_NONE, QUOTE_SINGLE, QUOTE_DOUBLE };

   quote_e quote;
   bool space;

   void assign(std::string identifier, Command* command);
   bool canCall(std::string identifier, Command::values_t values);
   quote_e getQuoteType(char c);
   Command::values_t split(std::string line); //Like strtok, but considers double and single quote bodies
};
