#include "zot.h"
#include "zthread.h"

#ifdef __ZOT_USE_MSTHREADS__
#include "zevent.h"
#include "windows.h"
#else
#include "SDL.h"
#endif

//using namespace std;
using namespace Zot;

///////////////////////////////////////////////////////////////
// Zthread

Zthread::Zthread(Zthread::ZthreadProc pfn, Zystem *pSys)
{
   create(pfn, pSys);
}

Zthread::~Zthread()
{
}

bool Zthread::create(Zthread::ZthreadProc pfn, Zystem *pSys)
{
   return false;
}

uint32 Zthread::getThreadId() const
{
   return -1;
}

#ifdef __ZOT_USE_MSTHREADS__
///////////////////////////////////////////////////////////////
// MSThread

MSThread::MSThread(ZthreadProc pfn, Zystem *pSys)
   : m_hThread(0)
   , m_threadId(0)
{
   create(pfn, pSys);
}

MSThread::~MSThread()
{
   if (m_hThread)
   {
      WaitForSingleObject((HANDLE)m_hThread, Zevent::ZOT_INDEFINITE);
      CloseHandle((HANDLE)m_hThread);
      m_hThread = 0;
   }
}

bool MSThread::create(ZthreadProc pfn, Zystem *pSys)
{
   m_hThread = (uint32)CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pfn, pSys, 0, (LPDWORD)&m_threadId);
   return m_hThread != 0;
}

uint32 MSThread::getThreadId() const
{
   // returns id of the current thread if m_pThread is NULL
   return m_threadId;
}
#else
///////////////////////////////////////////////////////////////
// SDLThread

SDLThread::SDLThread(ZthreadProc pfn, Zystem *pSys)
   : m_pThread(NULL)
{
   create(pfn, pSys);
}

SDLThread::~SDLThread()
{
   if (m_pThread)
   {
      SDL_WaitThread(m_pThread, NULL);
      m_pThread = NULL;
   }
}

bool SDLThread::create(ZthreadProc pfn, Zystem *pSys)
{
   return ((m_pThread = SDL_CreateThread(pfn, pSys)) ? true : false);
}

uint32 SDLThread::getThreadId() const
{
   // returns id of the current thread if m_pThread is NULL
   return SDL_GetThreadID(m_pThread);
}
#endif
