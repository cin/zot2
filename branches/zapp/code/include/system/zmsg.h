#pragma once

//#include "zerialize.h"
#include <iostream>

#include "zypes.h"

namespace Zot
{

// msg type declarations
const uint32 ZM_ALL           = 0xFFFF0000;
const uint32 ZM_NONE          = 0x00000000;

const uint32 ZM_CFG_GRP       = 0x00010000;
const uint32 ZM_CFG_MSG       = ZM_CFG_GRP | 0x0001;
const uint32 ZM_STOP_MSG      = ZM_CFG_GRP | 0x0002;

class Zystem;

class Zmsg// : public Zerialize
{

public:

   static const uint8 ZOT_PRIORITY_TOP = 0;
   static const uint8 ZOT_PRIORITY_BOTTOM = 10;

   class ZmsgComparison
   {

   public:

      ZmsgComparison(const bool &bRev = false)
         : bReverse(bRev)
      {
      }

      bool operator()(const Zmsg *lhs, const Zmsg *rhs) const
      {
         if (bReverse)
            return lhs->__m_priority > rhs->__m_priority;
         else
            return lhs->__m_priority < rhs->__m_priority;
      }

   private:

      bool bReverse;

   };

   Zmsg();
   Zmsg(uint32 _type, Zystem *sys = NULL);
   Zmsg(uint32 _type, float _timestamp, Zystem *sys = NULL);
   Zmsg(uint32 _type, float _timestamp, uint8 _priority, Zystem *sys = NULL);
   Zmsg(const Zmsg &other);
   virtual ~Zmsg();

   virtual Zmsg *copy();

   uint8 getSystem() const { return __m_system; }
   void setSystem(uint8 _sys) { __m_system = _sys; }
   void setSystem(Zystem *sys);

   virtual std::ostream &serialize(std::ostream &os);
   virtual std::istream &deserialize(std::istream &is);

   virtual std::ostream &jsonSerialize(std::ostream &os);
   virtual std::istream &jsonDeserialize(std::istream &is);

protected:

   uint32 __m_type;
   float __m_timestamp;
   uint16 __m_len;
   uint8 __m_priority;
   uint8 __m_system;

   static const uint16 s_baseLen = sizeof(uint32) + sizeof(float) + sizeof(uint32);

   friend class Zystem;

};

class ZmCfg : public Zmsg
{

public:

   ZmCfg();
   virtual Zmsg *copy();

   uint32 m_mask;

protected:

   virtual std::ostream &serialize(std::ostream &os);
   virtual std::istream &deserialize(std::istream &is);

};

class ZmStop : public Zmsg
{

public:

   ZmStop();
   virtual Zmsg *copy();

};

}
