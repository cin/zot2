#pragma once

#include "zobject.h"
#include "matrix.h"
#include "quaternion.h"

namespace Zot
{

// vertex
// plane intersection methods
class Frustum
{
};

class Camera;

///////////////////////////////////////////////////////////////////////////////
// CameraMoveController

class CameraMoveController
{

public:

   CameraMoveController(Camera *c);

   virtual void update() const;

   virtual void forwardDown();
   virtual void forwardUp();
   virtual void reverseDown();
   virtual void reverseUp();
   virtual void leftDown();
   virtual void leftUp();
   virtual void rightDown();
   virtual void rightUp();

   Camera *m_pCamera;

};

class CameraFpsController : public CameraMoveController
{
public:
};

class CameraWowController : public CameraMoveController
{
public:
};

///////////////////////////////////////////////////////////////////////////////
// Camera

class Camera : public WorldObject
{

public:

   Camera();
   virtual ~Camera();

   // setup projection matrix
   void setup(int width = 1024, int height = 768, float fov = 45.0f);

   // handle mouse/kb movement
   void update() const;

   void place();

   const Vec3f &getLook() const;
   const Vec3f &getRight() const;
   const Vec3f &getUp() const;
   const Vec3f &getPos() const;

   void setPos(const Vec3f &pos);

   Mat4f &getMatrix() const;
   CameraMoveController *getController() { return m_pController; }

protected:

   Mat4f m_glmat;

   float m_aspect;
   float m_fov;
   float m_near;
   float m_far;
   Frustum m_frustum;

   ZimTime m_last;

   CameraMoveController *m_pController;

   friend class CameraMoveController;
   friend class Frustum;

};

}
