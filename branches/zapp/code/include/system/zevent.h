#pragma once

struct SDL_mutex;
struct SDL_cond;

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
   virtual bool signal();
   virtual int32 wait(uint32 timeout = ZOT_INDEFINITE);

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
   virtual bool signal();
   virtual int32 wait(uint32 timeout = ZOT_INDEFINITE);

protected:

   SDL_mutex *m_pMutex;
   SDL_cond *m_pCond;

};

}
