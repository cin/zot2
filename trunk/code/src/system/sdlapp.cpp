#include "zot.h"
#include "iinput.h"
#include "sdlapp.h"
#include "zonsole.h"
#include "zogger.h"
#include "gui.h"
#include "zexture.h"
#include <SDL_opengl.h>
#include <SDL_image.h>

using namespace std;
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

SDLApp::SDLApp()
   : Zapp()
   , m_gui(NULL)
{
}

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

   initGl();
   initGUI();

   if (!IConsole::get()->init())
      return false;

   IInput::get()->init();

   // initialize the SDL_image library
   // this was taken straight outta their documentation
   int flags = IMG_INIT_JPG | IMG_INIT_PNG;
   int initted = IMG_Init(flags);
   if ((initted & flags) != flags)
   {
      fprintf(stderr, "IMG_Init: Failed to init required jpg and png support!\n");
      fprintf(stderr, "IMG_Init: %s\n", IMG_GetError()); // handle error
      return false;
   }

   if (!Zapp::init())
      return false;

   return true;
}

void SDLApp::initGUI()
{
   GUI::init();
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
   glEnable(GL_TEXTURE_2D);
}

void SDLApp::postDraw()
{
   GUI::render();
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
         pEvent = new Keyboard(
            EKeyDown,
            e.key.keysym.mod,
            e.key.keysym.sym,
            e.key.keysym.unicode);
         break;
      case SDL_KEYUP:
         pEvent = new Keyboard(
            EKeyUp,
            e.key.keysym.mod,
            e.key.keysym.sym,
            e.key.keysym.unicode);
         break;
      case SDL_MOUSEMOTION:
         pEvent = new MouseMotion(
            Pnt2ui(e.motion.x, e.motion.y),
            Pnt2i(e.motion.xrel, e.motion.yrel));
         break;
      case SDL_MOUSEBUTTONDOWN:
         pEvent = new MouseButton(
            EMouseButtonDown,
            e.button.button,
            Pnt2ui(e.button.x, e.button.y));
         break;
      case SDL_MOUSEBUTTONUP:
         pEvent = new MouseButton(
            EMouseButtonUp,
            e.button.button,
            Pnt2ui(e.button.x, e.button.y));
         break;
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
   IMG_Quit();
   Zapp::onExit();
   m_pApp = NULL;
   SDL_Quit();
   return 1;
}
