#include "zot.h"
#include "zystem.h"
#include "zthread.h"
#include "zogger.h"
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#include <time.h>
#endif

#include <SDL.h>

using namespace std;
using namespace Zot;

uint8 Zystem::s_id(0);

int bar(void *data)
{
   Zystem *foo = reinterpret_cast<Zystem *>(data);
   if (foo)
      foo->_();
   return -1;
}

Zystem::Zystem(Zystem *pParent, bool bThreaded)
   : m_pParent(pParent)
   , m_bRunning(false)
   , m_bThreaded(bThreaded)
   , m_timeout(17)
   , m_mask(ZM_ALL)
   , m_pThread(NULL)
   , m_id(++s_id)
   , m_curTime(true)
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
   m_bRunning = true;
   m_curTime.update();
   return true;
}

void Zystem::kill()
{
   m_pThread->kill();
}

void Zystem::post(ZmsgPtr pMsg)
{
   // make sure msg has a valid system
   if (pMsg->__m_system <= 0)
      pMsg->__m_system = getid();

   if (m_pParent)
      m_pParent->push(pMsg);
   else
      push(pMsg);
}

void Zystem::push(ZmsgPtr pMsg)
{
   if (m_mask & (pMsg->__m_type & ZM_ALL))
      m_msgq.push(pMsg, m_timeout);
}

int Zystem::onConfig(ZmsgPtr pMsg)
{
   ZmCfg *pCfg = dynamic_cast<ZmCfg *>(pMsg.get());
   if (pCfg)
      return !config(pCfg);
   return -1;
}

int Zystem::onExit()
{
#ifdef _DEBUG
   wostringstream os;
   int id = m_pThread ? m_pThread->getThreadId() : -1;
   os << "Zystem::onExit: id " << id << endl;
#ifdef _WIN32
   OutputDebugString(os.str().c_str());
#else
   cout << os;
#endif
#endif

   if (m_pThread)
      delete m_pThread, m_pThread = NULL;

   return 0;
}

int Zystem::onStop(ZmsgPtr pMsg)
{
#ifdef _DEBUG
   wostringstream os;
   int id = m_pThread ? m_pThread->getThreadId() : -1;
   os << "Zystem::onStop: id " << id << endl;
#ifdef _WIN32
   OutputDebugString(os.str().c_str());
#else
   cout << os;
#endif
#endif

   m_bRunning = false;
   return 0;
}

int Zystem::onLog(ZmsgPtr pMsg)
{
   return 0;
}

int Zystem::onTest(ZmsgPtr pMsg)
{
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
#ifdef __ZOT_USE_MSTHREADS__
      m_pThread = new MSThread(bar, this);
#else
      m_pThread = new SDLThread(bar, this);
#endif
   }
   else
      _();
}

void Zystem::tick()
{
   size_t qStartSz = getMsgqSize();
   ZimTime finishTime(m_curTime.update() + m_timeout);

   ZmsgPtr pMsg = m_msgq.wait(m_timeout);
   while (pMsg.get())
   {
      // if mask passed, find handler and invoke
      // note: messages are now filtered upon post
      // if (m_mask & (pMsg->__m_type & ZM_ALL))
      ZmhIter it = m_handlers.find(pMsg->__m_type);
      if (it != m_handlers.end())
         (this->*(it->second))(pMsg);

      if (!m_bRunning || m_curTime.update() > finishTime)
         pMsg.reset();
      else if (--qStartSz == 0)
         pMsg = m_msgq.wait((finishTime - m_curTime).get());
      else
         pMsg = m_msgq.pop();
   }
}

void Zystem::_()
{
   init();
   while (m_bRunning)
      tick();
   // MOVED: since onExit is deleting the thread,
   // this needs to be called at the app level (creator of the threads)
   //onExit();
}

bool Zystem::saveConfig(Zot::ZmCfg *pCfg)
{
   return false;
}
