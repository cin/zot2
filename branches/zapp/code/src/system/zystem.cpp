#include "zot.h"
#include "zystem.h"

#include <sdl.h>

#include <CEGUI.h>
#include <OpenGLGUIRenderer/openglrenderer.h>

using namespace Zot;
using namespace CEGUI;

Zystem::Zystem()
   : m_bRun(false)
   , m_delay(0)
{
}

Zystem::~Zystem()
{
}

bool Zystem::config(ZmCfg *pCfg)
{
   return false;
}

bool Zystem::init()
{
   m_bRun = true;
   m_delay = 17;
   return true;
}

int Zystem::onExit()
{
   return 0;
}

void Zystem::run()
{
   if (!m_bRun)
      init();

   while (m_bRun)
   {
      tick();
      SDL_Delay(m_delay);
   }
}

bool Zystem::saveConfig(Zot::ZmCfg *pCfg)
{
   return false;
}

void Zystem::tick()
{
   SDL_Event e;
   CEGUI::System &S = CEGUI::System::getSingleton();

   while (SDL_WaitEvent(&e))
   {
      switch (e.type)
      {
      case SDL_KEYDOWN:
         switch (e.key.keysym.sym)
         {
         case SDLK_BACKQUOTE:
            m_bRun = false;
            break;
         default:
            break;
         }
      case SDL_QUIT:
         break;
      default:
         break;
      }
   }
}
