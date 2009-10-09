#include "zot.h"
#include "zystress.h"
#include <limits>
#include <sstream>
#include <windows.h>
#include "zapp.h"
#include "convar.h"

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
   post(Zmsg(ZM_TEST_MSG, this));
   m_timeout = 10;
   return ret;
}

void Zystress::tick()
{
   Zystem::tick();
   if (m_time.update() > m_nextTime)
   {
      post(Zmsg(ZM_TEST_MSG, this));
      m_nextTime = m_time + stressDelay.getInt32();
   }
}

int Zystress::onTest(Zmsg *msg)
{
   if (++m_eventCounter >= UINT_MAX)
   {
      m_eventCounter = 0;
      D(wostringstream os;
      os << "Zystress::onTest: pSys id " << getThreadId()
         << " wrapped at " << m_time.get() << endl;
      OutputDebugString(os.str().c_str());)
   }

   D(if (getMsgqSize() != m_lastQSz)
   {
      wostringstream os;
      os << "Zystress::onTest: pSys id " << getThreadId()
         << " msgq size: " << getMsgqSize() << " at " << m_time.get() << endl;
      OutputDebugString(os.str().c_str());
      m_lastQSz = getMsgqSize();
   })
   return 0;
}
