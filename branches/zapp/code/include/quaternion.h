#pragma once

#include <boost/math/quaternion.hpp>
#include "vector.h"
#include "matrix.h"

namespace Zot
{

template<typename T>
class Quaternion : public boost::math::quaternion<T>
{

public:

   Quaternion &set(const Vector<T> &axis, T angle)
   {
      Vector<T>  tmpv(axis.norm());
      T tmpSin(sin(angle * 0.5f));

      Vector<T> v;
      this->a = cos(angle * 0.5f);
      v = tmpv * tmpSin;
      this->b = v.x;
      this->c = v.y;
      this->d = v.z;

      return *this;
   }

   T length() const
   {
      return sqrt(sqLength());
   }

   T sqLength() const
   {
      Vector<T> v(this->b, this->c, this->d);
      return (this->a * this->a + v.squaredLength());
   }

   Vector<T> &getLook(Vector<T> &look) const
   {
      T w(sqLength());
      w = (w > 0.0f) ? 2.0f / w : 0.0f;
      T wx(this->b * w);   T wy(this->c * w); T wz(this->d * w);
      T sx(this->a * wx);  T sy(this->a * wy);
      T xx(this->b * wx);  T xz(this->b * wz);
      T yy(this->c * wy);  T yz(this->c * wz);
      return look.set(xz + sy, yz - sx, 1.0f - (xx + yy));
   }

   Vector<T> &getRight(Vector<T> &right) const
   {
      T w(sqLength());
      w = (w > 0.0f) ? 2.0f / w : 0.0f;
      // double wx(b * w);
      T wy(this->c * w);   T wz(this->d * w);
      T sy(this->a * wy);  T sz(this->a * wz);
      T xy(this->b * wy);  T xz(this->b * wz);
      T yy(this->c * wy);  T zz(this->d * wz);
      return right.set(1.0f - (yy + zz), xy + sz, xz - sy);
   }

   Vector<T> &getUp(Vector<T> &up) const
   {
      T w(sqLength());
      w = (w > 0.0f) ? 2.0f / w : 0.0f;
      T wx(this->b * w);   T wy(this->c * w);   T wz(this->d * w);
      T sx(this->a * wx);  T sz(this->a * wz);
      T xx(this->b * wx);  T xy(this->b * wy);
      T yz(this->c * wz);  T zz(this->d * wz);
      return up.set(xy - sz, 1.0f - (xx + zz), yz + sx);
   }

   Matrix4<T> &glMatrix(Matrix4<T> &m, const Vector<T> &pos) const
   {
      T w(sqLength());
      w = (w > 0.0f) ? 2.0f / w : 0.0f;
      T wx(this->b * w);   T wy(this->c * w);   T wz(this->d * w);
      T sx(this->a * wx);  T sy(this->a * wy);  T sz(this->a * wz);
      T xx(this->b * wx);  T xy(this->b * wy);  T xz(this->b * wz);
      T yy(this->c * wy);  T yz(this->c * wz);  T zz(this->d * wz);
      m[0]  = T(1.0f - (yy + zz));
      m[1]  = T(       (xy + sz));
      m[2]  = T(       (xz - sy));
      m[4]  = T(       (xy - sz));
      m[5]  = T(1.0f - (xx + zz));
      m[6]  = T(       (yz + sx));
      m[8]  = T(       (xz + sy));
      m[9]  = T(       (yz - sx));
      m[10] = T(1.0f - (xx + yy));
      m[12] = pos.x;
      m[13] = pos.y;
      m[14] = pos.z;
      m[3]  = 0.0f; m[7]  = 0.0f; m[11] = 0.0f;
      m[15] = 1.0f;
      return m;
   }

};

typedef Quaternion<float> Quatf;
typedef Quaternion<double> Quatd;

}
