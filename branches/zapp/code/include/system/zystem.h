#pragma once

#include "zmsg.h"

namespace Zot
{

class Zystem
{

public:

   virtual ~Zystem();

   static Zystem *get() { return m_pSystem; }

   virtual bool config(ZmCfg *pCfg);
   virtual bool init();
   virtual int onExit();
   virtual void run();
   virtual bool saveConfig(ZmCfg *pCfg = NULL);

protected:

   Zystem();

   static Zystem *create();
   static Zystem *m_pSystem;

};

}
