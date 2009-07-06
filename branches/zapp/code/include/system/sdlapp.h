#pragma once

#include "SDL.h"

#include "zapp.h"

namespace Zot
{

class SDLApp : public Zapp
{
public:
   bool init();
   void run();
   int onExit();
};

}
