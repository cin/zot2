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

   typedef std::priority_queue<ZmsgPtr, std::vector<ZmsgPtr>, Zmsg::ZmsgComparison> Zmsgs;

   Zmsgq();
   virtual ~Zmsgq();

   virtual bool push(ZmsgPtr pMsg, uint32 timeout);
   virtual ZmsgPtr wait(uint32 timeout);

   ZmsgPtr pop();

protected:

   Zevent *m_pEvent;
   Zmsgs m_zmsgs;

   friend class Zystem;

};

}
