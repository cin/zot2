#include "sdlapp.h"
#include "zot.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <sstream>
#include <iomanip>

#ifdef __APPLE__
// built SDLMain.m manually bc not sure exactly how to do that in here
// i guess a prebuild step would work but dunno...just testing for now
// anyway...moved libSDLmain.a to lib folder after running the commands
// listed below
//
// gcc -I/Library/Frameworks/SDL.framework/Headers -arch i386 -c SDLMain.m
// ar rcs libSDLmain.a SDLMain.o

//#include "SDLMain.h"
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

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
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
