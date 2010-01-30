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
   : pCamera(c)
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
   Vec3f pos(pCamera->getPos()), l(pCamera->getLook());
   l *= -1.0f;
   pos += l;
   pCamera->setPos(pos);
}

void CameraMoveController::forwardUp()
{
}

void CameraMoveController::reverseDown()
{
   Vec3f pos(pCamera->getPos()), l(pCamera->getLook());
   l *= 1.0f;
   pos += l;
   pCamera->setPos(pos);
}

void CameraMoveController::reverseUp()
{
}

void CameraMoveController::leftDown()
{
}

void CameraMoveController::leftUp()
{
   Vec3f pos(pCamera->getPos()), l(pCamera->getRight());
   l *= -1.0f;
   pos += l;
   pCamera->setPos(pos);
}

void CameraMoveController::rightDown()
{
   Vec3f pos(pCamera->getPos()), l(pCamera->getRight());
   l *= 1.0f;
   pos += l;
   pCamera->setPos(pos);
}

void CameraMoveController::rightUp()
{
}

///////////////////////////////////////////////////////////////////////////////
// Camera

Camera::Camera()
   : _near(1.0f)
   , _far(1000.0f)
   , fov(45.0f)
   , aspect(1024.0f / 768.0f)
   , pController(NULL)
{
   pos.init(0, 0, 20);
}

void Camera::place()
{
   ori.glMatrix(glmat, -pos);
   glLoadMatrixf(glmat.m);
}

void Camera::setup(int width, int height, float fov)
{
   aspect = float(width) / height;
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(fov, aspect, _near, _far);
   glMatrixMode(GL_MODELVIEW);
   pController = new CameraMoveController(this);
}

void Camera::update() const
{
   if (pController)
      pController->update();
}

const Vec3f &Camera::getLook() const
{
   static Vec3f l;
   return ori.getLook(l);
}

const Vec3f &Camera::getRight() const
{
   static Vec3f r;
   return ori.getRight(r);
}

const Vec3f &Camera::getUp() const
{
   static Vec3f u;
   return ori.getUp(u);
}

const Vec3f &Camera::getPos() const
{
   return pos;
}

void Camera::setPos(const Vec3f &_pos)
{
   pos = _pos;
}
