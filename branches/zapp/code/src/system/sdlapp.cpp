#include "zot.h"

#include "sdlapp.h"

namespace Zot
{

bool SDLApp::init()
{
   SDL_Init(0);
   return true;
}

void SDLApp::run()
{
}

}
