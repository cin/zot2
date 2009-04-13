#include "stdafx.h"

#include <iostream>

#include <time.h>

#include "zogger.h"
#include "zonsole.h"

using namespace std;
using namespace Zot;

Zogger *Zogger::zogger = NULL;

Zogger::Zogger(ZogLevel l, int dest, const string &file)
{
   level = l;
   destMask = dest;
   filename = file;
}

Zogger::~Zogger()
{
}

void Zogger::create(ZogLevel l, int dest, const string &file)
{
   if (!zogger)
   {
      zogger = new Zogger(l, dest, file);
   }
}

void Zogger::zog(const string &msg)
{
   zog(msg, level, destMask);
}

void Zogger::zog(const string &msg, ZogLevel l, int dest)
{
   // hacky .. improve please
   char date[9], time[9];
   _strdate_s(date);
   _strtime_s(time);
   string timestamp = string(date) + " " + string(time);

   // append timestamp to message
   string fullmsg = timestamp + string(" ") + msg;

   // zog the message based on the destination

   // having to test if the Zonsole is available, sort of ugly
   // maybe if the Zonsole is not available, just cache
   // the log messages and then when it's ready, dump them
   // all in.
   if ((dest & ZOG_CONSOLE) && Z)
   {
      switch (l)
      {
      case ZOG_DEBUG: Z->debug(fullmsg); break;
      case ZOG_INFO: break;
      case ZOG_WARN: break;
      case ZOG_ERROR: break;
      default: break;
      }
   }

   // this is ugly. maybe have STDERR trump STDOUT if it is set?
   if (dest & ZOG_STDOUT) cout << fullmsg << endl;
   if (dest & ZOG_STDERR) cerr << fullmsg << endl;
   
   if (dest & ZOG_FILE)
   {
      // should file already be opened and awaiting data?
      // if so, the stream should be a memer, and we can simply
      // write to it here.
   }
}