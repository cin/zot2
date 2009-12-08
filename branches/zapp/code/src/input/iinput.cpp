#include "zot.h"
#include "zapp.h"
#include "iinput.h"
#include "zonsole.h"
#include "zogger.h"

#include <SDL.h>

#include <CEGUIDefaultResourceProvider.h>
#ifdef __CEGUI_0_6_2__
#include <OpenGLGUIRenderer/openglrenderer.h>
#else
#include <OpenGL/CEGUIOpenGLRenderer.h>
#endif

using namespace std;
using namespace Zot;

IInput::InputState IInput::ALL_OFF(sInputBitCount, 0ul);

// NOTE: only sets the lower 32 bits
IInput::InputState IInput::ALL_ON(sInputBitCount, 0xfffffffful);

IInput *IInput::m_pInput = NULL;

/////////////////////////////////////////////////////////////////////////////
// InputEvent

InputEvent::InputEvent()
   : m_type(0)
   , m_state(0)
{
}

InputEvent::InputEvent(const InputEvent &other)
   : m_type(other.m_type)
   , m_state(other.m_state)
{
}

/////////////////////////////////////////////////////////////////////////////
// Keyboard

Keyboard::Keyboard()
   : m_mod(EKmodNone)
   , m_key(SDLK_UNKNOWN)
   , m_unicode(0)
{
}

Keyboard::Keyboard(const Keyboard &other)
   : InputEvent(other)
   , m_mod(other.m_mod)
   , m_key(other.m_key)
   , m_unicode(other.m_unicode)
{
}

/////////////////////////////////////////////////////////////////////////////
// MouseMotion

MouseMotion::MouseMotion()
{
}

MouseMotion::MouseMotion(const MouseMotion &other)
   : InputEvent(other)
   , m_abs(other.m_abs)
   , m_rel(other.m_rel)
{
}

/////////////////////////////////////////////////////////////////////////////
// MouseButton

MouseButton::MouseButton()
   : m_button(0)
{
}

MouseButton::MouseButton(const MouseButton &other)
   : InputEvent(other)
   , m_button(other.m_button)
   , m_abs(other.m_abs)
{
}

/////////////////////////////////////////////////////////////////////////////
// IInput

IInput::IInput()
   : m_state(IInput::ALL_OFF)
   , m_mode(EModeUnknown)
{
   // see note above, temporary (maybe permanent) solution to that issue
   IInput::ALL_ON.set();
}

IInput::~IInput()
{
}

IInput *IInput::get()
{
   if (!m_pInput)
      create();
   return m_pInput;
}

void IInput::init()
{
   m_handlers[EInputKeyboard] = &IInput::keyboardHandler;
   m_handlers[EInputMouseMotion] = &IInput::mouseMotionHandler;
   m_handlers[EInputMouseButton] = &IInput::mouseButtonHandler;
}

int IInput::keyboardHandler(InputEvent *pEvent)
{
   return 0;
}

int IInput::mouseMotionHandler(InputEvent *pEvent)
{
   return 0;
}

int IInput::mouseButtonHandler(InputEvent *pEvent)
{
   return 0;
}

void IInput::tick(InputEvent *pEvent)
{
   IhIter it = m_handlers.find(pEvent->m_type);
   if (it != m_handlers.end())
      (this->*(it->second))(pEvent);
}

/////////////////////////////////////////////////////////////////////////////
// Zinput
// implementation of IInput for now...should be moved eventually

class Zinput : public IInput
{

public:

   Zinput();

   virtual void init();

protected:

   virtual int keyboardHandler(InputEvent *pEvent);
   virtual int mouseMotionHandler(InputEvent *pEvent);
   virtual int mouseButtonHandler(InputEvent *pEvent);

   void constructKeyMap();
   void constructMbMap();

   CEGUI::utf32 m_keyMap[SDLK_LAST];
   CEGUI::MouseButton m_mbMap[CEGUI::MouseButtonCount];

};

Zinput::Zinput()
{
}

void Zinput::constructKeyMap()
{
   memset(m_keyMap, 0, sizeof(m_keyMap));

   m_keyMap[SDLK_RSHIFT] = CEGUI::Key::RightShift;
   m_keyMap[SDLK_LSHIFT] = CEGUI::Key::LeftShift;
   m_keyMap[SDLK_RCTRL] = CEGUI::Key::RightControl;
   m_keyMap[SDLK_LCTRL] = CEGUI::Key::LeftControl;
   m_keyMap[SDLK_RALT] = CEGUI::Key::RightAlt;
   m_keyMap[SDLK_LALT] = CEGUI::Key::LeftAlt;
   m_keyMap[SDLK_LSUPER] = CEGUI::Key::LeftWindows;
   m_keyMap[SDLK_RSUPER] = CEGUI::Key::RightWindows;
   m_keyMap[SDLK_UP] = CEGUI::Key::ArrowUp;
   m_keyMap[SDLK_DOWN] = CEGUI::Key::ArrowDown;
   m_keyMap[SDLK_RIGHT] = CEGUI::Key::ArrowRight;
   m_keyMap[SDLK_LEFT] = CEGUI::Key::ArrowLeft;
   m_keyMap[SDLK_INSERT] = CEGUI::Key::Insert;
   m_keyMap[SDLK_HOME] = CEGUI::Key::Home;
   m_keyMap[SDLK_END] = CEGUI::Key::End;
   m_keyMap[SDLK_PAGEUP] = CEGUI::Key::PageUp;
   m_keyMap[SDLK_PAGEDOWN] = CEGUI::Key::PageDown;
   m_keyMap[SDLK_DELETE] = CEGUI::Key::Delete;
   m_keyMap[SDLK_INSERT] = CEGUI::Key::Insert;
   m_keyMap[SDLK_TAB] = CEGUI::Key::Tab;
   m_keyMap[SDLK_RETURN] = CEGUI::Key::Return;
   m_keyMap[SDLK_KP_ENTER] = CEGUI::Key::NumpadEnter;
   m_keyMap[SDLK_BACKSPACE] = CEGUI::Key::Backspace;
}

void Zinput::constructMbMap()
{
   memset(m_mbMap, 0, sizeof(m_mbMap));

   m_mbMap[EMbLeft] = CEGUI::LeftButton;
   m_mbMap[EMbRight] = CEGUI::RightButton;
   m_mbMap[EMbMiddle] = CEGUI::MiddleButton;
   m_mbMap[EMbX1] = CEGUI::X1Button;
   m_mbMap[EMbX2] = CEGUI::X2Button;
}

IInput *IInput::create()
{
   m_pInput = new Zinput();
   return m_pInput;
}

void Zinput::init()
{
   IInput::init();

   SDL_ShowCursor(0);
   SDL_EnableUNICODE(1);
   SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

   constructKeyMap();
   constructMbMap();

   // set the mode here for now...eventually will go to menu or w/e
   m_mode = EModeGame;
}

int Zinput::keyboardHandler(InputEvent *pEvent)
{
   CEGUI::System &S = CEGUI::System::getSingleton();

   Keyboard *pKb = (Keyboard *)pEvent;
   if (pKb->m_type == EKeyDown)
   {
      switch (pKb->m_key)
      {
      case SDLK_ESCAPE:
         if (m_mode == EModeGame)
            Zapp::get()->onExit();
         else if (m_mode & EModeConsole)
         {
            Zonsole::get()->hide();
            m_mode = EModeGame;
         }
         break;
      case SDLK_BACKQUOTE:
         if (m_mode == EModeGame)
         {
            Zonsole::get()->show();
            m_mode = EModeConsole;
         }
         else if (m_mode & EModeConsole)
         {
            Zonsole::get()->hide();
            m_mode = EModeGame;
         }
         break;
      default:
         if (m_mode < EModeGame)
         {
            if (m_keyMap[pKb->m_key] != 0)
            {
               if (m_mode & EModeConsole && pKb->m_key == SDLK_TAB)
                  Zonsole::get()->handleTab();
               else
                  S.injectKeyDown(m_keyMap[pKb->m_key]);
            }
            else
               S.injectKeyDown(pKb->m_key);

            if (pKb->m_unicode != 0 && pKb->m_key != SDLK_RETURN)
               S.injectChar(static_cast<CEGUI::utf32>(pKb->m_unicode));
         }
         else
         {
            // TODO: insert game handlers here!!!
         }
         break;
      }
   }
   else // pKb->m_type == EKeyUp
   {
      if (m_mode < EModeGame)
      {
         if (m_keyMap[pKb->m_key] != 0)
            S.injectKeyUp(m_keyMap[pKb->m_key]);
         else
            S.injectKeyUp(pKb->m_key);
      }
   }
   return 0;
}

int Zinput::mouseMotionHandler(Zot::InputEvent *pEvent)
{
   MouseMotion *pMotion = (MouseMotion *)pEvent;
   CEGUI::System::getSingleton().injectMousePosition(
      (float)pMotion->m_abs.x,
      (float)pMotion->m_abs.y);
   return 0;
}

int Zinput::mouseButtonHandler(Zot::InputEvent *pEvent)
{
   CEGUI::System &S = CEGUI::System::getSingleton();

   MouseButton *pMb = (MouseButton *)pEvent;
   if (pMb->m_type == EMouseButtonDown)
   {
      if (pMb->m_button > EMbNone && pMb->m_button < EMbLast)
         S.injectMouseButtonDown(m_mbMap[pMb->m_button]);
   }
   else
   {
      if (pMb->m_button > EMbNone && pMb->m_button < EMbLast)
         S.injectMouseButtonUp(m_mbMap[pMb->m_button]);
   }
   return 0;
}
