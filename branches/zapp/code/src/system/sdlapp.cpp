#include "zot.h"

#include "sdlapp.h"
#include "zonsole.h"

#include <CEGUIDefaultResourceProvider.h>
#include <OpenGLGUIRenderer/openglrenderer.h>

using namespace std;
using namespace CEGUI;
using namespace Zot;

CEGUI::utf32 sKeyMap[SDLK_LAST];

void handleExit()
{
   // TODO: create CEGUI event and pass through the system so
   // all other interfaces/systems can be shutdown gracefully
   exit(0);
}

ConVar zotExit("exit", handleExit, "Exits the game.");
ConVar zotQuit("quit", handleExit, "Exits the game.");

void SDLApp::constructKeyMap()
{
   memset(sKeyMap, 0, sizeof(sKeyMap));

   sKeyMap[SDLK_RSHIFT] = CEGUI::Key::RightShift;
   sKeyMap[SDLK_LSHIFT] = CEGUI::Key::LeftShift;
   sKeyMap[SDLK_RCTRL] = CEGUI::Key::RightControl;
   sKeyMap[SDLK_LCTRL] = CEGUI::Key::LeftControl;
   sKeyMap[SDLK_RALT] = CEGUI::Key::RightAlt;
   sKeyMap[SDLK_LALT] = CEGUI::Key::LeftAlt;
   sKeyMap[SDLK_LSUPER] = CEGUI::Key::LeftWindows;
   sKeyMap[SDLK_RSUPER] = CEGUI::Key::RightWindows;
   sKeyMap[SDLK_UP] = CEGUI::Key::ArrowUp;
   sKeyMap[SDLK_DOWN] = CEGUI::Key::ArrowDown;
   sKeyMap[SDLK_RIGHT] = CEGUI::Key::ArrowRight;
   sKeyMap[SDLK_LEFT] = CEGUI::Key::ArrowLeft;
   sKeyMap[SDLK_INSERT] = CEGUI::Key::Insert;
   sKeyMap[SDLK_HOME] = CEGUI::Key::Home;
   sKeyMap[SDLK_END] = CEGUI::Key::End;
   sKeyMap[SDLK_PAGEUP] = CEGUI::Key::PageUp;
   sKeyMap[SDLK_PAGEDOWN] = CEGUI::Key::PageDown;
   sKeyMap[SDLK_DELETE] = CEGUI::Key::Delete;
   sKeyMap[SDLK_INSERT] = CEGUI::Key::Insert;
   sKeyMap[SDLK_TAB] = CEGUI::Key::Tab;
}

void SDLApp::draw()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   glTranslatef(0, 0, -20);
   glBegin(GL_POLYGON);
   glVertex3i(5, 5, 0);
   glVertex3i(-5, 5, 0);
   glVertex3i(-5, -5, 0);
   glVertex3i(5, -5, 0);
   glEnd();

   System::getSingleton().renderGUI();
   SDL_GL_SwapBuffers();
}

bool SDLApp::init()
{
   if (!Zapp::init())
      return false;

   const SDL_VideoInfo *video;

   if (SDL_Init(SDL_INIT_VIDEO) < 0)
   {
      fprintf(stderr, "SDL_Init() failed\n");
      return false;
   }
   atexit(handleExit);

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
   SDL_ShowCursor(0);
   SDL_EnableUNICODE(1);
   SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

   constructKeyMap();

   initGl();
   initCegui();
   if (!Zonsole::get()->init())
      return false;

   return true;
}

void SDLApp::initCegui()
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
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0f, (float)width / (float)height, 1.0, 1000.0);
   glMatrixMode(GL_MODELVIEW);

   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glEnable(GL_DEPTH_TEST);
}

void SDLApp::tick()
{
   SDL_Event e;
   CEGUI::System &S = CEGUI::System::getSingleton();

   while (SDL_PollEvent(&e))
   {
      switch (e.type)
      {
      case SDL_KEYDOWN:
         switch (e.key.keysym.sym)
         {
         case SDLK_ESCAPE:
            // TODO: FIX ME!!
            handleExit();
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
         handleExit();
         break;
      }
   }
   draw();
}