#ifndef ZONSOLE_H
#define ZONSOLE_H

#include "iconsole.h"

#include <string>

#include <CEGUI.h>

#define Z Zonsole::get()

namespace Zot
{

class Zonsole : public IConsole
{
public:
   Zonsole();
   virtual ~Zonsole();

   static Zonsole *get() { return (Zonsole *)m_pConsole; }

   bool init();
   void draw();

   virtual void debug(const std::string &s);
   virtual void info(const std::string &s);
   virtual void warn(const std::string &s);
   virtual void error(const std::string &s);

   void show();
   void hide();
   bool isVisible();

   bool handleInput(const CEGUI::EventArgs &e);
   void handleTab();
   bool handleTextChanged(const CEGUI::EventArgs &e);

protected:
   CEGUI::FrameWindow *frameWnd;
   CEGUI::Editbox *inputWnd;
   CEGUI::Listbox *bufferWnd;
   CEGUI::Listbox *autoCompleteWnd;

   void addMessage(const CEGUI::colour &c, const CEGUI::String &s);

};

}

#endif