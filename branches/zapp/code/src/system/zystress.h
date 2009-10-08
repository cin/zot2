#pragma once

#include "zystem.h"

namespace Zot
{

class Zystress : public Zystem
{

public:

   Zystress(Zystem *pParent);
   virtual ~Zystress();

   virtual bool init();
   virtual void tick();
   virtual int onTest(Zmsg *pMsg);

protected:
   uint32 m_eventCounter;
   ZimTime m_startTime;
   ZimTime m_nextTime;
   ZimTime m_time;

};

}
