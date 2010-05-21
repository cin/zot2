#include "zot.h"
#include "iconsole.h"
#include <sstream>

#include <string.h>   // Linux needs this

using namespace std;
using namespace Zot;

IConsole *IConsole::m_pConsole = NULL;

IConsole *IConsole::get()
{
   if (!m_pConsole)
      create();
   return m_pConsole;
}

ConVar *IConsole::findCommand(const std::string &cmd)
{
   ConVarIter i = m_convars.find(cmd);
   if (i == m_convars.end())
      return NULL;
   return i->second;
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

void IConsole::handleCmd(const char *cmd)
{
   // clear out the old command
   m_cmd.clear();

   // parse cmd into tokens
   stringstream ss(cmd);
   string buf;

   // this actually needs to be more complex to handle
   // processing double quoted strings as one argument
   while (ss >> buf)
      m_cmd.push_back(buf);

   // if command is empty, do nothing
   if (!m_cmd.size())
      return;

   // process the command
   string theCmd(m_cmd[0]);
   ConVarIter i = m_convars.find(theCmd);
   if (i != m_convars.end())
   {
      ConVar *cv = i->second;
      if (cv)
      {
         // is there a handler to invoke?
         ConVar::ConVarPfn pfn = cv->getPfn();
         if (pfn)
         {
            pfn();
         }
         else
         {
            // if no handler, then it's a regular variant convar
            // are there arguments to process?
            if (m_cmd.size() > 1)
            {
               string val;
               CmdArgIter caIt(m_cmd.begin());
               caIt++; // get past the cmd itself

               // FIXME: doing this makes the cmd tokenizing seem ridiculous
               // could just remove the actual command from the string
               for (; caIt != m_cmd.end(); caIt++)
                  val += *caIt;

               // forces change handler to be called as well
               cv->setString(val);
            }
            else
            {
               // show value in console...don't like referring to the
               ostringstream os;
               os << "\"" << cv->getName() << "\" = \"" << cv->getString()
                  << "\" (default: \"" << cv->getDefaultValue() << "\")";
               info(os.str());
            }
         }
      }
   }

   // update the previous commands list
   m_prevCmds.push_back(cmd);
   if (m_prevCmds.size() > m_cachedCmdsMax)
      m_prevCmds.pop_front();
}

void IConsole::handleTextChanged(const char *txt)
{
   m_matches.clear();

   if (!txt || !strlen(txt))
      return;

   m_wrkBuf = txt;
   for (ConVarIter i = m_convars.begin(); i != m_convars.end(); i++)
   {
      if (m_wrkBuf.length() < i->first.length())
      {
         if (!strncmp(m_wrkBuf.c_str(), i->first.c_str(), m_wrkBuf.length()))
            m_matches.push_back(i->first);
      }
   }
}
