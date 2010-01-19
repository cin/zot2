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
   , m_pMm(NULL)
   , m_motionCnt(-1)
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

void IInput::regMouseMotion(ConVar *pCv)
{
   _ASSERT(pCv);
   m_pMm = pCv;
}

void IInput::unregMouseMotion()
{
   m_pMm = NULL;
}

void IInput::tick(InputEvent *pEvent)
{
   IhIter it = m_handlers.find(pEvent->m_type);
   if (it != m_handlers.end())
      (this->*(it->second))(pEvent);
}
