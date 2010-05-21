#pragma once

// why include a file when you don't have to
namespace CEGUI
{
   class Window;
   class OpenGLRenderer;
};

namespace Zot
{

class GUI
{
public:

   static void init();
   static void render();
   static CEGUI::Window *root();

protected:

   static CEGUI::Window *m_rootWin;

};

#define WM WindowManager::getSingleton()
#define SYS System::getSingleton()

}
