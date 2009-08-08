#include "zot.h"
#include "zapp.h"
#include "zogger.h"

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

   // now for a real built in system
   Zogger *pLogger = Zogger::create();
   if (pLogger)
      m_zystems.push_back(pLogger);

   return true;
}

int Zapp::onExit()
{
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

   int ret = Zystem::onExit();
   return ret;
}
