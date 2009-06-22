#pragma once

struct SDL_Event;

namespace Zot
{

/////////////////////////////////////////////////////////////////////
// Name: Zevent
// Purpose: basic event handling interface
/////////////////////////////////////////////////////////////////////

class Zevent
{

public:

   Zevent();
   virtual ~Zevent();

protected:

   SDL_Event *m_pEvent;

};

}
