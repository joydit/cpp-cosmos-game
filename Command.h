/*
   Parses string input containing a command identifier and parameter values.

   Indended for console and chat commands, additionally used for console variable value alteration event handling.
*/

#pragma once

#include <vector>
#include <Event.h>

class Command: public Event<void>
{
public:
   typedef uint8_t values_count_t;
   typedef std::vector<std::string> values_t;
   typedef void (*funct_t) (std::string, values_t);

   values_count_t valuesMin, valuesMax; //Handles value checking generically. Max values will largely be triggered due to erroneous string

   Command(name_t name, values_count_t valuesMin, values_count_t valuesMax);
   bool hasValueCount(int valuesCount);
   bool canCall(int valuesCount);
};
