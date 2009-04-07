#include "stdafx.h"
#include "iconsole.h"

using namespace std;
using namespace Zot;

IConsole *IConsole::m_spConsole = NULL;

IConsole *IConsole::get()
{
   if (!m_spConsole)
      create();
   return m_spConsole;
}

int IConsole::getArgc() const
{
   return 0;
}

const char *IConsole::getArgv(int i) const
{
   return 0;
}

/////////////////////////////////////////////////////////
// just a little test console that uses the interface

class TestConsole : public IConsole
{

public:

   TestConsole();

};

TestConsole::TestConsole()
{
}

IConsole *IConsole::create()
{
   m_spConsole = new TestConsole();
   return m_spConsole;
}
