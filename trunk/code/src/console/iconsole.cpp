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
   return (int)m_cmd.size();
}

const char *IConsole::getArgv(int i) const
{
   _ASSERT(i < (int)m_cmd.size());
   return m_cmd[i].c_str();
}

