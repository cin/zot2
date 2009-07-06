#pragma once

#include <queue>
#include <vector>
#include "zmsg.h"
#include "zevent.h"

namespace Zot
{

/////////////////////////////////////////////////////////////////////
// Name: Zmsgq
// Purpose: Abstracts message handling and mutex for threading
// Zystems. Uses a priority msg queue that is protected by a
// mutex.
/////////////////////////////////////////////////////////////////////

class Zmsgq
{

public:

   typedef std::priority_queue<Zmsg *, std::vector<Zmsg *>, Zmsg::ZmsgComparison> Zmsgs;

   Zmsgq();
   virtual ~Zmsgq();

   virtual void post(Zmsg *pMsg, uint32 timeout);
   virtual Zmsg *wait(uint32 timeout);

protected:

   Zevent *m_pEvent;
   Zmsgs m_zmsgs;

   friend class Zystem;

};

}
