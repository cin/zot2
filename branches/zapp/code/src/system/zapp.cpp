#include "zot.h"
#include "zapp.h"
#include "zogger.h"
#include <windows.h>
#include <sstream>
#include "zthread.h"

using namespace std;
using namespace Zot;

Zapp *Zapp::m_pApp = NULL;

Zapp::Zapp()
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

   // testing stuff for now...until created dynamically by cfg file
   Zystem *pSys = new Zystem(true);
   if (pSys->init())
   {
      pSys->run();
      m_zystems.push_back(pSys);
   }

   {
      wostringstream os;
      os << "Zapp::init: pSys id " << pSys->getThreadId() << endl;
      OutputDebugString(os.str().c_str());
   }

   // now for a real built in system
   Zogger *pLogger = Zogger::create();
   if (pLogger)
   {
      m_zystems.push_back(pLogger);
      {
         wostringstream os;
         os << "Zapp::init: pLogger id " << pLogger->getThreadId() << endl;
         OutputDebugString(os.str().c_str());
      }
   }


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
   {
      (*it)->push(&stop);
   }

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

   int ret = Zystem::onExit();
   return ret;
}
