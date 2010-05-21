#include "zot.h"

#include <iostream>

#include "zonsole.h"
#include "gui.h"

using namespace std;
using namespace Zot;
using namespace CEGUI;

// create the proper console for the interface
IConsole *IConsole::create()
{
   m_pConsole = new Zonsole();
   return m_pConsole;
}

Zonsole::Zonsole()
{
}

Zonsole::~Zonsole()
{
}

void Zonsole::draw()
{
   // nothing to do here anymore .. all rendering is done in gui.cpp
}

bool Zonsole::handleInput(const EventArgs &e)
{
   // get string from input field
   String s = inputWnd->getText();
   if (s.length() > 0)
   {
      // add message to output buffer
      ListboxTextItem *i = new ListboxTextItem(s);
      Listbox *l = (Listbox *)bufferWnd;
      l->addItem(i);

      handleCmd(s.c_str());

      // make sure the scrollbar stays at the bottom
      Scrollbar *scroll = (Scrollbar *)bufferWnd->getChild("Zonsole/Buffer__auto_vscrollbar__");
      scroll->setScrollPosition(scroll->getDocumentSize());

      // clear the input field
      inputWnd->setText("\0");

      if (autoCompleteWnd->isVisible())
         autoCompleteWnd->hide();
   }
   return true;
}

bool Zonsole::handleKeyDown(const EventArgs &e)
{
   const KeyEventArgs &k = static_cast<const KeyEventArgs &>(e);

   switch (k.scancode)
   {
   case Key::ArrowUp:
      if (autoCompleteWnd->isVisible())
      {
         chooseAuto(false);
      }
      else
      {
         //display previous history entry
      }
      break;
   case Key::ArrowDown:
      if (autoCompleteWnd->isVisible())
      {
         chooseAuto(true);
      }
      else
      {
         //display next history entry
      }
      break;
   default:
      return false;
   }
   return true;
}

void Zonsole::chooseAuto(bool dir)
{
   int cnt = (int)autoCompleteWnd->getItemCount();
   int osel = getSelectedIndex(), nsel = -1;

   if (osel >= 0)
   {
      if (dir)
      {
         nsel = (osel + 1 == cnt) ? 0 : osel + 1;
      }
      else
      {
         nsel = (osel == 0) ? cnt - 1 : osel - 1;
      }
      autoCompleteWnd->getListboxItemFromIndex(osel)->setSelected(false);
      autoCompleteWnd->getListboxItemFromIndex(nsel)->setSelected(true);
#ifdef __CEGUI_0_6_2__
      autoCompleteWnd->requestRedraw();
#else
      autoCompleteWnd->invalidate();
#endif
   }
}

void Zonsole::handleTab()
{
   if (autoCompleteWnd->isVisible() && m_matches.size())
   {
      // find out which convar is selected (default to first one)
      size_t selected = getSelectedIndex();

      // be paranoid
      if (selected < m_matches.size())
      {
         string &selectedMatch = m_matches[selected];
         size_t len(selectedMatch.length());
         inputWnd->setText(selectedMatch);
         inputWnd->setCaratIndex(len);
      }
   }
}

bool Zonsole::handleTextChanged(const EventArgs &e)
{
   IConsole::handleTextChanged(inputWnd->getText().c_str());
   if (m_matches.size())
   {
      if (!autoCompleteWnd->isVisible())
         autoCompleteWnd->show();
      autoCompleteWnd->resetList();

      for (CmdMatchIter i = m_matches.begin(); i != m_matches.end(); i++)
      {
         ListboxTextItem *item = new ListboxTextItem(*i);
         item->setSelectionBrushImage("TaharezLook", "ListboxSelectionBrush");
         autoCompleteWnd->addItem(item);
      }
      // auto highlight the first item in the list, if it exists
      if (autoCompleteWnd->getItemCount() > 0)
      {
         autoCompleteWnd->getListboxItemFromIndex(0)->setSelected(true);
      }
   }
   else
   {
      if (autoCompleteWnd->isVisible())
         autoCompleteWnd->hide();
   }
   return true;
}

void Zonsole::hide()
{
   if (zonsoleWnd->isVisible())
   {
      zonsoleWnd->hide();
      MouseCursor::getSingleton().hide();
   }
}

bool Zonsole::handleACClick(const EventArgs &e)
{
   int sel = getSelectedIndex();

   if (sel >= 0)
   {
      String txt = autoCompleteWnd->getListboxItemFromIndex(sel)->getText();
      inputWnd->setText(txt);
      inputWnd->setCaratIndex(txt.length());
   }

   return true;
}

int Zonsole::getSelectedIndex()
{
   for (size_t i = 0; i < autoCompleteWnd->getItemCount(); i++)
   {
      if (autoCompleteWnd->getListboxItemFromIndex(i)->isSelected())
         return i;
   }
   return -1;
}

bool Zonsole::init()
{
   try
   {
      // get window objects
      zonsoleWnd = (FrameWindow *)WM.loadWindowLayout("zotconsole.layout");
      inputWnd = (Editbox *)WM.getWindow("Zonsole/Input");
      bufferWnd = (Listbox *)WM.getWindow("Zonsole/Buffer");
      autoCompleteWnd = (Listbox *)WM.getWindow("Zonsole/Input/AutoComplete");

      // frameWmnd
      zonsoleWnd->setProperty("Alpha", "0.7");

      // inputWnd
      inputWnd->subscribeEvent(Editbox::EventTextAccepted,
                                 Event::Subscriber(&Zonsole::handleInput, this));
      inputWnd->subscribeEvent(Window::EventTextChanged,
                                 Event::Subscriber(&Zonsole::handleTextChanged, this));
      inputWnd->subscribeEvent(Editbox::EventKeyDown,
                                 Event::Subscriber(&Zonsole::handleKeyDown, this));

      // autoCompleteWnd
      autoCompleteWnd->subscribeEvent(Listbox::EventMouseClick,
                                        Event::Subscriber(&Zonsole::handleACClick, this));
      autoCompleteWnd->hide();

      // add the console window to the GUI
      GUI::root()->addChildWindow(zonsoleWnd);

      // initially hide the zonsole
      hide();

      return true;
   }
   catch (const CEGUI::InvalidRequestException &e)
   {
      cerr << e.getMessage() << endl;
   }
   catch (const CEGUI::UnknownObjectException &e)
   {
      cerr << e.getMessage() << endl;
   }
   catch (const CEGUI::GenericException &e)
   {
      cerr << e.getMessage() << endl;
   }

   return false;
}

bool Zonsole::isVisible()
{
   return zonsoleWnd->isVisible();
}

void Zonsole::show()
{
   if (!zonsoleWnd->isVisible())
   {
      zonsoleWnd->show();
      inputWnd->activate();
      MouseCursor::getSingleton().show();
   }
}

void Zonsole::debug(const string &s)
{
   addMessage(colour(0xff1fe000), s);
}

void Zonsole::info(const string &s)
{
   addMessage(colour(0xffe6cc80), s);
}

void Zonsole::warn(const string &s)
{
   addMessage(colour(0xffff8000), s);
}

void Zonsole::error(const string &s)
{
   addMessage(colour(0xffe00000), s);
}

void Zonsole::addMessage(const colour &c, const String &s)
{
   ListboxTextItem *i = new ListboxTextItem(s);
   i->setTextColours(c);
   bufferWnd->addItem(i);
}
