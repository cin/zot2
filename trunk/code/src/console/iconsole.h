#ifndef __ICONSOLE_H__
#define __ICONSOLE_H__

#ifdef _WIN32
#pragma once
#endif

#include <vector>
#include <deque>
#include <map>

#include "convar.h"

namespace Zot
{

class IConsole
{

public:

   typedef std::map<std::string, ConVar> ConVars;

   typedef std::vector<std::string> CmdArgs;
   typedef CmdArgs::iterator        CmdArgIter;

   typedef std::deque<std::string>  PrevCmds;
   typedef PrevCmds::iterator       PcIter;

public:

   virtual ~IConsole() {}

   static IConsole *create();

   static IConsole *get();

   int getArgc() const;
   const char *getArgv(int i) const;

   // output level
   // destination (error, stdout, console logfile, console)
   // handling input from cegui

protected:

   static IConsole  *m_spConsole;

   ConVars           m_convars;
   CmdArgs           m_cmd;
   PrevCmds          m_prevCmds;
   std::string       m_wrkBuf;

protected:

   IConsole() {}

};

}

#endif // __ICONSOLE_H__
