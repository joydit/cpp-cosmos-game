#include "Console.h"
#include "../Error.h"

using namespace std;

///Public
Command::Command(name_t name, values_count_t valuesMin, values_count_t valuesMax): Event(name), valuesMin(valuesMin), valuesMax(valuesMax)
{

}

bool Command::hasValueCount(int valuesCount)
{
   if ((valuesMin != 0 && valuesCount < valuesMin) || (valuesMax != 0 && valuesCount > valuesMax))
   {
      Console::PrintError("Does not contain a valid number of values (Min: %i Max: %i Contains: %i)", valuesMin, valuesMax, valuesCount);
      return false;
   }

   return true;
}

bool Command::canCall(int valuesCount)
{
   if (!Event::canCall())
      return false;

   return hasValueCount(valuesCount);
}
