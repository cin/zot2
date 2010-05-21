#pragma once

#include "vector.h"
#include "quaternion.h"

namespace Zot
{

///////////////////////////////////////////////////////////////////////////////
// Zobject

class Zobject
{

public:

   Zobject();
   Zobject(const Zobject &other);
   virtual ~Zobject();

   uint32 getid() const { return m_id; }
   void setid(uint32 id) { m_id = id; }

protected:

   uint32 m_id;

};

///////////////////////////////////////////////////////////////////////////////
// WorldObject

class WorldObject : public Zobject
{

public:

   WorldObject();
   virtual ~WorldObject();

   const Vec3f &getPos() const { return m_pos; }
   const Quatf &getOri() const { return m_ori; }

   void setPos(const Vec3f &pos) { m_pos = pos; }
   void setOri(const Quatf &ori) { m_ori = ori; }

protected:

   Vec3f m_pos;
   Quatf m_ori;

};

}
