#include "zot.h"
#include "zevent.h"

#ifdef __ZOT_USES_MSEVENTS__
#include <windows.h>
#endif

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

bool Zevent::lock(uint32 timeout)
{
   return false;
}

bool Zevent::unlock()
{
   return false;
}

void Zevent::signal()
{
}

bool Zevent::wait(uint32 timeout)
{
   //lock();
   //int32 ret = wait(timeout);
   //unlock();
   //return ret;
   return false;
}

#ifdef __ZOT_USES_MSEVENTS__
/////////////////////////////////////////////////////////////////////
// MSEvent

MSEvent::MSEvent()
   : m_hEvent(0)
   , m_hMutex(0)
{
   m_hEvent = (uint32)::CreateEvent(NULL, false, false, NULL);
   m_hMutex = (uint32)::CreateMutex(NULL, false, NULL);
}

MSEvent::~MSEvent()
{
   if (m_hEvent)
      ::CloseHandle((HANDLE)m_hEvent), m_hEvent = 0;
   if (m_hMutex)
      ::CloseHandle((HANDLE)m_hMutex), m_hMutex = 0;
}

bool MSEvent::lock(uint32 timeout)
{
   return WaitForSingleObject((HANDLE)m_hMutex, timeout) == WAIT_OBJECT_0;
}

bool MSEvent::unlock()
{
   return ReleaseMutex((HANDLE)m_hMutex) == TRUE;
}

void MSEvent::signal()
{
   ::SetEvent((HANDLE)m_hEvent);
}

bool MSEvent::wait(uint32 timeout)
{
   return WaitForSingleObject((HANDLE)m_hEvent, timeout) == WAIT_OBJECT_0;
}
#else
/////////////////////////////////////////////////////////////////////
// SDLEvent

SDLEvent::SDLEvent()
{
   m_pMutex = SDL_CreateMutex();
   m_pCond = SDL_CreateCond();
}

SDLEvent::~SDLEvent()
{
   if (m_pCond)
      SDL_DestroyCond(m_pCond), m_pCond = NULL;
   if (m_pMutex)
      SDL_DestroyMutex(m_pMutex), m_pMutex = NULL;
}

bool SDLEvent::lock(uint32 timeout)
{
   return SDL_LockMutex(m_pMutex) == 0;
}

bool SDLEvent::unlock()
{
   return SDL_UnlockMutex(m_pMutex) == 0;
}

void SDLEvent::signal()
{
   SDL_CondSignal(m_pCond);
}

bool SDLEvent::wait(uint32 timeout)
{
   // SDL_MUTEX_TIMEDOUT if timed out
   // 0 is signaled
   // -1 if error
   lock();
   bool ret = SDL_CondWaitTimeout(m_pCond, m_pMutex, timeout) == 0;
   unlock();
   return ret;
}
#endif
