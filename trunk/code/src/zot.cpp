// zot.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <SDL.h>
#include <windows.h>

#include <stdio.h>

#include <gl/gl.h>
#include <gl/glu.h>

#include "zonsole.h"
#include "zogger.h"

using namespace Zot;

static int width = 800, height = 600;

CEGUI::utf32 sKeyMap[SDLK_LAST];

void handleExit()
{
   // TODO: create CEGUI event and pass through the system so
   // all other interfaces/systems can be shutdown gracefully
   exit(0);
}

ConVar zotExit("exit", handleExit, "Exits the game.");
ConVar zotQuit("quit", handleExit, "Exits the game.");

ConVar zotTestInt("testInt", "0");
ConVar zotTestBool("testBool", "1");
ConVar zotTestFloat("testFloat", "5.1");
ConVar zotTestString("testString", "this is a test string");

void constructKeyMap()
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
}

bool init_sdl()
{
   const SDL_VideoInfo *video;

   if (SDL_Init(SDL_INIT_VIDEO) < 0)
   {
      fprintf(stderr, "SDL_Init() failed\n");
      return false;
   }
   atexit(SDL_Quit);

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

   return true;
}

void init_gl()
{
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0f, (float)width / (float)height, 1.0, 1000.0);
   glMatrixMode(GL_MODELVIEW);

   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glEnable(GL_DEPTH_TEST);
}

void repaint()
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

   Z->draw();

   SDL_GL_SwapBuffers();
}

void main_loop()
{
   SDL_Event e;
   CEGUI::System &S = CEGUI::System::getSingleton();

   while (1)
   {
      while (SDL_PollEvent(&e))
      {
         switch (e.type)
         {
         case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
            case SDLK_ESCAPE:
               exit(0);
               break;
            case SDLK_BACKQUOTE:
               Z->isVisible() ? Z->hide() : Z->show();
               break;
            default:
               if (sKeyMap[e.key.keysym.sym] != 0)
                  S.injectKeyDown(sKeyMap[e.key.keysym.sym]);
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
            exit(0);
            break;
         }
      }
      repaint();
      SDL_Delay(50);
   }
}

int main(int argc, char *argv[])
{
   Zogger::create(Zogger::ZOG_DEBUG, Zogger::ZOG_CONSOLE, "");

   if (!init_sdl())
   {
      return -1;
   }
   
   init_gl();

   if (!Z->init())
      return -1;

   Z->debug("zonsole initialized");

   Z->debug("This is a debug message");
   Z->info("This is a info message");
   Z->warn("This is a warn message");
   Z->error("This is a error message");

   // this needs to be at the top of main()
   // see the notes in Zogger::zog for more info.
   Zogger::get()->zog("Zogging started");

   main_loop();

   return 0;
}
