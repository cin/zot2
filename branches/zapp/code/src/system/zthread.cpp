#include "zot.h"
#include "zthread.h"
#include "SDL.h"

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

///////////////////////////////////////////////////////////////
// SDLThread

SDLThread::SDLThread(ZthreadProc pfn, Zystem *pSys)
   : m_pThread(NULL)
{
   create(pfn, pSys);
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
