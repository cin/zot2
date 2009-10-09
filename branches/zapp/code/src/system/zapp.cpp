#include "zot.h"
#include "zapp.h"
#include "zogger.h"
#include "Zystress.h"
#include <windows.h>
#include <sstream>
#include "zthread.h"
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
      {
         wostringstream os;
         os << "Zapp::init: pLogger id " << pLogger->getThreadId() << endl;
         OutputDebugString(os.str().c_str());
      }
   }

   m_timeout = 10;
   return true;
}

int Zapp::onExit()
{
   {
      wostringstream os;
      int id = m_pThread ? m_pThread->getThreadId() : -1;
      os << "Zapp::onExit: id " << id << endl;
      OutputDebugString(os.str().c_str());
   }

   // send event stop event to all zystems
   ZmStop stop;
   for (ZysIter it = m_zystems.begin(); it != m_zystems.end(); it++)
      (*it)->push(&stop);

   // wait for all zystems to finish exiting
   // TODO: handle infinite wait...hopefully won't happen, but u know it will
   Zystem *pSys = NULL;
   ZimTime curTime(true);
   ZimTime exitTime(curTime + 5000);

   while (curTime.update() < exitTime && m_zystems.size())
   {
      for (ZysIter it = m_zystems.begin(); it != m_zystems.end();)
      {
         pSys = *it;
         if (pSys->isRunning())
         {
            it++;
         }
         else
         {
            pSys->onExit();
            delete *it;
            it = m_zystems.erase(it);
         }
      }
   }

   {
      wostringstream os;
      int id = m_pThread ? m_pThread->getThreadId() : -1;
      os << "Zapp::onExit: " << id << " calling base class' onExit" << endl;
      OutputDebugString(os.str().c_str());
   }

   Zystem::onStop(&stop);
   int ret = Zystem::onExit();
   return ret;
}

void Zapp::tick()
{
   if (!m_bRunning)
      return;

   // FIXME: the app needs to get some priority here...
   // unless the some priority is given to it there is a chance
   // that worker thread message traffic could cause the tick
   // to never empty the message queue.
   size_t qStartSz = getMsgqSize();
   ZimTime curTime;
   ZimTime finishTime(curTime + m_timeout);

   Zmsg *pMsg = m_msgq.wait(m_timeout);
   while (pMsg)
   {
      //{
      //   wostringstream os;
      //   os << "Zapp has msg in q. type: "
      //      << showbase << hex << pMsg->getType() << endl;
      //   OutputDebugString(os.str().c_str());
      //}

      // check if other zystems want this msg
      Zystem *pSys = NULL;
      for (ZysIter it = m_zystems.begin(); it != m_zystems.end(); it++)
      {
         pSys = *it;
         // don't send to the calling system and only send
         // to systems that care about this message type
         if (pSys->getid() != pMsg->getSystem() &&
            pSys->getMask() & (pMsg->getType() & ZM_ALL))
         {
            pSys->push(pMsg);
         }
      }

      // if mask passed, find handler and invoke
      // note: messages are now filtered upon post
      // if (m_mask & (pMsg->__m_type & ZM_ALL))
      ZmhIter it = m_handlers.find(pMsg->getType());
      if (it != m_handlers.end())
         (this->*(it->second))(pMsg);

      delete pMsg;

      if (curTime.update() < finishTime)
         pMsg = NULL;
      else if (--qStartSz == 0)
         pMsg = m_msgq.wait(m_timeout);
      else
         pMsg = m_msgq.get();
   }
}

void Zapp::addZystress()
{
   static uint32 sNumZystress(0);

   Zystress *pSys = new Zystress(this);
   pSys->run();
   m_zystems.push_back(pSys);
   sNumZystress++;

   {
      wostringstream os;
      os << "Zapp::addZystress: pSys id " << pSys->getThreadId() << "; sNumZystress: " << setw(3) << sNumZystress << endl;
      OutputDebugString(os.str().c_str());
   }
}
