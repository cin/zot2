#include "zot.h"

#include <iostream>

#include "zonsole.h"

#include <CEGUIDefaultResourceProvider.h>
#include <OpenGLGUIRenderer/openglrenderer.h>

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
   System::getSingleton().renderGUI();
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
      break;
   }
   return true;
}

void Zonsole::chooseAuto(bool dir)
{
   size_t cnt = autoCompleteWnd->getItemCount();
   int osel = -1, nsel = -1;

   for (size_t i = 0; i < cnt; i++)
   {
      if (autoCompleteWnd->getListboxItemFromIndex(i)->isSelected())
      {
         osel = i;
         break;
      }
   }

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
      autoCompleteWnd->requestRedraw();
   }
}

void Zonsole::handleTab()
{
   if (autoCompleteWnd->isVisible() && m_matches.size())
   {
      // find out which convar is selected (default to first one)
      size_t i = 0, selected = 0;
      for (; i < autoCompleteWnd->getItemCount(); i++)
      {
         if (autoCompleteWnd->getListboxItemFromIndex(i)->isSelected())
         {
            selected = i;
            break;
         }
      }

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
   if (frameWnd->isVisible())
   {
      frameWnd->hide();
      MouseCursor::getSingleton().hide();
   }
}

bool Zonsole::init()
{
   try
   {
      // initialize CEGUI
      new System(new OpenGLRenderer(0), NULL, NULL, NULL, "", "../log/zot.CEGUI.log");

      // set default directories for GUI elements
      DefaultResourceProvider *rp = static_cast<DefaultResourceProvider *>(System::getSingleton().getResourceProvider());
      rp->setResourceGroupDirectory("imagesets", "../res/imagesets/");
      rp->setResourceGroupDirectory("schemes", "../res/schemes/");
      rp->setResourceGroupDirectory("fonts", "../res/fonts/");
      rp->setResourceGroupDirectory("looknfeels", "../res/looknfeel/");
      rp->setResourceGroupDirectory("layouts", "../res/layouts/");

      // now link them to CEGUI
      Imageset::setDefaultResourceGroup("imagesets");
      Scheme::setDefaultResourceGroup("schemes");
      Font::setDefaultResourceGroup("fonts");
      WidgetLookManager::setDefaultResourceGroup("looknfeels");
      WindowManager::setDefaultResourceGroup("layouts");      

      // load the scheme file
      SchemeManager::getSingleton().loadScheme("TaharezLook.scheme");

      // set a mouse cursor
      System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");

      // the zonsole actually starts here.  everything above
      // this should be in another file

      // load the layout file
      frameWnd = (FrameWindow *)WindowManager::getSingleton().loadWindowLayout("zotconsole.layout");

      // set the console as the active window
      // NOTE: this should be done in a controlling class with a root window.
      // CEGUI's DefaultWindow is a prime candidate for this, as it takes up
      // the entire screen and is invisible.
      System::getSingleton().setGUISheet(frameWnd);

      // initially hide the zonsole
      hide();

      // get pointers to child windows
      inputWnd = (Editbox *)WindowManager::getSingleton().getWindow("Zonsole/Input");
      bufferWnd = (Listbox *)WindowManager::getSingleton().getWindow("Zonsole/Buffer");
      autoCompleteWnd = (Listbox *)WindowManager::getSingleton().getWindow("Zonsole/Input/AutoComplete");
      autoCompleteWnd->hide();

      // make it a little transparent
      frameWnd->setProperty("Alpha", "0.7");

      // hook in events
      inputWnd->subscribeEvent(Editbox::EventTextAccepted, Event::Subscriber(&Zonsole::handleInput, this));
      inputWnd->subscribeEvent(Window::EventTextChanged, Event::Subscriber(&Zonsole::handleTextChanged, this));
      inputWnd->subscribeEvent(Editbox::EventKeyDown, Event::Subscriber(&Zonsole::handleKeyDown, this));

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
   return frameWnd->isVisible();
}

void Zonsole::show()
{
   if (!frameWnd->isVisible())
   {
      frameWnd->show();
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