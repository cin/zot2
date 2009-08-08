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
   virtual void tick();
   virtual void _();
   virtual bool saveConfig(ZmCfg *pCfg = NULL);

   // method by which other systems push msgs to this system
   virtual void push(Zmsg *pMsg);

   // accessors
   uint8 getid() const { return m_id; }
   void setid(uint8 id) { m_id = id; }

   bool isRunning() const { return m_bRunning; }
   bool isThreaded() const { return m_bThreaded; }
   uint32 getThreadId() const;
   uint32 getTimeout() const { return m_timeout; }
   void setTimeout(uint32 timeout) { m_timeout = timeout; }

   virtual int onLog(Zmsg *pMsg);

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
   uint8 m_id;

};

}
