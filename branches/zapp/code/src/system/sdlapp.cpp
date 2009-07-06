#include "zot.h"

#include "sdlapp.h"

namespace Zot
{

bool SDLApp::init()
{
   if (!Zapp::init())
      return false;
   SDL_Init(0);
   return true;
}

void SDLApp::run()
{
   Zapp::run();
}

int SDLApp::onExit()
{
   SDL_Quit();
   return 1;
}

}
