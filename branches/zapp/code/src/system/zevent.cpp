#include "zot.h"
#include "zevent.h"

//using namespace std;
using namespace Zot;

/////////////////////////////////////////////////////////////////////
// Zevent

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

void Zevent::push()
{
}

void Zevent::pump()
{
}

bool Zevent::poll()
{
   return false;
}

bool Zevent::wait(uint32 timeout)
{
   //lock();
   //int32 ret = wait(timeout);
   //unlock();
   //return ret;
   return false;
}

/////////////////////////////////////////////////////////////////////
// SDLEvent

SDLEvent::SDLEvent()
{
   m_pMutex = SDL_CreateMutex();
   m_pCond = SDL_CreateCond();

   m_pEvent = new SDL_Event;
   m_pEvent->type = SDL_USEREVENT;
   m_pEvent->user.code = 0;
   m_pEvent->user.data1 = NULL;
   m_pEvent->user.data2 = NULL;
}

SDLEvent::~SDLEvent()
{
   if (m_pEvent)
      delete m_pEvent, m_pEvent = NULL;
   if (m_pCond)
      SDL_DestroyCond(m_pCond), m_pCond = NULL;
   if (m_pMutex)
      SDL_DestroyMutex(m_pMutex), m_pMutex = NULL;
}

bool SDLEvent::lock()
{
   return SDL_LockMutex(m_pMutex) == 0;
}

bool SDLEvent::unlock()
{
   return SDL_UnlockMutex(m_pMutex) == 0;
}

void SDLEvent::push()
{
   lock();
   while (SDL_PushEvent(m_pEvent) == -1)
      SDL_CondWait(m_pCond, m_pMutex);
   unlock();

   SDL_CondSignal(m_pCond);
}

void SDLEvent::pump()
{
   lock();
   SDL_PumpEvents();
   unlock();
}

bool SDLEvent::poll()
{
   lock();
   bool bSignaled = SDL_PollEvent(m_pEvent) == 1;
   unlock();

   if (bSignaled)
      SDL_CondSignal(m_pCond);

   return bSignaled;
}

bool SDLEvent::wait(uint32 timeout)
{
   // SDL_MUTEX_TIMEDOUT if timed out
   // 0 is signaled
   // -1 if error
   lock();
   bool bSignaled = SDL_PollEvent(m_pEvent) == 1;
   if (!bSignaled)
      bSignaled = SDL_CondWait(m_pCond, m_pMutex) == 0;
      //bSignaled = SDL_CondWaitTimeout(m_pCond, m_pMutex, timeout) == 0;
   unlock();

   if (bSignaled)
      SDL_CondSignal(m_pCond);

   return bSignaled;
}
