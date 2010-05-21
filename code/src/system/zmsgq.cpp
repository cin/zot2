#include "zot.h"
#include "zmsgq.h"
#include "zevent.h"

using namespace std;
using namespace Zot;

Zmsgq::Zmsgq()
   : m_pEvent(NULL)
{
#ifdef __ZOT_USES_MS_EVENTS__
   m_pEvent = new MSEvent;
#elif defined(__ZOT_USES_BOOST_EVENTS__)
   m_pEvent = new BoostEvent;
#else
   m_pEvent = new SDLEvent;
#endif
}

Zmsgq::~Zmsgq()
{
#if 0
   if (m_pEvent)
   {
      m_pEvent->lock();
      Zmsg *pMsg = NULL;
      while (m_zmsgs.size())
      {
         pMsg = m_zmsgs.top();
         delete pMsg;
         m_zmsgs.pop();
      }
      m_pEvent->unlock();
   }
#endif

   if (m_pEvent)
      delete m_pEvent, m_pEvent = NULL;
}

ZmsgPtr Zmsgq::pop()
{
   // safely remove the msg q's top element, which is what wait returns when a msg is ready
   ZmsgPtr pMsg;
   if (m_pEvent->lock())
   {
      if (m_zmsgs.size())
      {
         pMsg = m_zmsgs.top();
         m_zmsgs.pop();
      }
      m_pEvent->unlock();
   }
   return pMsg;
}

bool Zmsgq::push(ZmsgPtr pMsg, uint32 timeout)
{
   bool bLocked = m_pEvent->lock(timeout);
   if (bLocked)
   {
      m_zmsgs.push(pMsg);
      m_pEvent->unlock();
      m_pEvent->signal();
   }
   return bLocked;
}

ZmsgPtr Zmsgq::wait(uint32 timeout)
{
   return m_pEvent->wait(timeout) ? pop() : ZmsgPtr();
}
