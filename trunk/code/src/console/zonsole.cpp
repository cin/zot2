#include "stdafx.h"

#include <iostream>

#include "zonsole.h"

#include <CEGUIDefaultResourceProvider.h>
#include <OpenGLGUIRenderer/openglrenderer.h>

using namespace Zot;
using namespace CEGUI;

Zonsole::Zonsole()
{
}

Zonsole::~Zonsole()
{
}

IConsole *IConsole::create()
{
   m_spConsole = new Zonsole();
   return m_spConsole;
}

void Zonsole::draw()
{
   System::getSingleton().renderGUI();
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

      // the zonsole actually starts here.  everything above
      // this should be in another file
      SchemeManager::getSingleton().loadScheme("zot.scheme");
      wnd = WindowManager::getSingleton().loadWindowLayout("zotconsole.layout");
      System::getSingleton().setGUISheet(wnd);
      return true;
   }
   catch (CEGUI::InvalidRequestException e)
   {
      std::cerr << e.getMessage() << std::endl;
   }
   catch (CEGUI::UnknownObjectException e)
   {
      std::cerr << e.getMessage() << std::endl;
   }
   catch (CEGUI::GenericException e)
   {
      std::cerr << e.getMessage() << std::endl;
   }

   return false;
}