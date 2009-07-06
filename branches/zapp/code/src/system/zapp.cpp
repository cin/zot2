#include "zot.h"
#include "zapp.h"

using namespace Zot;

Zapp *Zapp::m_pApp = NULL;

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

   // testing stuff for now...until created dynamically by cfg file
   Zystem *pSys = new Zystem(true);
   if (pSys->init())
      m_zystems.push_back(pSys);

   return true;
}

int Zapp::onExit()
{
   int ret = Zystem::onExit();
   return ret;
}
