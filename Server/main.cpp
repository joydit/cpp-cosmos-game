#include <MINIZ/miniz.c>
#include <unordered_map>
#include <deque>
#include <signal.h>
#include <typeinfo>
#include "Engine.h"

using namespace std;

bool compress();
void onExit();
void onSignal(int value);

int main()
{
   atexit(onExit);
   signal(SIGINT, onSignal);

   try
   {
      Engine::Init();
      Engine& engine = Engine::Get();

      engine.updateThread();
   }
   catch(std::exception& exception)
   {
      Console::PrintError(exception.what());
   }

   return EXIT_SUCCESS;
}

void onExit()
{
   Engine::Free();
}

void onSignal(int value)
{
   switch(value)
   {
   case 2:
      onExit();
      break;

   default:
      Console::PrintError("Unknown Signal '%i'", value);
   }
}

///Temporary - Experimental use of miniz
bool compress()
{
   const char *s_pStr = "Good morning sd75a6sd8d8s 64ad5s64 d6d sasd sad qewfg tr hhytrh rgefwe fs ewfef4wre45rf reg 435gt4rweff sdfew fef dsfdfdf";

   //uint32_t step = 0;
   int cmp_status;
   uLong src_len = (uLong)strlen(s_pStr);
   uLong cmp_len = compressBound(src_len);
   //uLong uncomp_len = src_len;
   uint8_t *pCmp, *pUncomp;
   //uint32_t total_succeeded = 0;

   //Allocate buffers to hold compressed and uncompressed data.
   pCmp = (mz_uint8 *)malloc((size_t)cmp_len);
   pUncomp = (mz_uint8 *)malloc((size_t)src_len);
   if ((!pCmp) || (!pUncomp))
   {
      printf("Out of memory!\n");
      return false;
   }

   // Compress the string.
   cmp_status = compress2(pCmp, &cmp_len, (const unsigned char *)s_pStr, src_len, 10);
   if (cmp_status != Z_OK)
   {
      printf("compress() failed!\n");
      free(pCmp);
      free(pUncomp);
      return false;
   }

   printf("Compressed from %u to %u bytes, at rate %g%%\n", (mz_uint32)src_len, (mz_uint32)cmp_len, (1.f - (float)cmp_len / src_len) * 100.f);

   return true;
}
