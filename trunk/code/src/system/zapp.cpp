#include "zot.h"
#include "zapp.h"
#include "zogger.h"
#include "zystress.h"
#include "zthread.h"

#include <SDL.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <sstream>
#include <iomanip>

using namespace std;
using namespace Zot;

Zapp *Zapp::m_pApp = NULL;

Zapp::Zapp()
   : Zystem(NULL, false)
{
}

Zapp::~Zapp()
{
}

bool Zapp::config(ZmCfg *pCfg)
{
   return true;
}

bool Zapp::init()
{
   bool ret = Zystem::init();
   if (!ret)
      return ret;

   // now for a real built in system
   Zogger *pLogger = Zogger::create(this);
   if (pLogger)
   {
      m_zystems.push_back(pLogger);
      D({
         ostringstream os;
         os << "Zapp::init: pLogger id " << pLogger->getThreadId() << endl;
         //Zogger::get()->zog(os.str());
         cout << os.str() << endl;
      })
   }

//   for (int i = 0; i < 20; i++)
//   {
//      addZystress();
//      SDL_Delay(10);
//   }

   m_terrain.initTerrain();
   m_terrain.build();

   m_camera.setup();

   m_timeout = 10;
   return true;
}

int Zapp::onExit()
{
   D({
      ostringstream os;
      int id = m_pThread ? m_pThread->getThreadId() : -1;
      os << "Zapp::onExit: id " << id << endl;
      Zogger::get()->zog(os.str());
   })

   // send event stop event to all zystems
   ZmsgPtr stop(new ZmStop);
   for (ZysIter it = m_zystems.begin(); it != m_zystems.end(); it++)
      (*it)->push(stop);

   // try to wait for all zystems to finish exiting
   ZimTime exitTime(m_curTime.update() + 5000);
   while (m_curTime.update() < exitTime && m_zystems.size())
   {
      for (ZysIter it = m_zystems.begin(); it != m_zystems.end();)
      {
         if ((*it)->isRunning())
         {
            it++;
         }
         else
         {
            (*it)->onExit();
            delete *it;
            it = m_zystems.erase(it);
         }
      }
   }

   // if some systems aren't cooperating, kill them
   for (ZysIter it = m_zystems.begin(); it != m_zystems.end();)
   {
      (*it)->kill();
      delete *it;
      it = m_zystems.erase(it);
   }

   {
      D(wostringstream os;
      int id = m_pThread ? m_pThread->getThreadId() : -1;
      os << "Zapp::onExit: " << id << " calling base class' onExit" << endl;)
#ifdef _WIN32
      D(OutputDebugString(os.str().c_str());)
#else
      D(cout << os;)
#endif
   }

   Zystem::onStop(stop);
   int ret = Zystem::onExit();
   return ret;
}

void Zapp::tick()
{
   // the app needs to get some priority here...
   // unless the some priority is given to it there is a chance
   // that worker thread message traffic could cause the tick
   // to never empty the message queue.
   size_t qStartSz = getMsgqSize();
   ZimTime finishTime(m_curTime.update() + m_timeout);

   ZmsgPtr pMsg = m_msgq.wait(m_timeout);
   while (pMsg.get())
   {
      // check if other zystems want this msg
      Zystem *pSys = NULL;
      for (ZysIter it = m_zystems.begin(); it != m_zystems.end(); it++)
      {
         pSys = *it;
         // don't send to the calling system and only send
         // to systems that care about this message type
         if (pSys->getid() != pMsg->getSystem())
            pSys->push(pMsg);
      }

      // if mask passed, find handler and invoke
      // note: messages are now filtered upon post
      // if (m_mask & (pMsg->__m_type & ZM_ALL))
      ZmhIter it = m_handlers.find(pMsg->getType());
      if (it != m_handlers.end())
         (this->*(it->second))(pMsg);

      if (m_curTime.update() > finishTime)
         pMsg.reset();
      else if (--qStartSz == 0)
         pMsg = m_msgq.wait((finishTime - m_curTime).get());
      else
         pMsg = m_msgq.pop();
   }
}

void Zapp::addZystress()
{
   static uint32 sNumZystress(0);

   Zystress *pSys = new Zystress(this);
   pSys->run();
   m_zystems.push_back(pSys);
   sNumZystress++;

   D({
      ostringstream os;
      os << "Zapp::addZystress: pSys id " << setw(4) << pSys->getThreadId()
         << "; sNumZystress: " << setw(3) << sNumZystress << endl;
      Zogger::get()->zog(os.str());
   })
}
