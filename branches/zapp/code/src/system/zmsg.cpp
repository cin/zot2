#include "zot.h"
#include "zmsg.h"
#include "zystem.h"
#include "zogger.h"

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

Zmsg::Zmsg(uint32 _type, Zystem *sys)
   : __m_type(_type)
   , __m_timestamp(0.0f)
   , __m_len(Zmsg::s_baseLen)
   , __m_priority(ZOT_PRIORITY_BOTTOM)
   , __m_system(0)
{
   setSystem(sys);
}

Zmsg::Zmsg(uint32 _type, float _timestamp, Zystem *sys)
   : __m_type(_type)
   , __m_timestamp(_timestamp)
   , __m_len(Zmsg::s_baseLen)
   , __m_priority(ZOT_PRIORITY_BOTTOM)
   , __m_system(0)
{
   setSystem(sys);
}

Zmsg::Zmsg(uint32 _type, float _timestamp, uint8 _priority, Zystem *sys)
   : __m_type(_type)
   , __m_timestamp(_timestamp)
   , __m_len(Zmsg::s_baseLen)
   , __m_priority(ZOT_PRIORITY_BOTTOM)
   , __m_system(0)
{
   setSystem(sys);
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

void Zmsg::setSystem(Zystem *sys)
{
   if (sys)
      __m_system = sys->getid();
}

ostream &Zmsg::serialize(ostream &os) const
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

ostream &Zmsg::jsonSerialize(ostream &os) const
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
   : m_mask(0)
{
   __m_type = ZM_CFG_MSG;
   __m_priority = ZOT_PRIORITY_TOP;
}

ZmCfg::ZmCfg(const ZmCfg &other)
   : m_mask(other.m_mask)
{
   __m_type = ZM_CFG_MSG;
   __m_priority = ZOT_PRIORITY_TOP;
}

Zmsg *ZmCfg::copy()
{
   return new ZmCfg(*this);
}

std::ostream &ZmCfg::serialize(std::ostream &os) const
{
   Zmsg::serialize(os);
   os.write((const char *)&m_mask, sizeof(m_mask));
   return os;
}

std::istream &ZmCfg::deserialize(std::istream &is)
{
   Zmsg::deserialize(is);
   is.read((char *)&m_mask, sizeof(m_mask));
   return is;
}

///////////////////////////////////////////////////////////
// ZmStop

ZmStop::ZmStop()
{
   __m_type = ZM_STOP_MSG;
   __m_priority = ZOT_PRIORITY_TOP;
}

Zmsg *ZmStop::copy()
{
   return new ZmStop(*this);
}

///////////////////////////////////////////////////////////
// ZmLog

ZmLog::ZmLog()
   : m_level(ZOT_PRIORITY_NORMAL)
   , m_dest(Zogger::ZOG_FILE)
{
   __m_type = ZM_LOG_MSG;
   __m_priority = ZOT_PRIORITY_NORMAL;
}

ZmLog::ZmLog(const string &msg)
   : m_level(ZOT_PRIORITY_NORMAL)
   , m_dest(Zogger::ZOG_FILE)
   , m_msg(msg)
{
   __m_type = ZM_LOG_MSG;
   __m_priority = ZOT_PRIORITY_NORMAL;
}

ZmLog::ZmLog(const std::string &msg, int level, int dest)
   : m_msg(msg)
{
   m_level = (uint8)level;
   m_dest = (uint8)dest;
   __m_type = ZM_LOG_MSG;
   __m_priority = ZOT_PRIORITY_NORMAL;
}

ZmLog::ZmLog(const ZmLog &other)
   : m_level(other.m_level)
   , m_dest(other.m_dest)
   , m_msg(other.m_msg)
{
   __m_type = ZM_LOG_MSG;
   __m_priority = ZOT_PRIORITY_NORMAL;
}

Zmsg *ZmLog::copy()
{
   return new ZmLog(*this);
}

ostream &ZmLog::serialize(ostream &os) const
{
   Zmsg::serialize(os);
   uint16 sz = (uint16)m_msg.length();
   os.write((const char *)&m_level, sizeof(m_level));
   os.write((const char *)&m_dest, sizeof(m_dest));
   os.write((const char *)&sz, sizeof(sz));
   os.write(m_msg.data(), sz);
   return os;
}

istream &ZmLog::deserialize(istream &is)
{
   Zmsg::deserialize(is);
   uint16 sz = 0;
   is.read((char *)&m_level, sizeof(m_level));
   is.read((char *)&m_dest, sizeof(m_dest));
   is.read((char *)&sz, sizeof(sz));
   m_msg.resize(sz + 1);
   is.read(&m_msg.at(0), sz);
   return is;
}
