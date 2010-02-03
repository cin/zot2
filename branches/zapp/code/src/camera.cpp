#include "zot.h"
#include "camera.h"
#include "zapp.h"
#include "zinput.h"
#include "convar.h"

#include <CEGUIDefaultResourceProvider.h>
#ifdef __CEGUI_0_6_2__
#include <OpenGLGUIRenderer/openglrenderer.h>
#else
#include <OpenGL/CEGUIOpenGLRenderer.h>
#endif

using namespace Zot;

void forwardDown()
{
   Zapp::get()->getCamera().getController()->forwardDown();
}

void forwardUp()
{
   Zapp::get()->getCamera().getController()->forwardUp();
}

void reverseDown()
{
   Zapp::get()->getCamera().getController()->reverseDown();
}

void reverseUp()
{
   Zapp::get()->getCamera().getController()->reverseUp();
}

void leftDown()
{
   Zapp::get()->getCamera().getController()->leftDown();
}

void leftUp()
{
   Zapp::get()->getCamera().getController()->leftUp();
}

void rightDown()
{
   Zapp::get()->getCamera().getController()->rightDown();
}

void rightUp()
{
   Zapp::get()->getCamera().getController()->rightUp();
}

void mouseMotion()
{
}

ConVar InputMouseMotion("mouse_motion", mouseMotion);
ConVar InputForwardDown("+forward", forwardDown);
ConVar InputForwardUp("-forward", forwardUp);
ConVar InputReverseDown("+reverse", reverseDown);
ConVar InputReverseUp("-reverse", reverseUp);
ConVar InputLeftDown("+left", leftDown);
ConVar InputLeftUp("-left", leftUp);
ConVar InputRightDown("+right", rightDown);
ConVar InputRightUp("-right", rightUp);

///////////////////////////////////////////////////////////////////////////////
// CameraMoveController

CameraMoveController::CameraMoveController(Camera *c)
   : m_pCamera(c)
{
   Zinput *pInput = Zinput::get();
   pInput->bind("w", "+forward");
   pInput->bind("S", "+reverse");
   pInput->bind("a", "+left");
   pInput->bind("d", "+right");
   pInput->regMouseMotion(&InputMouseMotion);
}

void CameraMoveController::update() const
{
   // handler for input controls
}

void CameraMoveController::forwardDown()
{
   Vec3f pos(m_pCamera->getPos()), l(m_pCamera->getLook());
   l *= -1.0f;
   pos += l;
   m_pCamera->setPos(pos);
}

void CameraMoveController::forwardUp()
{
}

void CameraMoveController::reverseDown()
{
   Vec3f pos(m_pCamera->getPos()), l(m_pCamera->getLook());
   l *= 1.0f;
   pos += l;
   m_pCamera->setPos(pos);
}

void CameraMoveController::reverseUp()
{
}

void CameraMoveController::leftDown()
{
   Vec3f pos(m_pCamera->getPos()), l(m_pCamera->getRight());
   l *= -1.0f;
   pos += l;
   m_pCamera->setPos(pos);
}

void CameraMoveController::leftUp()
{
}

void CameraMoveController::rightDown()
{
   Vec3f pos(m_pCamera->getPos()), l(m_pCamera->getRight());
   l *= 1.0f;
   pos += l;
   m_pCamera->setPos(pos);
}

void CameraMoveController::rightUp()
{
}

///////////////////////////////////////////////////////////////////////////////
// Camera

Camera::Camera()
   : m_near(1.0f)
   , m_far(1000.0f)
   , m_fov(45.0f)
   , m_aspect(1024.0f / 768.0f)
   , m_pController(NULL)
{
   m_pos.init(0, 0, 20);
}

Camera::~Camera()
{
   if (m_pController)
   {
      delete m_pController;
      m_pController = NULL;
   }
}

void Camera::place()
{
   m_ori.glMatrix(m_glmat, -m_pos);
   glLoadMatrixf(m_glmat.m);
}

void Camera::setup(int width, int height, float fov)
{
   m_aspect = float(width) / height;
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(m_fov, m_aspect, m_near, m_far);
   glMatrixMode(GL_MODELVIEW);

   if (m_pController)
   {
      delete m_pController;
      m_pController = NULL;
   }
   m_pController = new CameraMoveController(this);
}

void Camera::update() const
{
   if (m_pController)
      m_pController->update();
}

const Vec3f &Camera::getLook() const
{
   static Vec3f l;
   return m_ori.getLook(l);
}

const Vec3f &Camera::getRight() const
{
   static Vec3f r;
   return m_ori.getRight(r);
}

const Vec3f &Camera::getUp() const
{
   static Vec3f u;
   return m_ori.getUp(u);
}

const Vec3f &Camera::getPos() const
{
   return m_pos;
}

void Camera::setPos(const Vec3f &pos)
{
   m_pos = pos;
}
