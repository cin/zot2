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
   virtual void run();
   virtual int onExit();

protected:

   virtual void draw();
   virtual void initGl();
   virtual void initCegui();
   virtual void postDraw();

   static const int width = 1024, height = 768;

};

}
