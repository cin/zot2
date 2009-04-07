#include "stdafx.h"
#include <sstream>
#include "convar.h"

using namespace std;
using namespace Zot;

ConVar::ConVar()
   : m_flags(0)
   , m_changePfn(NULL)
   , m_pfn(NULL)
{
}

ConVar::ConVar(
   const string &name,
   const string &value,
   int flags,
   const string &description,
   ConVar::ChangePfn pfn)
   : m_name(name)
   , m_value(value)
   , m_defaultValue(value)
   , m_flags(flags)
   , m_description(description)
   , m_changePfn(pfn)
   , m_pfn(NULL)
{
}

ConVar::~ConVar()
{
}

float ConVar::getFloat() const
{
   istringstream iss(m_value);
   float ret = 0.0f;
   iss >> ret;
   return ret;
}

int32 ConVar::getInt32() const
{
   istringstream iss(m_value);
   int32 ret = 0;
   iss >> ret;
   return ret;
}

uint32 ConVar::getUint32() const
{
   istringstream iss(m_value);
   uint32 ret = 0;
   iss >> ret;
   return ret;
}

void ConVar::setFloat(float value)
{
   ostringstream oss;
   oss << value;
   setString(oss.str());
}

void ConVar::setInt32(int32 value)
{
   ostringstream oss;
   oss << value;
   setString(oss.str());
}

void ConVar::setString(const string &value)
{
   // not really sure if this is important
   // may just want to invoke the cb handler in some cases
   //if (value == m_value)
   //   return;
   string oldValue(m_value);
   m_value = value;
   if (m_changePfn)
      m_changePfn(this, oldValue);
}

void ConVar::setUint32(uint32 value)
{
   ostringstream oss;
   oss << value;
   setString(oss.str());
}
