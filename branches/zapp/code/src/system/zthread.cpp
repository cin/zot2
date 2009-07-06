#include "zot.h"
#include "zthread.h"
#include "SDL.h"

//using namespace std;
using namespace Zot;

///////////////////////////////////////////////////////////////
// Zthread

Zthread::Zthread(Zystem *pSys)
   : m_pSys(pSys)
{
}

Zthread::~Zthread()
{
}

bool Zthread::create(Zthread::ZthreadProc pfn)
{
   return false;
}

///////////////////////////////////////////////////////////////
// SDLThread

SDLThread::SDLThread(Zystem *pSys)
  : Zthread(pSys)
{
}

SDLThread::~SDLThread()
{
}

bool SDLThread::create(ZthreadProc pfn)
{
   return ((m_pThread = SDL_CreateThread(pfn, m_pSys)) ? true : false);
}
