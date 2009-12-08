#pragma once

#include <boost/dynamic_bitset.hpp>
#include <map>
#include <SDL_keysym.h>
#include "vector.h"

namespace Zot
{

// thin wrapper layer for input events based on SDL_Event
typedef enum EInputEvent
{
   EInputEventNone,
   EInputKeyboard,
   EInputMouseMotion,
   EInputMouseButton,
   EInputJoyAxis,
   EInputJoyBall,
   EInputJoyHat,
   EInputJoyButton,
   EInputEventLast
};

typedef enum EInputType
{
   EInputNone,
   EKeyDown,
   EKeyUp,
   EMouseMotion,
   EMouseButtonDown,
   EMouseButtonUp,
   EJoyAxisMotion,
   EJoyBallMotion,
   EJoyHatMotion,
   EJoyButtonDown,
   EJoyButtonUp,
   EInputLast
};

// mimics SDL_keysym.h
typedef enum EKeyMod
{
   EKmodNone  = 0x0000,
   EKmodLshift= 0x0001,
   EKmodRshift= 0x0002,
   EKmodLctrl = 0x0040,
   EKmodRctrl = 0x0080,
   EKmodLalt  = 0x0100,
   EKmodRalt  = 0x0200,
   EKmodLmeta = 0x0400,
   EKmodRmeta = 0x0800,
   EKmodNum   = 0x1000,
   EKmodCaps  = 0x2000,
   EKmodMode  = 0x4000,
   EKmodReserved = 0x8000
};

#define EKmodCtrl  (EKmodLctrl  | EKmodRctrl)
#define EKmodShift (EKmodLshift | EKmodRshift)
#define EKmodAlt   (EKmodLalt   | EKmodRalt)
#define EKmodMeta  (EKmodLmeta  | EKmodRmeta)

// mimics SDL_mouse.h
typedef enum EMouseButton
{
   EMbNone,
   EMbLeft,
   EMbMiddle,
   EMbRight,
   EMbWheelUp,
   EMbWheelDown,
   EMbX1,
   EMbX2,
   EMbLast
};

/////////////////////////////////////////////////////////////////////////////
// InputEvent

struct InputEvent
{

   uint8 m_type;
   uint8 m_state;

   InputEvent();
   InputEvent(const InputEvent &other);

};

/////////////////////////////////////////////////////////////////////////////
// Keyboard
// to keep things simple, m_key is modelled after SDLKey and
// m_mod after SDLMod

struct Keyboard : InputEvent
{

   uint8 m_mod;
   uint16 m_key;
   uint16 m_unicode;

   Keyboard();
   Keyboard(const Keyboard &other);

};

/////////////////////////////////////////////////////////////////////////////
// MouseMotion

struct MouseMotion : InputEvent
{

   Pnt2ui m_abs;
   Pnt2i m_rel;

   MouseMotion();
   MouseMotion(const MouseMotion &other);

};

/////////////////////////////////////////////////////////////////////////////
// MouseButton
// button here can handle way more buttons than EMouseButton
// as it is not masked, can be any uint8

struct MouseButton : InputEvent
{

   uint8 m_button; 
   Pnt2ui m_abs;

   MouseButton();
   MouseButton(const MouseButton &other);

};

#if 0
/////////////////////////////////////////////////////////////////////////////
// JoyAxis

struct JoyAxis : InputEvent
{
};

/////////////////////////////////////////////////////////////////////////////
// JoyBallMotion

struct JoyBallMotion : InputEvent
{
};

/////////////////////////////////////////////////////////////////////////////
// JoyHatMotion

struct JoyHatMotion : InputEvent
{
};

/////////////////////////////////////////////////////////////////////////////
// JoyButton

struct JoyButton : InputEvent
{
};

// aren't going to need this union b/c the handlers are
// are going to pass in an InputEvent and simply cast it
// to what is desired
//typedef union
//{
//   uint8 m_type;
//   Keyboard m_key;
//   MouseMotion m_mouse;
//   MouseButton m_mb;
//   JoyAxis m_joyAxis;
//   JoyBallMotion m_joyBall;
//   JoyHatMotion m_joyHat;
//   JoyButton m_joyButton;
//} InputEvent;
#endif

typedef enum EInputMode
{
   EModeUnknown = 0x0,
   EModeMenu = 0x01,
   EModeConsole = 0x2,
   EModeGame = 0x4,
   EModeLast = 0x8
};

/////////////////////////////////////////////////////////////////////////////
// IInput

class IInput
{

public:

   IInput();
   virtual ~IInput();

   static IInput *get();

   virtual void init();
   virtual void tick(InputEvent *pEvent);

   EInputMode getMode() const { return m_mode; }
   void setMode(EInputMode mode) { m_mode = mode; }

protected:

   static const int32 sInputBitCount = SDLK_LAST;
   typedef boost::dynamic_bitset<> InputState;

   static InputState ALL_OFF;
   static InputState ALL_ON;

   typedef int (IInput::*InputHandler)(InputEvent *);
   typedef std::map<uint32, InputHandler> InputHandlers;
   typedef InputHandlers::iterator IhIter;

   InputHandlers m_handlers;
   InputState m_state;
   EInputMode m_mode;

   virtual int keyboardHandler(InputEvent *pEvent);
   virtual int mouseMotionHandler(InputEvent *pEvent);
   virtual int mouseButtonHandler(InputEvent *pEvent);

protected:

   static IInput *create();

   static IInput *m_pInput;

};

}
