#include "zot.h"
#include "zthread.h"
#include "SDL.h"
#include "zystem.h"

//using namespace std;
using namespace Zot;

Zthread::Zthread()
   : m_pThread(NULL)
{
}

Zthread::~Zthread()
{
   // don't clean up thread here b/c it may be used down the line
   // by someone else...or self cleanup, etc
   m_pThread = NULL;
}

bool Zthread::create()
{
   m_pThread = SDL_CreateThread(sdlCallback, this);
   return m_pThread != NULL;
}

void Zthread::kill()
{
   if (m_pThread)
   {
      SDL_KillThread(m_pThread);
      m_pThread = NULL;
   }
}

int Zthread::sdlCallback(void *data)
{
   Zystem *sys = reinterpret_cast<Zystem *>(data);
   if (sys)
      sys->run();
   return 0;
}
