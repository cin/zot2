#include "zot.h"

#ifdef _WIN32
#include <time.h>
#endif

#include "zogger.h"
#include "zonsole.h"

using namespace std;
using namespace Zot;

Zogger *Zogger::zogger = NULL;

ConVar zotLogFileName("zotLogFileName", "../log/zot.log", ConVar::EConVarArchive, "Sets the name of the zot logfile.");
ConVar zotLogLevel("zotLogLevel", "5", ConVar::EConVarArchive, "Sets the log level of the zot logfile.");

Zogger::Zogger(Zystem *pParent, const string &file)
   : Zystem(pParent, true)
   , filename(file)
{
}

Zogger::~Zogger()
{
}

Zogger *Zogger::create(Zystem *pParent)
{
   if (!zogger)
   {
      zogger = new Zogger(pParent, zotLogFileName.getString());
      if (zogger->init())
         zogger->run();
   }
   return zogger;
}

bool Zogger::init()
{
   if (!Zystem::init())
      return false;

   // TODO: back up the old file
   ofs.open(filename.c_str(), ios::trunc);
   reg(ZM_LOG_MSG, &Zystem::onLog);
   m_bRunning = true;
   return true;
}

int Zogger::onExit()
{
   if (ofs)
      ofs.close();
   Zystem::onExit();
   return 0;
}

int Zogger::onLog(Zmsg *msg)
{
   ZmLog *pMsg = dynamic_cast<ZmLog *>(msg);
   if (!pMsg)
      return -1;

   // hacky .. improve please
   char date[9], time[9];
   _strdate_s(date);
   _strtime_s(time);
#if 0
   // append timestamp to message
   string timestamp = string(date) + " " + string(time);
   string fullmsg = timestamp + string(" ") + pMsg->m_msg;
#else // no need to check log level here b/c it's filtered at the pushing level
   ostringstream os;
   os << date << "_" << time << " : " << (uint16)pMsg->m_level << " : " << pMsg->m_msg;
   string fullmsg(os.str()); // done this way so string won't get copied for each os call
#endif

   // zog the message based on the destination

#if 0 // TODO: update this...cannot do this btwn threads...access the renderer that is
   // having to test if the Zonsole is available, sort of ugly
   // maybe if the Zonsole is not available, just cache
   // the log messages and then when it's ready, dump them
   // all in.
   if ((pMsg->dest & ZOG_CONSOLE) && Zonsole::get())
   {
      switch (l)
      {
      case ZOG_DEBUG: Zonsole::get()->debug(fullmsg); break;
      case ZOG_INFO: break;
      case ZOG_WARN: break;
      case ZOG_ERROR: break;
      default: break;
      }
   }
#endif
   // this is ugly. maybe have STDERR trump STDOUT if it is set?
   // MOD: now forcing users to end their own lines...may want to call this
   // multiple times w/out necessarily always ending
   // should file already be opened and awaiting data?
   // if so, the stream should be a memer, and we can simply
   // write to it here.
   if (pMsg->m_dest & ZOG_STDOUT) cout << fullmsg;
   if (pMsg->m_dest & ZOG_STDERR) cerr << fullmsg;
   if (pMsg->m_dest & ZOG_FILE)   ofs  << fullmsg;
   return 0;
}

void Zogger::zog(const string &msg)
{
   if (Zmsg::ZOT_PRIORITY_NORMAL >= zotLogLevel.getInt32())
      push(ZmLog(msg, Zmsg::ZOT_PRIORITY_NORMAL, Zogger::ZOG_FILE));
}

void Zogger::zog(const string &msg, ZogLevel l, int dest)
{
   if (l >= zotLogLevel.getInt32())
      push(ZmLog(msg, l, dest));
}
