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

class CameraMoveController
{
public:
   CameraMoveController();
};

class CameraFpsController : public CameraMoveController
{
public:
};

class CameraWowController : public CameraMoveController
{
public:
};

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

   Mat4f &getMatrix() const;

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

};

}
