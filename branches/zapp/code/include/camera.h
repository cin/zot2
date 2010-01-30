#pragma once

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

   Camera *pCamera;

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

class Camera
{

public:

   Camera();

   // setup projection matrix
   void setup(int width = 1024, int height = 768, float fov = 45.0f);

   // handle mouse/kb movement
   void update() const;

   void place();

   const Vec3f &getLook() const;
   const Vec3f &getRight() const;
   const Vec3f &getUp() const;
   const Vec3f &getPos() const;

   void setPos(const Vec3f &_pos);

   Mat4f &getMatrix() const;
   CameraMoveController *getController() { return pController; }

protected:

   Quatf ori;
   Vec3f pos;
   Mat4f glmat;

   float aspect;
   float fov;
   float _near;
   float _far;
   Frustum frustum;

   ZimTime last;

   CameraMoveController *pController;

   friend CameraMoveController;
   friend Frustum;

};

}
