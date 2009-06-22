#include "sdlapp.h"

using namespace Zot;

class Tesystem : public Zystem
{
public:
   Tesystem()
   {
   }
};

int main(int argc, char *argv[])
{
   SDLApp sdlapp;
   if (!sdlapp.init())
   {
      return -1;
   }
   Tesystem sys;
   sys.create();

   sdlapp.run();
   return 0;
}
