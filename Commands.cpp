#include <set>
#include "Commands.h"
#include "Console.h"
#include "BaseEngine.h"

using namespace std;

///Public
Commands::Commands()
{

}

Command& Commands::create(Command::name_t name, Command::funct_t funct, Command::values_count_t valuesMin, Command::values_count_t valuesMax)
{
   Command& command = BaseEvents::create(name, 0, valuesMin, valuesMax);
   command.add("primary", (void*)funct);

   return command;
}

bool Commands::run(string line)
{
   Command::values_t values = split(line);
   string identifier = values.back();
   values.pop_back();

   all_t::iterator it = find(identifier);
   if (exists(it))
   {
      Command& command = **it;
      if (command.canCall(values.size()))
      {
         command.call(identifier, values);
         return true;
      }
      else
         Console::Print("Command is missing parameters: %s", identifier.c_str());
   }
   else
      Console::Print("Command not found: %s", identifier.c_str());

   return false;
}

Commands::quote_e Commands::getQuoteType(char c)
{
   return (c == '"')? QUOTE_DOUBLE: QUOTE_SINGLE;
}

Command::values_t Commands::split(string line)
{
   string value = "";
   string identifier;
   Command::values_t values;
   bool first = true;
   quote = QUOTE_NONE;

   for(auto c: line)
   {
      bool append = false;
      if (c != '\'' && c != '"')
      {
         if (quote == QUOTE_NONE)
         {
            if (c == ' ')
            {
               if (first)
               {
                  identifier = value;
                  first = false;
               }
               else
                  values.push_back(value);

               value = "";
            }
            else
               append = true;
         }
         else
            append = true;
      }
      else
      {
         if (quote != QUOTE_NONE)
         {
            if (quote == getQuoteType(c))
               quote = QUOTE_NONE;
            else
               append = true;
         }
         else
            quote = getQuoteType(c);
      }

      if (append)
         value.push_back(c);

      space = (c == ' ')? true: false; //Whether last character was a space
   }

   values.push_back(value); //Remaining value
   if (!first) //Check whether any parameters were used with the command identifier
      values.push_back(identifier); //Put command identifier at the end, for cleaner removal

   return values;
}
