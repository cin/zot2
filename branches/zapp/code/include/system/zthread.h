#pragma once

#include "zystem.h"

//#define __ZOT_USE_MSTHREADS__

#ifndef __ZOT_USE_MSTHREADS__
struct SDL_Thread;
#endif

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
   virtual void kill();

protected:

   Zthread(){}

};

#ifdef __ZOT_USE_MSTHREADS__
class MSThread : public Zthread
{

public:

   MSThread(ZthreadProc pfn, Zystem *pSys);
   virtual ~MSThread();

   virtual bool create(Zthread::ZthreadProc pfn, Zystem *pSys);
   virtual uint32 getThreadId() const;
   virtual void kill();

protected:

   uint32 m_hThread;
   uint32 m_threadId;

};
#else
class SDLThread : public Zthread
{

public:

   SDLThread(ZthreadProc pfn, Zystem *pSys);
   virtual ~SDLThread();
   virtual bool create(Zthread::ZthreadProc pfn, Zystem *pSys);
   virtual uint32 getThreadId() const;
   virtual void kill();

protected:

   SDL_Thread *m_pThread;

};
#endif

}
