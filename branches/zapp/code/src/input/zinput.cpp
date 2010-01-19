#include "zot.h"
#include "zapp.h"
#include "zinput.h"
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

void bindHandler()
{
   Zonsole *pZonsole = Zonsole::get();
   if (3 != pZonsole->getArgc())
   {
      pZonsole->warn("bind: invalid format used for bind. `bind key convar`");
      return;
   }

   const char *pKey = pZonsole->getArgv(1);
   const char *pCmd = pZonsole->getArgv(2);
   if (!Zinput::get()->bind(pKey, pCmd))
   {
      ostringstream os;
      os << "bind: invalid key used for bind; " << pKey;
      string msg(os.str());
      pZonsole->warn(msg);
   }
}

ConVar keyBind("bind", bindHandler, "binds a key to a handler");

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

void mouseMotion()
{
}

ConVar InputMouseMotion("mouse_motion", mouseMotion);

Zinput::Zinput()
{
}

bool Zinput::bind(const char *pKey, const char *pCmd)
{
   if (checkKeyBind(pKey, pCmd) ||
      checkMouseButtonBind(pKey, pCmd))
      return true;
   return false;
}

bool Zinput::checkKeyBind(const char *pKey, const char *pCmd)
{
   uint16 key = SDLK_LAST;
   if (1 == strlen(pKey))
   {
      key = (uint16)(*pKey);

      // if key is a capital, make it lowercase
      // TODO: handle other 'uppercase' instances (i.e. ! and 1)
      if (isalpha(key) && isupper(key))
         key = tolower(key);
   }
   else if (0 == strncmp("SDLK_", pKey, 5))
   {
      key = keyStringToUint16(pKey);
   }
   else
   {
      // build SDLK_ string from key and convert to uppercase
      // to ease bind command syntax restrictions
      string tmpKey(pKey);
      transform(tmpKey.begin(), tmpKey.end(), tmpKey.begin(), toupper);

      ostringstream os;
      os << "SDLK_" << tmpKey;
      string sdlKey(os.str());
      key = keyStringToUint16(sdlKey.c_str());
   }

   bool bRet = (key != SDLK_LAST);
   if (bRet)
      bRet = setupKeyBind(key, pCmd);
   return bRet;
}

bool Zinput::checkMouseButtonBind(const char *pKey, const char *pCmd)
{
   string lowerKey(pKey);
   transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(), tolower);

   string mbLower;
   uint8 mb = EMbLast;
   for (int i = 1; i < EMbLast && mb == EMbLast; i++)
   {
      mbLower = MouseButtonStrings[i];
      transform(mbLower.begin(), mbLower.end(), mbLower.begin(), tolower);
      if (lowerKey == mbLower)
         mb = i;
   }

   bool bRet = (mb != EMbLast);
   if (bRet)
      bRet = setupMouseButtonBind(mb, pCmd);
   return bRet;
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

ConVar *Zinput::findConVar(const char *pCmd)
{
   ConVar *pCv = Zonsole::get()->findCommand(pCmd);
   if (!pCv)
   {
      ostringstream os;
      os << "bind: failed because '"
         << pCmd
         << "' does not exist as a ConVar";
      string msg(os.str());
      Zonsole::get()->warn(msg);
   }
   return pCv;
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
   bind("w", "+forward");
   bind("S", "+reverse");
   bind("]", "+forward");
   bind("leftbracket", "+reverse");
   bind("mouseleft", "+forward");
   regMouseMotion(&InputMouseMotion);
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
   if (++m_motionCnt >= MOTION_SZ)
      m_motionCnt = 0;
   m_motion[m_motionCnt] = *pMotion;
   m_motionTimes[m_motionCnt].update();
   if (m_mode < EModeGame)
   {
      CEGUI::System::getSingleton().injectMousePosition(
         (float)pMotion->m_abs.x,
         (float)pMotion->m_abs.y);
   }
   else if (m_pMm)
      (m_pMm->getPfn())();

   return 0;
}

int Zinput::mouseButtonHandler(Zot::InputEvent *pEvent)
{
   MouseButton *pMb = (MouseButton *)pEvent;
   if (m_mode < EModeGame)
   {
      CEGUI::System &S = CEGUI::System::getSingleton();
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
   }
   else
   {
      if (pMb->m_state == EMouseButtonDown)
      {
         MbdmIter it = m_mbdm.find(pMb->m_button);
         if (it != m_mbdm.end())
            (it->second->getPfn())();
      }
      else
      {
         MbumIter it = m_mbum.find(pMb->m_button);
         if (it != m_mbum.end())
            (it->second->getPfn())();
      }
   }
   return 0;
}


bool Zinput::setupKeyBind(uint16 key, const char *pCmd)
{
   // register the binding with the input system
   ConVar *pCv = findConVar(pCmd);
   if (pCv)
      regKeyDown(key, pCv);
   else
      return false;

   // handle special case for +commands,
   // link in their corresponding -command
   if (pCmd[0] == '+')
   {
      string cmd(pCmd);
      cmd[0] = '-';
      ConVar *pCvUp = findConVar(cmd.c_str());
      if (pCvUp)
         regKeyUp(key, pCvUp);
      else
      {
         unregKeyDown(key);

         ostringstream os;
         os << "bind: no corresponding -command exists for "
            << pCmd << ". No bindings have been made.";
         string msg(os.str());
         Zonsole::get()->warn(msg);
         return false;
      }
   }
   return true;
}

bool Zinput::setupMouseButtonBind(uint8 mb, const char *pCmd)
{
   // register the binding with the input system
   ConVar *pCv = findConVar(pCmd);
   if (pCv)
      regMbDown(mb, pCv);
   else
      return false;

   // handle special case for +commands,
   // link in their corresponding -command
   if (pCmd[0] == '+')
   {
      string cmd(pCmd);
      cmd[0] = '-';
      ConVar *pCvUp = findConVar(cmd.c_str());
      if (pCvUp)
         regMbUp(mb, pCvUp);
      else
      {
         unregMbDown(mb);

         ostringstream os;
         os << "bind: no corresponding -command exists for "
            << pCmd << ". No bindings have been made.";
         string msg(os.str());
         Zonsole::get()->warn(msg);
         return false;
      }
   }
   return true;
}

// keep this big arse method at the bottom so as not to intefere
// with the readability of the rest of this class
uint16 Zinput::keyStringToUint16(const char *pKey)
{
   // copied from SDL_keysym.h
   // has to be a better way to do this...but sadly this seems
   // to be the shortest path to a solution
   // remember that this list must change if SDL's key map changes
   // also note that this array is static const so it only gets created
   // the first time this function is called
   //
   // ty regex for saving a whole lotta time here...
   // find = "{SDLK[a-zA-Z0-9_]+>}:b*=:b*{[0-9]+}"
   // replace = "pair<const char *, uint16>("\1", \2)"
   static const pair<const char *, uint16> sSdlKeyStrings[] =
   {
	   pair<const char *, uint16>("SDLK_UNKNOWN", 0),
	   pair<const char *, uint16>("SDLK_FIRST", 0),
	   pair<const char *, uint16>("SDLK_BACKSPACE", 8),
	   pair<const char *, uint16>("SDLK_TAB", 9),
	   pair<const char *, uint16>("SDLK_CLEAR", 12),
	   pair<const char *, uint16>("SDLK_RETURN", 13),
	   pair<const char *, uint16>("SDLK_PAUSE", 19),
	   pair<const char *, uint16>("SDLK_ESCAPE", 27),
	   pair<const char *, uint16>("SDLK_SPACE", 32),
	   pair<const char *, uint16>("SDLK_EXCLAIM", 33),
	   pair<const char *, uint16>("SDLK_QUOTEDBL", 34),
	   pair<const char *, uint16>("SDLK_HASH", 35),
	   pair<const char *, uint16>("SDLK_DOLLAR", 36),
	   pair<const char *, uint16>("SDLK_AMPERSAND", 38),
	   pair<const char *, uint16>("SDLK_QUOTE", 39),
	   pair<const char *, uint16>("SDLK_LEFTPAREN", 40),
	   pair<const char *, uint16>("SDLK_RIGHTPAREN", 41),
	   pair<const char *, uint16>("SDLK_ASTERISK", 42),
	   pair<const char *, uint16>("SDLK_PLUS", 43),
	   pair<const char *, uint16>("SDLK_COMMA", 44),
	   pair<const char *, uint16>("SDLK_MINUS", 45),
	   pair<const char *, uint16>("SDLK_PERIOD", 46),
	   pair<const char *, uint16>("SDLK_SLASH", 47),
	   pair<const char *, uint16>("SDLK_0", 48),
	   pair<const char *, uint16>("SDLK_1", 49),
	   pair<const char *, uint16>("SDLK_2", 50),
	   pair<const char *, uint16>("SDLK_3", 51),
	   pair<const char *, uint16>("SDLK_4", 52),
	   pair<const char *, uint16>("SDLK_5", 53),
	   pair<const char *, uint16>("SDLK_6", 54),
	   pair<const char *, uint16>("SDLK_7", 55),
	   pair<const char *, uint16>("SDLK_8", 56),
	   pair<const char *, uint16>("SDLK_9", 57),
	   pair<const char *, uint16>("SDLK_COLON", 58),
	   pair<const char *, uint16>("SDLK_SEMICOLON", 59),
	   pair<const char *, uint16>("SDLK_LESS", 60),
	   pair<const char *, uint16>("SDLK_EQUALS", 61),
	   pair<const char *, uint16>("SDLK_GREATER", 62),
	   pair<const char *, uint16>("SDLK_QUESTION", 63),
	   pair<const char *, uint16>("SDLK_AT", 64),
	   /* 
	      Skip uppercase letters
	    */
	   pair<const char *, uint16>("SDLK_LEFTBRACKET", 91),
	   pair<const char *, uint16>("SDLK_BACKSLASH", 92),
	   pair<const char *, uint16>("SDLK_RIGHTBRACKET", 93),
	   pair<const char *, uint16>("SDLK_CARET", 94),
	   pair<const char *, uint16>("SDLK_UNDERSCORE", 95),
	   pair<const char *, uint16>("SDLK_BACKQUOTE", 96),
	   pair<const char *, uint16>("SDLK_a", 97),
	   pair<const char *, uint16>("SDLK_b", 98),
	   pair<const char *, uint16>("SDLK_c", 99),
	   pair<const char *, uint16>("SDLK_d", 100),
	   pair<const char *, uint16>("SDLK_e", 101),
	   pair<const char *, uint16>("SDLK_f", 102),
	   pair<const char *, uint16>("SDLK_g", 103),
	   pair<const char *, uint16>("SDLK_h", 104),
	   pair<const char *, uint16>("SDLK_i", 105),
	   pair<const char *, uint16>("SDLK_j", 106),
	   pair<const char *, uint16>("SDLK_k", 107),
	   pair<const char *, uint16>("SDLK_l", 108),
	   pair<const char *, uint16>("SDLK_m", 109),
	   pair<const char *, uint16>("SDLK_n", 110),
	   pair<const char *, uint16>("SDLK_o", 111),
	   pair<const char *, uint16>("SDLK_p", 112),
	   pair<const char *, uint16>("SDLK_q", 113),
	   pair<const char *, uint16>("SDLK_r", 114),
	   pair<const char *, uint16>("SDLK_s", 115),
	   pair<const char *, uint16>("SDLK_t", 116),
	   pair<const char *, uint16>("SDLK_u", 117),
	   pair<const char *, uint16>("SDLK_v", 118),
	   pair<const char *, uint16>("SDLK_w", 119),
	   pair<const char *, uint16>("SDLK_x", 120),
	   pair<const char *, uint16>("SDLK_y", 121),
	   pair<const char *, uint16>("SDLK_z", 122),
	   pair<const char *, uint16>("SDLK_DELETE", 127),
	   /* End of ASCII mapped keysyms */

	   /* International keyboard syms */
	   pair<const char *, uint16>("SDLK_WORLD_0", 160),		/* 0xA0 */
	   pair<const char *, uint16>("SDLK_WORLD_1", 161),
	   pair<const char *, uint16>("SDLK_WORLD_2", 162),
	   pair<const char *, uint16>("SDLK_WORLD_3", 163),
	   pair<const char *, uint16>("SDLK_WORLD_4", 164),
	   pair<const char *, uint16>("SDLK_WORLD_5", 165),
	   pair<const char *, uint16>("SDLK_WORLD_6", 166),
	   pair<const char *, uint16>("SDLK_WORLD_7", 167),
	   pair<const char *, uint16>("SDLK_WORLD_8", 168),
	   pair<const char *, uint16>("SDLK_WORLD_9", 169),
	   pair<const char *, uint16>("SDLK_WORLD_10", 170),
	   pair<const char *, uint16>("SDLK_WORLD_11", 171),
	   pair<const char *, uint16>("SDLK_WORLD_12", 172),
	   pair<const char *, uint16>("SDLK_WORLD_13", 173),
	   pair<const char *, uint16>("SDLK_WORLD_14", 174),
	   pair<const char *, uint16>("SDLK_WORLD_15", 175),
	   pair<const char *, uint16>("SDLK_WORLD_16", 176),
	   pair<const char *, uint16>("SDLK_WORLD_17", 177),
	   pair<const char *, uint16>("SDLK_WORLD_18", 178),
	   pair<const char *, uint16>("SDLK_WORLD_19", 179),
	   pair<const char *, uint16>("SDLK_WORLD_20", 180),
	   pair<const char *, uint16>("SDLK_WORLD_21", 181),
	   pair<const char *, uint16>("SDLK_WORLD_22", 182),
	   pair<const char *, uint16>("SDLK_WORLD_23", 183),
	   pair<const char *, uint16>("SDLK_WORLD_24", 184),
	   pair<const char *, uint16>("SDLK_WORLD_25", 185),
	   pair<const char *, uint16>("SDLK_WORLD_26", 186),
	   pair<const char *, uint16>("SDLK_WORLD_27", 187),
	   pair<const char *, uint16>("SDLK_WORLD_28", 188),
	   pair<const char *, uint16>("SDLK_WORLD_29", 189),
	   pair<const char *, uint16>("SDLK_WORLD_30", 190),
	   pair<const char *, uint16>("SDLK_WORLD_31", 191),
	   pair<const char *, uint16>("SDLK_WORLD_32", 192),
	   pair<const char *, uint16>("SDLK_WORLD_33", 193),
	   pair<const char *, uint16>("SDLK_WORLD_34", 194),
	   pair<const char *, uint16>("SDLK_WORLD_35", 195),
	   pair<const char *, uint16>("SDLK_WORLD_36", 196),
	   pair<const char *, uint16>("SDLK_WORLD_37", 197),
	   pair<const char *, uint16>("SDLK_WORLD_38", 198),
	   pair<const char *, uint16>("SDLK_WORLD_39", 199),
	   pair<const char *, uint16>("SDLK_WORLD_40", 200),
	   pair<const char *, uint16>("SDLK_WORLD_41", 201),
	   pair<const char *, uint16>("SDLK_WORLD_42", 202),
	   pair<const char *, uint16>("SDLK_WORLD_43", 203),
	   pair<const char *, uint16>("SDLK_WORLD_44", 204),
	   pair<const char *, uint16>("SDLK_WORLD_45", 205),
	   pair<const char *, uint16>("SDLK_WORLD_46", 206),
	   pair<const char *, uint16>("SDLK_WORLD_47", 207),
	   pair<const char *, uint16>("SDLK_WORLD_48", 208),
	   pair<const char *, uint16>("SDLK_WORLD_49", 209),
	   pair<const char *, uint16>("SDLK_WORLD_50", 210),
	   pair<const char *, uint16>("SDLK_WORLD_51", 211),
	   pair<const char *, uint16>("SDLK_WORLD_52", 212),
	   pair<const char *, uint16>("SDLK_WORLD_53", 213),
	   pair<const char *, uint16>("SDLK_WORLD_54", 214),
	   pair<const char *, uint16>("SDLK_WORLD_55", 215),
	   pair<const char *, uint16>("SDLK_WORLD_56", 216),
	   pair<const char *, uint16>("SDLK_WORLD_57", 217),
	   pair<const char *, uint16>("SDLK_WORLD_58", 218),
	   pair<const char *, uint16>("SDLK_WORLD_59", 219),
	   pair<const char *, uint16>("SDLK_WORLD_60", 220),
	   pair<const char *, uint16>("SDLK_WORLD_61", 221),
	   pair<const char *, uint16>("SDLK_WORLD_62", 222),
	   pair<const char *, uint16>("SDLK_WORLD_63", 223),
	   pair<const char *, uint16>("SDLK_WORLD_64", 224),
	   pair<const char *, uint16>("SDLK_WORLD_65", 225),
	   pair<const char *, uint16>("SDLK_WORLD_66", 226),
	   pair<const char *, uint16>("SDLK_WORLD_67", 227),
	   pair<const char *, uint16>("SDLK_WORLD_68", 228),
	   pair<const char *, uint16>("SDLK_WORLD_69", 229),
	   pair<const char *, uint16>("SDLK_WORLD_70", 230),
	   pair<const char *, uint16>("SDLK_WORLD_71", 231),
	   pair<const char *, uint16>("SDLK_WORLD_72", 232),
	   pair<const char *, uint16>("SDLK_WORLD_73", 233),
	   pair<const char *, uint16>("SDLK_WORLD_74", 234),
	   pair<const char *, uint16>("SDLK_WORLD_75", 235),
	   pair<const char *, uint16>("SDLK_WORLD_76", 236),
	   pair<const char *, uint16>("SDLK_WORLD_77", 237),
	   pair<const char *, uint16>("SDLK_WORLD_78", 238),
	   pair<const char *, uint16>("SDLK_WORLD_79", 239),
	   pair<const char *, uint16>("SDLK_WORLD_80", 240),
	   pair<const char *, uint16>("SDLK_WORLD_81", 241),
	   pair<const char *, uint16>("SDLK_WORLD_82", 242),
	   pair<const char *, uint16>("SDLK_WORLD_83", 243),
	   pair<const char *, uint16>("SDLK_WORLD_84", 244),
	   pair<const char *, uint16>("SDLK_WORLD_85", 245),
	   pair<const char *, uint16>("SDLK_WORLD_86", 246),
	   pair<const char *, uint16>("SDLK_WORLD_87", 247),
	   pair<const char *, uint16>("SDLK_WORLD_88", 248),
	   pair<const char *, uint16>("SDLK_WORLD_89", 249),
	   pair<const char *, uint16>("SDLK_WORLD_90", 250),
	   pair<const char *, uint16>("SDLK_WORLD_91", 251),
	   pair<const char *, uint16>("SDLK_WORLD_92", 252),
	   pair<const char *, uint16>("SDLK_WORLD_93", 253),
	   pair<const char *, uint16>("SDLK_WORLD_94", 254),
	   pair<const char *, uint16>("SDLK_WORLD_95", 255),		/* 0xFF */

	   /* Numeric keypad */
	   pair<const char *, uint16>("SDLK_KP0", 256),
	   pair<const char *, uint16>("SDLK_KP1", 257),
	   pair<const char *, uint16>("SDLK_KP2", 258),
	   pair<const char *, uint16>("SDLK_KP3", 259),
	   pair<const char *, uint16>("SDLK_KP4", 260),
	   pair<const char *, uint16>("SDLK_KP5", 261),
	   pair<const char *, uint16>("SDLK_KP6", 262),
	   pair<const char *, uint16>("SDLK_KP7", 263),
	   pair<const char *, uint16>("SDLK_KP8", 264),
	   pair<const char *, uint16>("SDLK_KP9", 265),
	   pair<const char *, uint16>("SDLK_KP_PERIOD", 266),
	   pair<const char *, uint16>("SDLK_KP_DIVIDE", 267),
	   pair<const char *, uint16>("SDLK_KP_MULTIPLY", 268),
	   pair<const char *, uint16>("SDLK_KP_MINUS", 269),
	   pair<const char *, uint16>("SDLK_KP_PLUS", 270),
	   pair<const char *, uint16>("SDLK_KP_ENTER", 271),
	   pair<const char *, uint16>("SDLK_KP_EQUALS", 272),

	   /* Arrows + Home/End pad */
	   pair<const char *, uint16>("SDLK_UP", 273),
	   pair<const char *, uint16>("SDLK_DOWN", 274),
	   pair<const char *, uint16>("SDLK_RIGHT", 275),
	   pair<const char *, uint16>("SDLK_LEFT", 276),
	   pair<const char *, uint16>("SDLK_INSERT", 277),
	   pair<const char *, uint16>("SDLK_HOME", 278),
	   pair<const char *, uint16>("SDLK_END", 279),
	   pair<const char *, uint16>("SDLK_PAGEUP", 280),
	   pair<const char *, uint16>("SDLK_PAGEDOWN", 281),

	   /* Function keys */
	   pair<const char *, uint16>("SDLK_F1", 282),
	   pair<const char *, uint16>("SDLK_F2", 283),
	   pair<const char *, uint16>("SDLK_F3", 284),
	   pair<const char *, uint16>("SDLK_F4", 285),
	   pair<const char *, uint16>("SDLK_F5", 286),
	   pair<const char *, uint16>("SDLK_F6", 287),
	   pair<const char *, uint16>("SDLK_F7", 288),
	   pair<const char *, uint16>("SDLK_F8", 289),
	   pair<const char *, uint16>("SDLK_F9", 290),
	   pair<const char *, uint16>("SDLK_F10", 291),
	   pair<const char *, uint16>("SDLK_F11", 292),
	   pair<const char *, uint16>("SDLK_F12", 293),
	   pair<const char *, uint16>("SDLK_F13", 294),
	   pair<const char *, uint16>("SDLK_F14", 295),
	   pair<const char *, uint16>("SDLK_F15", 296),

	   /* Key state modifier keys */
	   pair<const char *, uint16>("SDLK_NUMLOCK", 300),
	   pair<const char *, uint16>("SDLK_CAPSLOCK", 301),
	   pair<const char *, uint16>("SDLK_SCROLLOCK", 302),
	   pair<const char *, uint16>("SDLK_RSHIFT", 303),
	   pair<const char *, uint16>("SDLK_LSHIFT", 304),
	   pair<const char *, uint16>("SDLK_RCTRL", 305),
	   pair<const char *, uint16>("SDLK_LCTRL", 306),
	   pair<const char *, uint16>("SDLK_RALT", 307),
	   pair<const char *, uint16>("SDLK_LALT", 308),
	   pair<const char *, uint16>("SDLK_RMETA", 309),
	   pair<const char *, uint16>("SDLK_LMETA", 310),
	   pair<const char *, uint16>("SDLK_LSUPER", 311),		/* Left "Windows" key */
	   pair<const char *, uint16>("SDLK_RSUPER", 312),		/* Right "Windows" key */
	   pair<const char *, uint16>("SDLK_MODE", 313),		/* "Alt Gr" key */
	   pair<const char *, uint16>("SDLK_COMPOSE", 314),		/* Multi-key compose key */

	   /* Miscellaneous function keys */
	   pair<const char *, uint16>("SDLK_HELP", 315),
	   pair<const char *, uint16>("SDLK_PRINT", 316),
	   pair<const char *, uint16>("SDLK_SYSREQ", 317),
	   pair<const char *, uint16>("SDLK_BREAK", 318),
	   pair<const char *, uint16>("SDLK_MENU", 319),
	   pair<const char *, uint16>("SDLK_POWER", 320),		/* Power Macintosh power key */
	   pair<const char *, uint16>("SDLK_EURO", 321),		/* Some european keyboards */
	   pair<const char *, uint16>("SDLK_UNDO", 322),		/* Atari keyboard has Undo */

	   /* Add any other keys here */

	   //SDLK_LAST
   };

   int sz = sizeof(sSdlKeyStrings) / sizeof(pair<const char *, uint16>);
   int key = SDLK_LAST;
   for (int i = 0; i < sz && key == SDLK_LAST; i++)
   {
      if (0 == strncmp(pKey, sSdlKeyStrings[i].first, strlen(pKey)))
         key = sSdlKeyStrings[i].second;
   }
   return key;
}
