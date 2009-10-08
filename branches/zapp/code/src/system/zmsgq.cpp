#include "zot.h"
#include "zmsgq.h"
#include "zevent.h"

using namespace std;
using namespace Zot;

Zmsgq::Zmsgq()
   : m_pEvent(NULL)
{
#ifdef WIN32
   m_pEvent = new MSEvent;
#else
   m_pEvent = new SDLEvent;
#endif
}

Zmsgq::~Zmsgq()
{
   if (m_pEvent)
      delete m_pEvent, m_pEvent = NULL;
}

Zmsg *Zmsgq::get()
{
   // safely remove the msg q's top element, which is what wait returns when a msg is ready
   m_pEvent->lock();
   Zmsg *pMsg = NULL;
   if (m_zmsgs.size())
   {
      pMsg = m_zmsgs.top();
      m_zmsgs.pop();
   }
   m_pEvent->unlock();
   return pMsg;
}

void Zmsgq::push(Zmsg *pMsg, uint32 timeout)
{
   m_pEvent->lock(timeout);
   m_zmsgs.push(pMsg->copy());
   m_pEvent->unlock();
   m_pEvent->signal();
}

Zmsg *Zmsgq::wait(uint32 timeout)
{
   return m_pEvent->wait(timeout) ? get() : NULL;
}
