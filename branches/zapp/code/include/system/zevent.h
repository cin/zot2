#pragma once

namespace Zot
{

/////////////////////////////////////////////////////////////////////
// Name: Zevent
// Purpose: basic event handling interface
/////////////////////////////////////////////////////////////////////

class Zevent
{

public:

   virtual ~Zevent();

protected:

   Zevent();

   int m_hEvent;

};

}
