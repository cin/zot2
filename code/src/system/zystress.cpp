#include "zot.h"
#include "zystress.h"
#include "zapp.h"
#include "convar.h"
#include "zogger.h"

#include <limits>
#include <sstream>

using namespace std;
using namespace Zot;

ConVar stressDelay("stressDelay", "10", ConVar::EConVarArchive, "Stress test thread message delay");

Zystress::Zystress(Zystem *pParent)
   : Zystem(pParent, true)
   , m_eventCounter(0)
   , m_lastQSz(0)
{
}

Zystress::~Zystress()
{
}

bool Zystress::init()
{
   bool ret = Zystem::init();
   reg(ZM_TEST_MSG, &Zystem::onTest);
   m_startTime = ZimTime(true);
   m_nextTime = m_startTime + stressDelay.getInt32();
   ZmsgPtr ptr(new Zmsg(ZM_TEST_MSG, this));
   post(ptr);
   m_timeout = 10;
   return ret;
}

void Zystress::tick()
{
   Zystem::tick();
   if (m_time.update() > m_nextTime)
   {
      ZmsgPtr ptr(new Zmsg(ZM_TEST_MSG, this));
      post(ptr);
      m_nextTime = m_time + stressDelay.getInt32();
   }
}

int Zystress::onTest(ZmsgPtr msg)
{
   if (++m_eventCounter >= UINT_MAX)
   {
      m_eventCounter = 0;
      D(ostringstream os;
      os << "Zystress::onTest: pSys id " << getThreadId()
         << " wrapped at " << m_time.get() << endl;
      Zogger::get()->zog(os.str());)
   }

   if (getMsgqSize() != m_lastQSz)
   {
      ostringstream os;
      os << "Zystress::onTest: pSys id " << getThreadId()
         << " msgq size: " << getMsgqSize() << " at " << m_time.get() << endl;
      Zogger::get()->zog(os.str());
      m_lastQSz = getMsgqSize();
   }

#if 0 // testing a whole lotta logging
   {
      ostringstream os;
      os << "Zystress::onTest: pSys id " << getThreadId()
         << " msgq size: " << getMsgqSize() << " at " << m_time.get() << endl;
      Zogger::get()->zog(os.str());
   }
#endif

   return 0;
}
