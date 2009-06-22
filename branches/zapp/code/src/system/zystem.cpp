#include "zot.h"
#include "zystem.h"

using namespace Zot;

Zystem::Zystem()
{
}

Zystem::~Zystem()
{
}

bool Zystem::config(ZmCfg *pCfg)
{
   return false;
}

bool Zystem::init()
{
   return false;
}

int Zystem::onExit()
{
   return 0;
}

void Zystem::run()
{
}

bool Zystem::saveConfig(Zot::ZmCfg *pCfg)
{
   return false;
}
