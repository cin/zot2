#include "zot.h"
#include "zogger.h"
#include "zonsole.h"
#include <iomanip>

#ifdef _WIN32
#include "zutil.h"

#include <time.h>
#include <sys/stat.h>
#include <windows.h>
#else
#include <sys/time.h>
#endif

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
      zogger->run();
   }
   return zogger;
}

bool Zogger::init()
{
   if (!Zystem::init())
      return false;

   if (ofs)
   {
      ofs.close();
      ofs.clear();
   }

#ifdef _WIN32
   // TODO: handle pruning logs once x backups have been made?
   // would help tidy up the log directory a bit but may also
   // remove files that the user wants to keep around
   struct _stat finfo;
   if (0 == _stat(filename.c_str(), &finfo) && 0 < finfo.st_size)
   {
      tm tinfo;
      char buf[16];

      // convert the file's last modified time to something more useful
      localtime_s(&tinfo, &finfo.st_mtime);
      strftime(buf, sizeof(buf), "%y%m%d_%H%M%S", &tinfo);

      // strip off and save extension for use when putting
      // filename back together
      // if no extension, simply assign and keep on truckin'
      size_t dot = filename.find_last_of('.');
      string filebase, ext;
      if (dot == string::npos)
         filebase = filename;
      else
      {
         filebase = filename.substr(0, dot);
         ext = filename.substr(dot + 1);
      }

      // construct the new filename and rename
      ostringstream fns;
      fns << filebase << "_" << buf << "." << ext;
      rename(filename.c_str(), fns.str().c_str());
   }
#endif

   ofs.open(filename.c_str(), ios::trunc);
   if (!ofs)
   {
      wostringstream os;
      wstring wfn(filename.begin(), filename.end());
      os << "Zogger::init: failed to open log file: " << wfn << endl;
#ifdef _WIN32
      OutputDebugString(os.str().c_str());
#else
      cout << os;
#endif
   }

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

int Zogger::onLog(ZmsgPtr msg)
{
   // no need to check log level here b/c it's filtered at the pushing level
   ZmLog *pMsg = dynamic_cast<ZmLog *>(msg.get());
   if (!pMsg)
      return -1;

   timeval tv;
   tm tinfo;
   char date[16];

	gettimeofday(&tv, NULL);
   time_t secs = tv.tv_sec;
#ifdef _WIN32
	localtime_s(&tinfo, &secs);
#else
	localtime_r(&secs, &tinfo);
#endif
   strftime(date, sizeof(date), "%y%m%d_%H:%M:%S", &tinfo);

   ostringstream os;
   os << date << "." << left << setw(3) << (int)(tv.tv_usec / 1000)
      << " | " << right << setw(2) << (uint16)pMsg->m_level
      << " | " << pMsg->m_msg;
   string fullmsg(os.str()); // done this way so string won't get copied for each os call

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

   // zog the message based on the destination
   //
   // this is ugly. maybe have STDERR trump STDOUT if it is set?
   // MOD: now forcing users to end their own lines...may want to call this
   // multiple times w/out necessarily always ending
   // should file already be opened and awaiting data?
   // if so, the stream should be a memer, and we can simply
   // write to it here.
   if (pMsg->m_dest & ZOG_STDOUT) cout << fullmsg;
   if (pMsg->m_dest & ZOG_STDERR) cerr << fullmsg;
   if (pMsg->m_dest & ZOG_FILE) ofs << fullmsg, ofs.flush();

   return 0;
}

void Zogger::zog(const string &msg)
{
   if (Zmsg::ZOT_PRIORITY_NORMAL >= zotLogLevel.getInt32())
   {
      ZmsgPtr ptr(new ZmLog(msg, Zmsg::ZOT_PRIORITY_NORMAL, Zogger::ZOG_FILE));
      push(ptr);
   }
}

void Zogger::zog(const string &msg, ZogLevel l, int dest)
{
   if (l >= zotLogLevel.getInt32())
   {
      ZmsgPtr ptr(new ZmLog(msg, l, dest));
      push(ptr);
   }
}
