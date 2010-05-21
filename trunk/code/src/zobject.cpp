#include "zot.h"
#include "zobject.h"

using namespace Zot;

///////////////////////////////////////////////////////////////////////////////
// ZObject

Zobject::Zobject()
   : m_id(0)
{
}

Zobject::Zobject(const Zobject &other)
   : m_id(other.m_id)
{
}

Zobject::~Zobject()
{
}

///////////////////////////////////////////////////////////////////////////////
// WorldObject

WorldObject::WorldObject()
{
}

WorldObject::~WorldObject()
{
}
