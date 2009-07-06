#include "zot.h"
#include "zystem.h"
#include "zthread.h"

#include <SDL.h>

using namespace Zot;

int bar(void *data)
{
   Zystem *foo = reinterpret_cast<Zystem *>(data);
   if (foo)
      foo->_();
   return -1;
}

Zystem::Zystem(bool bThreaded)
   : m_bRunning(false)
   , m_bThreaded(bThreaded)
   , m_timeout(17)
   , m_mask(0xffff0000)
   , m_pThread(NULL)
{
}

Zystem::~Zystem()
{
}

bool Zystem::config(ZmCfg *pCfg)
{
   return false;
}

bool Zystem::init()
{
   reg(ZM_CFG_MSG, &Zystem::onConfig);
   reg(ZM_STOP_MSG, &Zystem::onStop);
   m_bRunning = true;
   return true;
}

void Zystem::publish(Zot::Zmsg *pMsg)
{
   if (m_mask & (pMsg->__m_type & 0xffff0000))
      m_msgq.post(pMsg, m_timeout);
}

int Zystem::onConfig(Zmsg *pMsg)
{
   ZmCfg *pCfg = dynamic_cast<ZmCfg *>(pMsg);
   if (pCfg)
      return !config(pCfg);
   return -1;
}

int Zystem::onExit()
{
   if (m_pThread)
      delete m_pThread, m_pThread = NULL;
   return 0;
}

int Zystem::onStop(Zmsg *pMsg)
{
   m_bRunning = false;
   return 0;
}

void Zystem::reg(uint32 msgType, ZmsgHandler pmf)
{
   m_handlers[msgType] = pmf;
}

void Zystem::run()
{
   if (m_bThreaded)
   {
      m_pThread = new SDLThread(this);
      Zthread::ZthreadProc pfn = bar;
      m_pThread->create(pfn);
   }
   else
      _();
}

void Zystem::tick()
{
   Zmsg *pMsg = NULL;
   while (pMsg = m_msgq.wait(m_timeout))
   {
      // if mask passed, find handler and invoke
      // note: messages are now filtered upon post
      // if (m_mask & (pMsg->__m_type & 0xffff0000))
      ZmhIter it = m_handlers.find(pMsg->__m_type);
      if (it != m_handlers.end())
      {
         ZmsgHandler pmf = it->second;
         (this->*pmf)(pMsg);
      }
      delete pMsg;
   }
}

void Zystem::_()
{
   while (m_bRunning)
      tick();
   onExit();
}

bool Zystem::saveConfig(Zot::ZmCfg *pCfg)
{
   return false;
}
