#include "stdafx.h"
#include "iconsole.h"

using namespace std;
using namespace Zot;

IConsole *IConsole::m_pConsole = NULL;

IConsole *IConsole::get()
{
   if (!m_pConsole)
      create();
   return m_pConsole;
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

void IConsole::handleCmd(const string &cmd)
{
   // TODO: parse cmd into tokens
   ConVarIter i = m_convars.find(cmd);
   if (i != m_convars.end())
   {
      ConVar *cv = i->second;
      if (cv)
      {
         ConVar::ConVarPfn pfn = cv->getPfn();
         if (pfn)
            pfn();
      }
   }

   m_prevCmds.push_back(cmd);
   if (m_prevCmds.size() > m_cachedCmdsMax)
      m_prevCmds.pop_front();
}
