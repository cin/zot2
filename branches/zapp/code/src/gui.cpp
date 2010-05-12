#include "zot.h"
#include "gui.h"

#include <CEGUISystem.h>
#include <CEGUIDefaultResourceProvider.h>
#ifdef __CEGUI_0_6_2__
#include <OpenGLGUIRenderer/openglrenderer.h>
#else
#include <OpenGL/CEGUIOpenGLRenderer.h>
#endif

#include <CEGUIImageset.h>
#include <CEGUIScheme.h>
#include <CEGUIFont.h>
#include <falagard/CEGUIFalWidgetLookManager.h>
#include <CEGUIWindowManager.h>

using namespace Zot;
using namespace CEGUI;
using namespace std;

Window *GUI::m_rootWin = NULL;

void GUI::init()
{
   try
   {
      // initialize CEGUI
#ifdef __CEGUI_0_6_2__
      new System(new OpenGLRenderer(0), NULL, NULL, NULL, "", "../log/zot.CEGUI.log");
#else
      System::create(OpenGLRenderer::create(), NULL, NULL, NULL, NULL, "", "../log/zot.CEGUI.log");
#endif

      // set default directories for GUI elements
      DefaultResourceProvider *rp = static_cast<DefaultResourceProvider *>(System::getSingleton().getResourceProvider());
      rp->setResourceGroupDirectory("imagesets", "../res/imagesets/");
      rp->setResourceGroupDirectory("schemes", "../res/schemes/");
      rp->setResourceGroupDirectory("fonts", "../res/fonts/");
      rp->setResourceGroupDirectory("looknfeel", "../res/looknfeel/");
      rp->setResourceGroupDirectory("layouts", "../res/layouts/");

      // now link them to CEGUI
      Imageset::setDefaultResourceGroup("imagesets");
      Scheme::setDefaultResourceGroup("schemes");
      Font::setDefaultResourceGroup("fonts");
      WidgetLookManager::setDefaultResourceGroup("looknfeel");
      WindowManager::setDefaultResourceGroup("layouts");      

      // load the scheme file
#ifdef __CEGUI_0_6_2__
      SchemeManager::getSingleton().loadScheme("TaharezLook.scheme");
#else
      SchemeManager::getSingleton().create("TaharezLook.scheme");
#endif

      // set a mouse cursor
      SYS.setDefaultMouseCursor("TaharezLook", "MouseArrow");

      // create a blank root window and set as the main GUI sheet
      // all other windows will be children
      m_rootWin = WM.createWindow("DefaultWindow", "Root");
      SYS.setGUISheet(m_rootWin);
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
}

void GUI::render()
{
   glPushAttrib(GL_POLYGON_BIT);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      SYS.renderGUI();
   glPopAttrib();
}

Window *GUI::root()
{
   return m_rootWin;
}
