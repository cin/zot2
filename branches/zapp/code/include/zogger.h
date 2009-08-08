#pragma once

#include <string>
#include <fstream>

#include "zystem.h"

namespace Zot
{

class Zogger :public Zystem
{
public:
   enum ZogLevel
   {
      ZOG_DEBUG,
      ZOG_INFO,
      ZOG_WARN,
      ZOG_ERROR,
      ZOG_FATAL
   };

   enum ZogDest
   {
      ZOG_STDOUT = 0x1,
      ZOG_STDERR = 0x2,
      ZOG_CONSOLE = 0x4,
      ZOG_FILE = 0x8,
      ZOG_DEST_ALL = 0xf
   };

public:
   virtual ~Zogger();

   static Zogger *create();
   static Zogger *get() { return zogger; }

   void zog(const std::string &msg);
   void zog(const std::string &msg, ZogLevel l, int dest);
   virtual int onLog(Zmsg *pMsg);

protected:
   virtual bool init();
   virtual int onExit();
   void zog(ZmLog *pMsg);

private:
   Zogger(const std::string &file);

   static Zogger *zogger;

   std::string    filename;
   std::ofstream  ofs;

};

}
