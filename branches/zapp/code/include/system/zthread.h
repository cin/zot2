#pragma once

struct SDL_Thread;

namespace Zot
{

class Zthread
{

public:

   Zthread();
   virtual ~Zthread();

   virtual bool create();
   virtual void kill();

protected:

   SDL_Thread *m_pThread;

private:
   static int sdlCallback(void *);

};

}
