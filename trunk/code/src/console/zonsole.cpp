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

bool Zonsole::init()
{
   try
   {
      // this is CEGUI initialization. it does not belong in here.
      new System(new OpenGLRenderer(0));
      Logger::getSingleton().setLogFilename("../log/CEGUI.log");

      // set default directories for GUI elements
      DefaultResourceProvider *rp = static_cast<DefaultResourceProvider *>(System::getSingleton().getResourceProvider());
      rp->setResourceGroupDirectory("imagesets", "../res/imagesets/");
      rp->setResourceGroupDirectory("schemes", "../res/schemes/");

      // now link them to CEGUI
      Imageset::setDefaultResourceGroup("imagesets");
      Scheme::setDefaultResourceGroup("schemes");

      SchemeManager::getSingleton().loadScheme("zot.scheme");
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