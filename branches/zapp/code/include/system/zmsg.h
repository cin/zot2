//#include "zerialize.h"
#include <iostream>

#include "zypes.h"

namespace Zot
{

// msg type declarations
const uint32 ZM_ALL           = 0xFFFF0000;
const uint32 ZM_NONE          = 0x00000000;

const uint32 ZM_CFG_GRP       = 0x00010000;
const uint32 ZM_CFG_MSG       = 0x00010001;

class Zmsg// : public Zerialize
{

public:

   Zmsg();
   Zmsg(const Zmsg &other);
   virtual ~Zmsg();

   virtual std::ostream &serialize(std::ostream &os);
   virtual std::istream &deserialize(std::istream &is);

   virtual std::ostream &jsonSerialize(std::ostream &os);
   virtual std::istream &jsonDeserialize(std::istream &is);

protected:

   uint32 m_type;
   float m_timestamp;
   uint16 m_len;

   static const uint16 s_baseLen = sizeof(uint32) + sizeof(float) + sizeof(uint16);

};

class ZmCfg : public Zmsg
{
   ZmCfg();
};

}
