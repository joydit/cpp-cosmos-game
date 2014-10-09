/*
   Standalone reusable functionality for a generic purpose.

   Mostly containing shortened statements or trivial functionality missing from the STL.
*/

#pragma once

#include <chrono>
#include <thread>

inline void sleep(int ms)
{
   std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
