#pragma once

#include <string>
#include <fstream>

#include "zystem.h"

namespace Zot
{

class Zogger :public Zystem
{
public:
   static const uint8 ZOG_PRIORITY_TOP = 10;
   static const uint8 ZOG_PRIORITY_NORMAL = 5;
   static const uint8 ZOG_PRIORITY_BOTTOM = 0;

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

   static Zogger *create(Zystem *pParent);
   static Zogger *get() { return zogger; }

   void zog(const std::string &msg,
      int level = ZOG_PRIORITY_NORMAL,
      int dest = ZOG_STDOUT | ZOG_FILE);

#ifdef UNICODE
   void zog(const std::wstring &msg,
      int level = ZOG_PRIORITY_NORMAL,
      int dest = ZOG_STDOUT | ZOG_FILE);
#endif

   virtual int onLog(ZmsgPtr pMsg);

protected:
   virtual bool init();
   virtual int onExit();
   void zog(ZmLog *pMsg);

private:
   Zogger(Zystem *pParent, const std::string &file);

   static Zogger *zogger;

   std::string    filename;
   std::ofstream  ofs;

};

}
