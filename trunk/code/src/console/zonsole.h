#ifndef ZONSOLE_H
#define ZONSOLE_H

#include "iconsole.h"

#include <CEGUI.h>

namespace Zot
{

class Zonsole : public IConsole
{
public:
   Zonsole();
   virtual ~Zonsole();

   bool init();
   void draw();

   void show();
   void hide();
   bool isVisible();

   bool handleInput(const CEGUI::EventArgs &e);

protected:
   CEGUI::Window *wnd;
   CEGUI::Window *input;
   CEGUI::Window *buffer;
};

}

#endif