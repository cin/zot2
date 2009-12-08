#include "zot.h"
#include "iinput.h"
#include "sdlapp.h"
#include "zonsole.h"
#include "zogger.h"

#include <CEGUIDefaultResourceProvider.h>
#ifdef __CEGUI_0_6_2__
#include <OpenGLGUIRenderer/openglrenderer.h>
#else
#include <OpenGL/CEGUIOpenGLRenderer.h>
#endif

using namespace std;
using namespace CEGUI;
using namespace Zot;

void handleZystress()
{
   Zapp::get()->addZystress();
}

void handleExit()
{
   Zapp::get()->onExit();
}

void handleLog()
{
   Zogger::get()->zog("this is a test of the emergency broadcast system\n");
}

ConVar foo1("foo1", NULL, "foo1");
ConVar foo2("foo2", NULL, "foo2");
ConVar foo3("foo3", NULL, "foo3");
ConVar zotExit("exit", handleExit, "Exits the game.");
ConVar zotQuit("quit", handleExit, "Exits the game.");
ConVar zotLog("log", handleLog, "Logs a message to the logfile.");
ConVar zotZystress("addStress", handleZystress, "Adds another Zystress thread for testing.");

void SDLApp::draw()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   m_camera.place();
   m_terrain.draw();
}

bool SDLApp::init()
{
   const SDL_VideoInfo *video;

   m_pApp = this;

   if (SDL_Init(SDL_INIT_VIDEO) < 0)
   {
      fprintf(stderr, "SDL_Init() failed\n");
      return false;
   }

   video = SDL_GetVideoInfo();
   if (!video)
   {
      fprintf(stderr, "SDL_GetVideoInfo() failed\n");
      return false;
   }

   SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
   SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
   SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
   if (SDL_SetVideoMode(width, height, video->vfmt->BitsPerPixel, SDL_OPENGL) == 0)
   {
      fprintf(stderr, "SDL_SetVideoMode() failed\n");
      return false;
   }

   IInput::get()->init();

   initGl();
   initCegui();
   if (!Zonsole::get()->init())
      return false;

   if (!Zapp::init())
      return false;

   return true;
}

void SDLApp::initCegui()
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
      rp->setResourceGroupDirectory("looknfeels", "../res/looknfeel/");
      rp->setResourceGroupDirectory("layouts", "../res/layouts/");

      // now link them to CEGUI
      Imageset::setDefaultResourceGroup("imagesets");
      Scheme::setDefaultResourceGroup("schemes");
      Font::setDefaultResourceGroup("fonts");
      WidgetLookManager::setDefaultResourceGroup("looknfeels");
      WindowManager::setDefaultResourceGroup("layouts");      

      // load the scheme file
#ifdef __CEGUI_0_6_2__
      SchemeManager::getSingleton().loadScheme("TaharezLook.scheme");
#else
      SchemeManager::getSingleton().create("TaharezLook.scheme");
#endif

      // set a mouse cursor
      System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
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

void SDLApp::initGl()
{
   //glViewport(0, 0, width, height);
   //glMatrixMode(GL_PROJECTION);
   //glLoadIdentity();
   //gluPerspective(45.0f, (float)width / (float)height, 1.0, 1000.0);
   //glMatrixMode(GL_MODELVIEW);

   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glEnable(GL_DEPTH_TEST);
}

void SDLApp::postDraw()
{
   System::getSingleton().renderGUI();
   SDL_GL_SwapBuffers();
}

void SDLApp::tick()
{
   SDL_Event e;
   CEGUI::System &S = CEGUI::System::getSingleton();

   while (SDL_PollEvent(&e))
   {
#if 1
      InputEvent *pEvent = NULL;
      switch (e.type)
      {
      case SDL_KEYDOWN:
      {
         Keyboard *pKb = new Keyboard;
         pKb->m_type = EKeyDown;
         pKb->m_state = e.key.state;
         pKb->m_mod = e.key.keysym.mod;
         pKb->m_key = e.key.keysym.sym;
         pKb->m_unicode = e.key.keysym.unicode;
         pEvent = pKb;
         break;
      }
      case SDL_KEYUP:
      {
         Keyboard *pKb = new Keyboard;
         pKb->m_type = EKeyUp;
         pKb->m_state = e.key.state;
         pKb->m_mod = e.key.keysym.mod;
         pKb->m_key = e.key.keysym.sym;
         pKb->m_unicode = e.key.keysym.unicode;
         pEvent = pKb;
         break;
      }
      case SDL_MOUSEMOTION:
      {
         MouseMotion *pMm = new MouseMotion;
         pMm->m_type = EMouseMotion;
         pMm->m_state = e.motion.state;
         pMm->m_abs.x = e.motion.x;
         pMm->m_abs.y = e.motion.y;
         pMm->m_rel.x = e.motion.xrel;
         pMm->m_rel.y = e.motion.yrel;
         pEvent = pMm;
         break;
      }
      case SDL_MOUSEBUTTONDOWN:
      {
         MouseButton *pMb = new MouseButton;
         pMb->m_type = EMouseButtonDown;
         pMb->m_state = e.button.state;
         pMb->m_button = e.button.button;
         pMb->m_abs.x = e.button.x;
         pMb->m_abs.y = e.button.y;
         pEvent = pMb;
         break;
      }
      case SDL_MOUSEBUTTONUP:
      {
         MouseButton *pMb = new MouseButton;
         pMb->m_type = EMouseButtonUp;
         pMb->m_state = e.button.state;
         pMb->m_button = e.button.button;
         pMb->m_abs.x = e.button.x;
         pMb->m_abs.y = e.button.y;
         pEvent = pMb;
         break;
      }
      case SDL_QUIT:
         onExit();
         break;
      default:
         break;
      }

      if (pEvent)
      {
         IInput::get()->tick(pEvent);
         delete pEvent;
      }
#else
      switch (e.type)
      {
      case SDL_KEYDOWN:
         switch (e.key.keysym.sym)
         {
         case SDLK_ESCAPE:
            onExit();
            break;
         case SDLK_BACKQUOTE:
            Zonsole::get()->isVisible() ? Zonsole::get()->hide() : Zonsole::get()->show();
            break;
         default:
            if (sKeyMap[e.key.keysym.sym] != 0)
            {
               if (Zonsole::get()->isVisible() && e.key.keysym.sym == SDLK_TAB)
               {
                  Zonsole::get()->handleTab();
               }
               else
                  S.injectKeyDown(sKeyMap[e.key.keysym.sym]);
            }
            else
               S.injectKeyDown(e.key.keysym.scancode);

            if (e.key.keysym.unicode != 0 && e.key.keysym.sym != SDLK_RETURN)
               S.injectChar(static_cast<CEGUI::utf32>(e.key.keysym.unicode));
            break;
         }
         break;
      case SDL_KEYUP:
         S.injectKeyUp(e.key.keysym.scancode);
         break;
      case SDL_MOUSEMOTION:
         S.injectMousePosition((float)e.motion.x, (float)e.motion.y);
         break;
      case SDL_MOUSEBUTTONDOWN:
         switch (e.button.button)
         {
         case SDL_BUTTON_LEFT:
            S.injectMouseButtonDown(CEGUI::LeftButton);
            break;
         case SDL_BUTTON_MIDDLE:
            S.injectMouseButtonDown(CEGUI::MiddleButton);
            break;
         case SDL_BUTTON_RIGHT:
            S.injectMouseButtonDown(CEGUI::RightButton);
            break;
         }
         break;
      case SDL_MOUSEBUTTONUP:
         switch (e.button.button)
         {
         case SDL_BUTTON_LEFT:
            S.injectMouseButtonUp(CEGUI::LeftButton);
            break;
         case SDL_BUTTON_MIDDLE:
            S.injectMouseButtonUp(CEGUI::MiddleButton);
            break;
         case SDL_BUTTON_RIGHT:
            S.injectMouseButtonUp(CEGUI::RightButton);
            break;
         }
         break;
      case SDL_QUIT:
         onExit();
         break;
      default:
         break;
      }
#endif
   }

   if (m_bRunning)
   {
      Zapp::tick();
      draw();
      postDraw();
   }
}

void SDLApp::run()
{
   init();
   while (m_bRunning)
      tick();
}

int SDLApp::onExit()
{
   m_bRunning = false;
   Zapp::onExit();
   m_pApp = NULL;
   SDL_Quit();
   return 1;
}
