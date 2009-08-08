#include "zot.h"
#include "zystem.h"
#include "zthread.h"
#include "zogger.h"

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
   , m_mask(ZM_ALL)
   , m_pThread(NULL)
   , m_id(0)
{
}

Zystem::~Zystem()
{
}

bool Zystem::config(ZmCfg *pCfg)
{
   return false;
}

uint32 Zystem::getThreadId() const
{
   return m_pThread ? m_pThread->getThreadId() : SDL_ThreadID();
}

bool Zystem::init()
{
   reg(ZM_CFG_MSG, &Zystem::onConfig);
   reg(ZM_STOP_MSG, &Zystem::onStop);
   reg(ZM_LOG_MSG, &Zystem::onLog);
   m_bRunning = true;
   return true;
}

void Zystem::push(Zot::Zmsg *pMsg)
{
   if (m_mask & (pMsg->__m_type & ZM_ALL))
   {
      // make sure msg has a valid system
      if (pMsg->__m_system <= 0)
         pMsg->__m_system = getid();
      m_msgq.push(pMsg, m_timeout);
   }
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

int Zystem::onLog(Zmsg *pMsg)
{
   Zogger::get()->onLog(pMsg);
   return 0;
}

void Zystem::reg(uint32 msgType, ZmsgHandler pmf)
{
   m_handlers[msgType] = pmf;
}

void Zystem::run()
{
   if (m_bThreaded)
      m_pThread = new SDLThread(bar, this);
   else
      _();
}

void Zystem::tick()
{
   Zmsg *pMsg = m_msgq.wait(m_timeout);
   while (pMsg)
   {
      // if mask passed, find handler and invoke
      // note: messages are now filtered upon post
      // if (m_mask & (pMsg->__m_type & ZM_ALL))
      ZmhIter it = m_handlers.find(pMsg->__m_type);
      if (it != m_handlers.end())
         (this->*(it->second))(pMsg);

      delete pMsg;
      pMsg = m_msgq.wait(m_timeout);
   }
}

void Zystem::_()
{
   while (m_bRunning)
      tick();
   // MOVED: since onExit is deleting the thread, this needs to be called at the app level (creator of the threads)
   //onExit();
}

bool Zystem::saveConfig(Zot::ZmCfg *pCfg)
{
   return false;
}
