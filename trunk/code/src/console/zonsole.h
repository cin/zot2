#ifndef ZONSOLE_H
#define ZONSOLE_H

#include "iconsole.h"

#include <string>

#include <CEGUI.h>

#define Z Zonsole::get()

namespace Zot
{

class Zonsole
{
public:
   virtual ~Zonsole();

   static Zonsole *get() { if (!zonsole) zonsole = new Zonsole(); return zonsole; }

   bool init();
   void draw();

   void debug(const std::string &s);
   void info(const std::string &s);
   void warn(const std::string &s);
   void error(const std::string &s);
   void fatal(const std::string &s);

   void show();
   void hide();
   bool isVisible();

   bool handleInput(const CEGUI::EventArgs &e);

protected:
   CEGUI::FrameWindow *frameWnd;
   CEGUI::Editbox *inputWnd;
   CEGUI::Listbox *bufferWnd;

   void addMessage(const CEGUI::colour &c, const CEGUI::String &s);

   Zonsole();

   static Zonsole *zonsole;
};

}

#endif