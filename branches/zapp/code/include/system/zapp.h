#pragma once

#include <vector>
#include "zystem.h"
#include "terrain.h"
#include "camera.h"

namespace Zot
{

class Zapp : public Zystem
{

public:

   typedef std::vector<Zystem *> Zystems;
   typedef Zystems::iterator ZysIter;

public:

   Zapp();
   virtual ~Zapp();

   static Zapp *get() { return m_pApp; }

   virtual bool config(ZmCfg *pCfg);
   virtual bool init(); // overridden to not create da thread
   virtual int onExit();
   virtual void tick();

   void addZystress();

protected:

   Zystems m_zystems;
   Terrain m_terrain;
   Camera m_camera;

   static Zapp *m_pApp;

};

}
