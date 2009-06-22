#pragma once

#include "SDL.h"

#include "zapp.h"

namespace CEGUI
{
class EventArgs;
class FrameWindow;
}

namespace Zot
{

class SDLApp : public Zapp
{

public:

   virtual bool init();
   virtual void tick();

protected:

   void constructKeyMap();
   bool handleKeyDown(const CEGUI::EventArgs &e);
   void draw();
   void initGl();
   void initCegui();

   static const int width = 1024, height = 768;

};

}
