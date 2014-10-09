#ifdef SERVER
   #include "../Server/Engine.h"
#elif CLIENT
   #include "../Client/Engine.h"
#endif

#pragma once

inline Engine& GetEngine()
{
   return Engine::Get();
}
