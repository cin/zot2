#include "zot.h"
#include "zapp.h"
#include "iinput.h"
#include "zonsole.h"
#include "zogger.h"
#include "convar.h"

#include "windows.h"
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
   : m_type(EInputEventNone)
   , m_state(EInputNone)
{
}

InputEvent::InputEvent(uint8 type)
   : m_type(type)
   , m_state(EInputNone)
{
}

InputEvent::InputEvent(uint8 type, uint8 state)
   : m_type(type)
   , m_state(state)
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
   : InputEvent(EInputKeyboard, EInputNone)
   , m_mod(EKmodNone)
   , m_key(SDLK_UNKNOWN)
   , m_unicode(0)
{
}

Keyboard::Keyboard(uint8 state, uint8 mod, uint16 key, uint16 unicode)
   : InputEvent(EInputKeyboard, state)
   , m_mod(mod)
   , m_key(key)
   , m_unicode(unicode)
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
   : InputEvent(EInputMouseMotion, EInputNone)
{
}

MouseMotion::MouseMotion(Pnt2ui abs, Pnt2i rel)
   : InputEvent(EInputMouseMotion, EMouseMotion)
   , m_abs(abs)
   , m_rel(rel)
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
   : InputEvent(EInputMouseButton, EInputNone)
   , m_button(EMbNone)
{
}

MouseButton::MouseButton(uint8 state, uint8 button, Pnt2ui abs)
   : InputEvent(EInputMouseButton, state)
   , m_button(button)
   , m_abs(abs)
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

void IInput::regKeyDown(uint16 key, ConVar *pCv)
{
   _ASSERT(pCv);
   m_kdm[key] = pCv;
}

void IInput::unregKeyDown(uint16 key)
{
   KdmIter it = m_kdm.find(key);
   if (it != m_kdm.end())
      m_kdm.erase(it);
}

void IInput::regKeyUp(uint16 key, ConVar *pCv)
{
   _ASSERT(pCv);
   m_kum[key] = pCv;
}

void IInput::unregKeyUp(uint16 key)
{
   KumIter it = m_kum.find(key);
   if (it != m_kum.end())
      m_kum.erase(it);
}

void IInput::regMbDown(uint8 mb, ConVar *pCv)
{
   _ASSERT(pCv);
   m_mbdm[mb] = pCv;
}

void IInput::unregMbDown(uint8 mb)
{
   MbdmIter it = m_mbdm.find(mb);
   if (it != m_mbdm.end())
      m_mbdm.erase(it);
}

void IInput::regMbUp(uint8 mb, ConVar *pCv)
{
   _ASSERT(pCv);
   m_mbum[mb] = pCv;
}

void IInput::unregMbUp(uint8 mb)
{
   MbumIter it = m_mbum.find(mb);
   if (it != m_mbum.end())
      m_mbum.erase(it);
}

void IInput::tick(InputEvent *pEvent)
{
   IhIter it = m_handlers.find(pEvent->m_type);
   if (it != m_handlers.end())
      (this->*(it->second))(pEvent);
}

///////////////////////////////////////////////////////////////////////////////
// test mappings

void forwardDown()
{
   OutputDebugString(L"forwardDown\n");
}

void forwardUp()
{
   OutputDebugString(L"forwardUp\n");
}

void reverseDown()
{
   OutputDebugString(L"reverseDown\n");
}

void reverseUp()
{
   OutputDebugString(L"reverseUp\n");
}

ConVar InputForwardDown("+forward", forwardDown);
ConVar InputForwardUp("-forward", forwardUp);

ConVar InputReverseDown("+reverse", reverseDown);
ConVar InputReverseUp("-reverse", reverseUp);

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

   // setup some test mappings
   regKeyDown(SDLK_w, &InputForwardDown);
   regKeyUp(SDLK_w, &InputForwardUp);
   regKeyDown(SDLK_s, &InputReverseDown);
   regKeyUp(SDLK_s, &InputReverseUp);
}

int Zinput::keyboardHandler(InputEvent *pEvent)
{
   CEGUI::System &S = CEGUI::System::getSingleton();

   Keyboard *pKb = (Keyboard *)pEvent;
   if (pKb->m_state == EKeyDown)
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
            m_state[pKb->m_key] = 1;
            KdmIter it = m_kdm.find(pKb->m_key);
            if (it != m_kdm.end())
               (it->second->getPfn())();
         }
         break;
      }
   }
   else // pKb->m_state == EKeyUp
   {
      if (m_mode < EModeGame)
      {
         if (m_keyMap[pKb->m_key] != 0)
            S.injectKeyUp(m_keyMap[pKb->m_key]);
         else
            S.injectKeyUp(pKb->m_key);
      }
      else
      {
         m_state[pKb->m_key] = 0;
         KumIter it = m_kum.find(pKb->m_key);
         if (it != m_kum.end())
            (it->second->getPfn())();
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
   if (pMb->m_state == EMouseButtonDown)
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
