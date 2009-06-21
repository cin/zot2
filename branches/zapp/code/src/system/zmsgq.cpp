#include "zot.h"
#include "zmsgq.h"
#include "zevent.h"

using namespace std;
using namespace Zot;

Zmsgq::Zmsgq()
   : m_pEvent(NULL)
   , m_pMutex(NULL)
{
}

Zmsgq::~Zmsgq()
{
}
