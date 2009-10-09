#pragma once

#include "zmsg.h"
#include "zmsgq.h"
#include "zimtime.h"

#include <map>

namespace Zot
{

class Zthread;

class Zystem
{

public:

   Zystem(Zystem *pParent, bool bThreaded = false);
   virtual ~Zystem();

   virtual bool config(ZmCfg *pCfg);
   virtual bool init();
   virtual int onExit();
   virtual void run();
   virtual void tick();
   virtual void _();
   virtual bool saveConfig(ZmCfg *pCfg = NULL);

   // accessors
   uint8 getid() const { return m_id; }
   void setid(uint8 id) { m_id = id; }

   bool isRunning() const { return m_bRunning; }
   bool isThreaded() const { return m_bThreaded; }
   uint32 getThreadId() const;
   uint32 getTimeout() const { return m_timeout; }
   void setTimeout(uint32 timeout) { m_timeout = timeout; }

   uint32 getMask() const { return m_mask; }
   void setMask(uint32 mask) { m_mask = mask; }
   void resetMask() { m_mask = 0; }
   void addMask(uint32 mask) { m_mask |= mask; }
   void toggleMask(uint32 mask) { m_mask ^= mask; }
   void removeMask(uint32 mask) { m_mask &= ~mask; }

   virtual int onLog(Zmsg *pMsg);
   virtual int onTest(Zmsg *pMsg);

   Zystem *getParent() const { return m_pParent; }

   size_t getMsgqSize() const { return m_msgq.m_zmsgs.size(); }

   // push to parent -- this is the method all systems should be using
   // to pass messages to thier parent as it will set the msg system
   // id properly
   virtual void post(Zmsg *pMsg);
   virtual void post(Zmsg &msg);

protected:

   // method by which other systems push msgs to this system
   virtual void push(Zmsg *pMsg);
   virtual void push(Zmsg &msg);

   virtual int onConfig(Zmsg *pMsg);
   virtual int onStop(Zmsg *pMsg);

   // define the mfp typedef
   typedef int (Zystem::*ZmsgHandler)(Zmsg *);
   typedef std::map<uint32, ZmsgHandler> ZmsgHandlers;
   typedef ZmsgHandlers::iterator ZmhIter;

   virtual void reg(uint32 msgType, ZmsgHandler pmf);

   Zystem *m_pParent;

   bool m_bRunning;
   bool m_bThreaded;
   Zmsgq m_msgq;
   uint32 m_timeout;
   uint32 m_mask;
   ZmsgHandlers m_handlers;
   Zthread *m_pThread;
   uint8 m_id;
   ZimTime m_curTime;

private:

   static uint8 s_id;

   friend class Zapp;

};

}
