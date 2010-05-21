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

class GUI;

class SDLApp : public Zapp
{

public:

   SDLApp();

   virtual bool init();
   virtual void tick();
   virtual void run();
   virtual int onExit();

protected:

   virtual void draw();
   virtual void initGl();
   virtual void initGUI();
   virtual void postDraw();

   static const int width = 1024, height = 768;

   GUI *m_gui;
};

}
