#include "zot.h"
#include "zmsg.h"

using namespace std;
using namespace Zot;

Zmsg::Zmsg()
   : __m_type(ZM_NONE)
   , __m_timestamp(0.0f)
   , __m_len(Zmsg::s_baseLen)
   , __m_priority(ZOT_PRIORITY_BOTTOM)
   , __m_system(0)
{
}

Zmsg::Zmsg(const Zmsg &other)
   : __m_type(other.__m_type)
   , __m_timestamp(other.__m_timestamp)
   , __m_len(other.__m_len)
   , __m_priority(other.__m_priority)
   , __m_system(other.__m_system)
{
}

Zmsg::~Zmsg()
{
}

Zmsg *Zmsg::copy()
{
   return new Zmsg(*this);
}

ostream &Zmsg::serialize(ostream &os)
{
   os.write((const char *)&__m_type, sizeof(__m_type));
   os.write((const char *)&__m_timestamp, sizeof(__m_timestamp));
   os.write((const char *)&__m_len, sizeof(__m_len));
   os.write((const char *)&__m_priority, sizeof(__m_priority));
   os.write((const char *)&__m_system, sizeof(__m_system));
   return os;
}

istream &Zmsg::deserialize(istream &is)
{
   is.read((char *)&__m_type, sizeof(__m_type));
   is.read((char *)&__m_timestamp, sizeof(__m_timestamp));
   is.read((char *)&__m_len, sizeof(__m_len));
   is.read((char *)&__m_priority, sizeof(__m_priority));
   is.read((char *)&__m_system, sizeof(__m_system));
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
   __m_type = ZM_CFG_MSG;
   __m_priority = ZOT_PRIORITY_TOP;
}

///////////////////////////////////////////////////////////
// ZmStop

ZmStop::ZmStop()
{
   __m_type = ZM_STOP_MSG;
   __m_priority = ZOT_PRIORITY_TOP;
}
