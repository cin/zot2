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

void Zmsgq::post(Zmsg *pMsg, uint32 timeout)
{
   m_pEvent->lock();
   m_zmsgs.push(pMsg->copy());
   m_pEvent->unlock();
}

Zmsg *Zmsgq::wait(uint32 timeout)
{
   if (!m_pEvent->wait(timeout))
   {
      return m_zmsgs.top();
   }
   return NULL;
}
