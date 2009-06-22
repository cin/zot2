#include "sdlapp.h"

using namespace Zot;

int main(int argc, char *argv[])
{
   SDLApp sdlapp;
   if (!sdlapp.init())
   {
      return -1;
   }
   sdlapp.run();
   return 0;
}
