#ifndef __CONVAR_H__
#define __CONVAR_H__

#ifdef _WIN32
#pragma once
#endif

#include "Zot.h"
#include <string>

namespace Zot
{

class ConVar
{

public:

   typedef enum EConVarFlags
   {
      EConVarNone       = 0x01,
      EConVarServer     = 0x02,
      EConVarClient     = 0x04,
      EConVarReplicated = 0x08,
   };

	typedef void (__stdcall *ConVarPfn)();
	typedef void (__stdcall *ChangePfn)();

public:

   ConVar(const std::string &name,
      const std::string &value,
      int flags = EConVarNone,
      const std::string &description = "",
      ChangePfn pfn = NULL);

   virtual ~ConVar();

   int getFlags() const { return m_flags; }
   void setFlags(int flags) { m_flags = flags; }
   void addFlag(int flag) { m_flags |= flag; }
   void toggleFlag(int flag) { m_flags ^= flag; }
   void removeFlag(int flag) { m_flags &= ~flag; }
   void resetFlags() { m_flags = 0; }

   ConVarPfn getPfn() const { return m_pfn; }
   void setPfn(ConVarPfn pfn) { m_pfn = pfn; }

   bool getBool() const { getInt32() ? true : false; }
   int32 getInt32() const;
   uint32 getUint32() const;
   float getFloat() const;
   const std::string &getString() const { return m_value; }

   void setBool(bool value) { setInt32(value ? 1 : 0); }
   void setInt32(int32 value);
   void setUint32(uint32 value);
   void setFloat(float value);
   void setString(const std::string &value);

   // need an assert or something here to let ppl know they're
   // overwriting a callback handler
   void changeHandler(ChangePfn pfn) { m_changePfn = pfn; }

protected:

   int         m_flags;
   std::string m_name;
   std::string m_value;
   std::string m_defaultValue;
   std::string m_description;
   ConVarPfn   m_pfn;
   ChangePfn   m_changePfn;

private:

   ConVar();

};

}

#endif // __CONVAR_H__
