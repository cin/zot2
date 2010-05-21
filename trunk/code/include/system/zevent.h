#pragma once

#ifdef _WIN32
#define __ZOT_USES_MS_EVENTS__
//#define __ZOT_USES_BOOST_EVENTS__
#else
//#define __ZOT_USES_BOOST_EVENTS__
#endif

#if defined(__ZOT_USES_BOOST_EVENTS__)
namespace boost
{
   class condition_variable;
   class mutex;
}
#elif (!defined(__ZOT_USES_MS_EVENTS__) && !defined(__ZOT_USES_BOOST_EVENTS__))
struct SDL_mutex;
struct SDL_cond;
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

#if defined(__ZOT_USES_MS_EVENTS__)

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

#elif defined(__ZOT_USES_BOOST_EVENTS__)

class BoostEvent : public Zevent
{

public:

   BoostEvent();
   virtual ~BoostEvent();

   virtual bool lock(uint32 timeout = ZOT_INDEFINITE);
   virtual bool unlock();
   virtual void signal();
   virtual bool wait(uint32 timeout = ZOT_INDEFINITE);

protected:

   boost::mutex *m_pMutex;
   boost::condition_variable *m_pCond;

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
