#pragma once

#include "zystem.h"

struct SDL_Thread;

namespace Zot
{

class Zthread
{

public:

   typedef int (*ZthreadProc)(void *);

   Zthread(Zystem *pSys);
   virtual ~Zthread();
   virtual bool create(ZthreadProc pfn);

protected:

   Zystem *m_pSys;
};

class SDLThread : public Zthread
{

public:

   SDLThread(Zystem *pSys);
   virtual ~SDLThread();
   virtual bool create(Zthread::ZthreadProc pfn);

protected:

   SDL_Thread *m_pThread;

};

}
