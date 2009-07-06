#pragma once

#include "zystem.h"

struct SDL_Thread;

namespace Zot
{

class Zthread
{

public:

   typedef int (*ZthreadProc)(void *);

   Zthread(ZthreadProc pfn, Zystem *pSys);
   virtual ~Zthread();
   virtual bool create(Zthread::ZthreadProc pfn, Zystem *pSys) = 0;
   virtual uint32 getThreadId() const;

protected:

   Zthread(){}

};

class SDLThread : public Zthread
{

public:

   SDLThread(ZthreadProc pfn, Zystem *pSys);
   virtual bool create(Zthread::ZthreadProc pfn, Zystem *pSys);
   virtual uint32 getThreadId() const;

protected:

   SDL_Thread *m_pThread;

};

}
