#include "sdlapp.h"
#include "zot.h"
#include <sstream>
#include <iomanip>
#include <SDL_opengl.h>

using namespace std;
using namespace Zot;

//class Tesystem : public Zystem
//{
//public:
//   Tesystem()
//   {
//   }
//};

class Zemo : public SDLApp
{
public:
protected:
   virtual void initGl();
   virtual void draw();
};

void Zemo::initGl()
{
   SDLApp::initGl();
   // load w/e
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Zemo::draw()
{
   SDLApp::draw();
}

int main(int argc, char *argv[])
{
   Zemo app;
   app.run();
   return 0;
}
