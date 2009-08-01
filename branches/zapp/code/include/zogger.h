#pragma once

#include <string>

namespace Zot
{

class Zogger
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
      ZOG_FILE = 0x8
   };

public:
   virtual ~Zogger();

   static void create(ZogLevel l, int dest, const std::string &file);

   static Zogger *get() { return zogger; }

   void zog(const std::string &msg);
   void zog(const std::string &msg, ZogLevel l, int dest);

private:
   Zogger(ZogLevel l, int dest, const std::string &file);

   static Zogger *zogger;

   ZogLevel    level;
   int         destMask;
   std::string filename;
};

}
