#include "ReusableTcpListener.h"

#ifdef SFML_SYSTEM_WINDOWS
  #include <winsock2.h>
#else
  #include <sys/socket.h>
#endif

void ReusableTcpListener::free()
{
  char reuse = 1;
  setsockopt(getHandle(), SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
}
