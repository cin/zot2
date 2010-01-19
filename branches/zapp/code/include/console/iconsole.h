#pragma once

#include <vector>
#include <deque>
#include <map>

#include "convar.h"

namespace Zot
{

class IConsole
{

public:

   typedef std::map<std::string, ConVar *> ConVars;
   typedef ConVars::iterator               ConVarIter;

   typedef std::vector<std::string> CmdArgs;
   typedef CmdArgs::iterator        CmdArgIter;

   typedef std::deque<std::string>  PrevCmds;
   typedef PrevCmds::iterator       PcIter;

   typedef std::vector<std::string> CmdMatches;
   typedef CmdMatches::iterator     CmdMatchIter;

public:

   virtual ~IConsole() {}

   static IConsole *get();

   ConVar *findCommand(const std::string &cmd);

   int getArgc() const;
   const char *getArgv(int i) const;

   // output level
   // destination (error, stdout, console logfile, console)
   // handling input from cegui

   virtual bool init() = 0;

   virtual void debug(const std::string &s) = 0;
   virtual void info(const std::string &s) = 0;
   virtual void warn(const std::string &s) = 0;
   virtual void error(const std::string &s) = 0;

protected:

   static IConsole *create();

   static IConsole  *m_pConsole;

   ConVars           m_convars;
   CmdArgs           m_cmd;
   CmdMatches        m_matches;
   PrevCmds          m_prevCmds;
   std::string       m_wrkBuf;
   std::size_t       m_cachedCmdsMax;

   // allows the ConVar to add things to the convar list abstractly,
   // without providing a public accessor to the lists
   friend class ConVar;

protected:

   IConsole()
      : m_cachedCmdsMax(10)
   {
   }

   void handleCmd(const char *cmd);
   void handleTextChanged(const char *txt);

};

}
