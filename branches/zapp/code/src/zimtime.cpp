#include "zot.h"
#include "SDL_timer.h"

using namespace Zot;

ZimTime &ZimTime::update()
{
   m_time = SDL_GetTicks();
   return *this;
}
