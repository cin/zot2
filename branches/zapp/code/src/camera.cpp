#include "zot.h"
#include "camera.h"
#include "zapp.h"

#include <CEGUIDefaultResourceProvider.h>
#ifdef __CEGUI_0_6_2__
#include <OpenGLGUIRenderer/openglrenderer.h>
#else
#include <OpenGL/CEGUIOpenGLRenderer.h>
#endif

using namespace Zot;

Camera::Camera()
   : _near(1.0f)
   , _far(1000.0f)
   , fov(45.0f)
   , aspect(1024.0f / 768.0f)
{
   pos.init(0, 0, 20);
}

void Camera::place()
{
   float m[16];
   glGetFloatv(GL_MODELVIEW_MATRIX, m);
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
}

const Vec3f &Camera::getLook() const
{
   return sZeros;
}

const Vec3f &Camera::getRight() const
{
   return sZeros;
}

const Vec3f &Camera::getUp() const
{
   return sZeros;
}
