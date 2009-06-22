#pragma once

#include "zmsgq.h"
#include "zthread.h"

namespace Zot
{

class Zystem : public Zthread
{

public:

   virtual ~Zystem();

   static Zystem *get() { return m_pSystem; }

   virtual bool config(ZmCfg *pCfg);
   virtual bool init();
   virtual int onExit();
   virtual void run();
   virtual void tick();
   virtual bool saveConfig(ZmCfg *pCfg = NULL);

protected:

   Zystem();

   //static Zystem *create();
   static Zystem *m_pSystem;

   bool m_bRun;
   uint32 m_delay;
   Zmsgq m_zq;

};

}
