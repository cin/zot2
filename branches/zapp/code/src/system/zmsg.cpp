#include "zot.h"
#include "zmsg.h"

using namespace std;
using namespace Zot;

Zmsg::Zmsg()
   : m_type(ZM_NONE)
   , m_timestamp(0.0f)
   , m_len(Zmsg::s_baseLen)
{
}

Zmsg::Zmsg(const Zmsg &other)
   : m_type(other.m_type)
   , m_timestamp(other.m_timestamp)
   , m_len(other.m_len)
{
}

Zmsg::~Zmsg()
{
}

ostream &Zmsg::serialize(ostream &os)
{
   os.write((const char *)&m_type, sizeof(m_type));
   os.write((const char *)&m_timestamp, sizeof(m_timestamp));
   os.write((const char *)&m_len, sizeof(m_len));
   return os;
}

istream &Zmsg::deserialize(istream &is)
{
   is.read((char *)&m_type, sizeof(m_type));
   is.read((char *)&m_timestamp, sizeof(m_timestamp));
   is.read((char *)&m_len, sizeof(m_len));
   return is;
}

ostream &Zmsg::jsonSerialize(ostream &os)
{
   return os;
}

istream &Zmsg::jsonDeserialize(istream &is)
{
   return is;
}

///////////////////////////////////////////////////////////
// ZmCfg

ZmCfg::ZmCfg()
{
   m_type = ZM_CFG_MSG;
}
