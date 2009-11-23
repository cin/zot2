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

class Camera
{

public:

   Camera();

   // setup perspective and modelview matrices
   void setup(int width = 1024, int height = 768, float fov = 45.0f);

   // handle mouse/kb movement
   void update() const;

   const Vec3f &getLook() const;
   const Vec3f &getRight() const;
   const Vec3f &getUp() const;

   Mat3f &getMatrix() const;

protected:

   Quatf ori;
   Vec3f pos;
   Mat3f mat;

   float aspect;
   float fov;
   float _near;
   float _far;
   Frustum frustum;

   ZimTime last;

};

}
