#pragma once

#ifndef WIN32
#include "SDL.h"
#endif

namespace Zot
{

/////////////////////////////////////////////////////////////////////
// Name: Zevent
// Purpose: basic event handling interface
/////////////////////////////////////////////////////////////////////

class Zevent
{

public:

   static const uint32 ZOT_INDEFINITE = 0xffffffff;

   virtual ~Zevent();

   virtual bool lock(uint32 timeout = ZOT_INDEFINITE);
   virtual bool unlock();
   virtual void signal();
   virtual bool wait(uint32 timeout = ZOT_INDEFINITE);

protected:

   Zevent();

};

#ifdef WIN32
class MSEvent : public Zevent
{

public:

   MSEvent();
   virtual ~MSEvent();

   virtual bool lock(uint32 timeout = ZOT_INDEFINITE);
   virtual bool unlock();
   virtual void signal();
   virtual bool wait(uint32 timeout = ZOT_INDEFINITE);

protected:

   uint32 m_hEvent;
   uint32 m_hMutex;

};
#else
class SDLEvent : public Zevent
{

public:

   SDLEvent();
   virtual ~SDLEvent();

   virtual bool lock(uint32 timeout = ZOT_INDEFINITE);
   virtual bool unlock();
   virtual void signal();
   virtual bool wait(uint32 timeout = ZOT_INDEFINITE);

protected:

   SDL_mutex *m_pMutex;
   SDL_cond *m_pCond;

};
#endif

}
