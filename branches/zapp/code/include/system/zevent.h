#pragma once

#include "SDL.h"

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

   virtual bool lock();
   virtual bool unlock();
   virtual void push();
   virtual void pump();
   virtual bool poll();
   virtual bool wait(uint32 timeout = ZOT_INDEFINITE);

protected:

   Zevent();

};

class SDLEvent : public Zevent
{

public:

   SDLEvent();
   virtual ~SDLEvent();

   virtual bool lock();
   virtual bool unlock();
   virtual void push();
   virtual void pump();
   virtual bool poll();
   virtual bool wait(uint32 timeout = ZOT_INDEFINITE);

protected:

   SDL_mutex *m_pMutex;
   SDL_cond *m_pCond;
   SDL_Event *m_pEvent;

};

}
