#include "zot.h"
#include "zevent.h"
#include "SDL.h"

//using namespace std;
using namespace Zot;

Zevent::Zevent()
{
}

Zevent::~Zevent()
{
}

bool Zevent::lock()
{
   return false;
}

bool Zevent::unlock()
{
   return false;
}

bool Zevent::signal()
{
   return false;
}

int32 Zevent::wait(uint32 timeout)
{
   //lock();
   //int32 ret = wait(timeout);
   //unlock();
   //return ret;
   return false;
}

SDLEvent::SDLEvent()
{
   m_pMutex = SDL_CreateMutex();
   m_pCond = SDL_CreateCond();
}

SDLEvent::~SDLEvent()
{
   if (m_pMutex)
      SDL_DestroyMutex(m_pMutex), m_pMutex = NULL;
   if (m_pCond)
      SDL_DestroyCond(m_pCond), m_pCond = NULL;
}

bool SDLEvent::lock()
{
   return SDL_LockMutex(m_pMutex) == 0;
}

bool SDLEvent::unlock()
{
   return SDL_UnlockMutex(m_pMutex) == 0;
}

bool SDLEvent::signal()
{
   lock();
   bool ret = SDL_CondSignal(m_pCond) == 0;
   unlock();
   return ret;
}

int32 SDLEvent::wait(uint32 timeout)
{
   // SDL_MUTEX_TIMEDOUT if timed out
   // 0 is signaled
   // -1 if error
   lock();
   int32 ret = SDL_CondWaitTimeout(m_pCond, m_pMutex, timeout);
   unlock();
   return ret;
}
