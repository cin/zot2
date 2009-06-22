#include "zot.h"

#include "zapp.h"

namespace Zot
{

Zapp::Zapp()
{
}

Zapp::~Zapp()
{
}

bool Zapp::config(ZmCfg *pCfg)
{
   return true;
}

bool Zapp::init()
{
   bool ret = Zystem::init();
   if (!ret)
      return ret;
   return true;
}

int Zapp::onExit()
{
   int ret = Zystem::onExit();
   return ret;
}

}