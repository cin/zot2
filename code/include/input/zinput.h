#pragma once

#include "iinput.h"
#include <CEGUIInputEvent.h>

namespace Zot
{

/////////////////////////////////////////////////////////////////////////////
// Zinput
// implementation of IInput for now...should be moved eventually

class Zinput : public IInput
{

public:

   Zinput();

   virtual void init();

   static Zinput *get() { return (Zinput *)m_pInput; }

   bool bind(const char *pKey, const char *handler);

protected:

   virtual int keyboardHandler(InputEvent *pEvent);
   virtual int mouseMotionHandler(InputEvent *pEvent);
   virtual int mouseButtonHandler(InputEvent *pEvent);

   void constructKeyMap();
   void constructMbMap();

   CEGUI::utf32 m_keyMap[SDLK_LAST];
   CEGUI::MouseButton m_mbMap[EMbLast];

protected:

   ConVar *findConVar(const char *pCmd);
   uint16 keyStringToUint16(const char *pKey);

   bool checkKeyBind(const char *pKey, const char *pCmd);
   bool checkMouseButtonBind(const char *pKey, const char *pCmd);
   bool setupKeyBind(uint16 key, const char *pCmd);
   bool setupMouseButtonBind(uint8 mb, const char *pCmd);

};

}
