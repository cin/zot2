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
      a = cos(angle * 0.5f);
      v = tmpv * tmpSin;
      b = v.x;
      c = v.y;
      d = v.z;

      return *this;
   }

   T length() const
   {
      return sqrt(sqLength());
   }

   T sqLength() const
   {
      Vector<T> v(b, c, d);
      return (a * a + v.squaredLength());
   }

   Vector<T> &getLook(Vector<T> &look) const
   {
      T w(sqLength());
      w = (w > 0.0f) ? 2.0f / w : 0.0f;
      T wx(b * w);   T wy(c * w); T wz(d * w);
      T sx(a * wx);  T sy(a * wy);
      T xx(b * wx);  T xz(b * wz);
      T yy(c * wy);  T yz(c * wz);
      return look.set(xz + sy, yz - sx, 1.0f - (xx + yy));
   }

   Vector<T> &getRight(Vector<T> &right) const
   {
      T w(sqLength());
      w = (w > 0.0f) ? 2.0f / w : 0.0f;
      // double wx(b * w);
      T wy(c * w);   T wz(d * w);
      T sy(a * wy);  T sz(a * wz);
      T xy(b * wy);  T xz(b * wz);
      T yy(c * wy);  T zz(d * wz);
      return right.set(1.0f - (yy + zz), xy + sz, xz - sy);
   }

   Vector<T> &getUp(Vector<T> &up) const
   {
      T w(sqLength());
      w = (w > 0.0f) ? 2.0f / w : 0.0f;
      T wx(b * w);   T wy(c * w);   T wz(d * w);
      T sx(a * wx);  T sz(a * wz);
      T xx(b * wx);  T xy(b * wy);
      T yz(c * wz);  T zz(d * wz);
      return up.set(xy - sz, 1.0f - (xx + zz), yz + sx);
   }

   Matrix4<T> &glMatrix(Matrix4<T> &m, const Vector<T> &pos) const
   {
      T w(sqLength());
      w = (w > 0.0f) ? 2.0f / w : 0.0f;
      T wx(b * w);   T wy(c * w);   T wz(d * w);
      T sx(a * wx);  T sy(a * wy);  T sz(a * wz);
      T xx(b * wx);  T xy(b * wy);  T xz(b * wz);
      T yy(c * wy);  T yz(c * wz);  T zz(d * wz);
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
