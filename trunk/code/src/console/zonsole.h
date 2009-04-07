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

protected:
   CEGUI::Window *wnd;
};

}

#endif