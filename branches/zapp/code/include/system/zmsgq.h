#pragma once

#include <vector>
#include "zmsg.h"

namespace Zot
{

/////////////////////////////////////////////////////////////////////
// forward declarations
/////////////////////////////////////////////////////////////////////

class Zevent;
class Zmutex;

/////////////////////////////////////////////////////////////////////
// Name: Zmsgq
// Purpose: Abstracts message handling and mutex for threading
// Zystems. Uses a priority msg queue that is protected by a
// mutex.
/////////////////////////////////////////////////////////////////////

class Zmsgq
{

public:

   typedef std::vector<Zmsg *> Zmsgs;
   typedef Zmsgs::iterator ZmsgIter;

   Zmsgq();
   virtual ~Zmsgq();

protected:

   Zmutex *m_pMutex;
   Zevent *m_pEvent;
   Zmsgs m_zmsgs;

};

}
