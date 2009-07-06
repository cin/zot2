#include "zot.h"
#include "zmsgq.h"
#include "zevent.h"

using namespace std;
using namespace Zot;

Zmsgq::Zmsgq()
   : m_pEvent(NULL)
{
   m_pEvent = new SDLEvent;
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
   Zmsg *pMsg = m_zmsgs.top();
   m_zmsgs.pop();
   m_pEvent->unlock();
   return pMsg;
}

void Zmsgq::push(Zmsg *pMsg, uint32 timeout)
{
   m_pEvent->lock();
   m_zmsgs.push(pMsg->copy());
   m_pEvent->unlock();
}

Zmsg *Zmsgq::wait(uint32 timeout)
{
   return m_pEvent->wait(timeout) ? get() : NULL;
}
