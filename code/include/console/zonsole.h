#include "iconsole.h"

#include <string>

#include <CEGUI.h>

using CEGUI::EventArgs;

namespace Zot
{

class Zonsole : public IConsole
{
public:
   Zonsole();
   virtual ~Zonsole();

   static Zonsole *get() { return (Zonsole *)m_pConsole; }

   virtual bool init();
   void draw();

   virtual void debug(const std::string &s);
   virtual void info(const std::string &s);
   virtual void warn(const std::string &s);
   virtual void error(const std::string &s);

   void show();
   void hide();
   bool isVisible();

   bool handleInput(const EventArgs &e);
   bool handleKeyDown(const EventArgs &e);
   void handleTab();
   bool handleTextChanged(const EventArgs &e);
   bool handleACClick(const EventArgs &e);

protected:
   CEGUI::FrameWindow *zonsoleWnd;
   CEGUI::Editbox *inputWnd;
   CEGUI::Listbox *bufferWnd;
   CEGUI::Listbox *autoCompleteWnd;

   void addMessage(const CEGUI::colour &c, const CEGUI::String &s);

   void chooseAuto(bool dir);

   int getSelectedIndex();
};

}
