#include "zot.h"
#include "SDL_timer.h"

using namespace std;
using namespace Zot;

ZimTime &ZimTime::update()
{
   m_time = SDL_GetTicks();
   return *this;
}

ostream &ZimTime::operator<<(ostream &os) const
{
   os << m_time;
   return os;
}
