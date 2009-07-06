#pragma once

#include "zmsg.h"
#include "zmsgq.h"

#include <map>

namespace Zot
{

   class Zthread;

class Zystem
{

public:

   Zystem(bool bThreaded = false);
   virtual ~Zystem();

   virtual bool config(ZmCfg *pCfg);
   virtual bool init();
   virtual int onExit();
   virtual void run();
   virtual void _();
   virtual bool saveConfig(ZmCfg *pCfg = NULL);

   // method by which other systems publish to this...
   virtual void publish(Zmsg *pMsg);

protected:

   virtual int onConfig(Zmsg *pMsg);
   virtual int onStop(Zmsg *pMsg);

   // define the mfp typedef
   typedef int (Zystem::*ZmsgHandler)(Zmsg *);
   typedef std::map<uint32, ZmsgHandler> ZmsgHandlers;
   typedef ZmsgHandlers::iterator ZmhIter;

   virtual void reg(uint32 msgType, ZmsgHandler pmf);

   bool m_bRunning;
   bool m_bThreaded;
   Zmsgq m_msgq;
   uint32 m_timeout;
   uint32 m_mask;
   ZmsgHandlers m_handlers;
   Zthread *m_pThread;

};

}
