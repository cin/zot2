#include "sdlapp.h"
#include "zot.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <sstream>
#include <iomanip>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <gl/gl.h>
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
   //D({
   //   wostringstream os;
   //   os << "Zemo::draw" << endl;
   //   OutputDebugString(os.str().c_str());
   //})
}

int main(int argc, char *argv[])
{
   Zemo app;
   app.run();
   return 0;
}
