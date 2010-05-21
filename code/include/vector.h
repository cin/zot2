#pragma once

namespace Zot
{

///////////////////////////////////////////////////////////////////////////////
// Point2d

template<typename T>
struct Point2d
{

   T x;
   T y;

   // should T(0) these for type safety but for performance here...
   Point2d()
      : x(0)
      , y(0)
   {
   }

   Point2d(T _x, T _y)
      : x(_x)
      , y(_y)
   {
   }

   Point2d(const Point2d<T> &other)
      : x(other.x)
      , y(other.y)
   {
   }

   Point2d<T> &init(T _x, T _y)
   {
      x = _x;
      y = _y;
      return *this;
   }

   Point2d<T> &operator=(const Point2d<T> &rhs)
   {
      if (this != &rhs)
      {
         x = rhs.x;
         y = rhs.y;
      }
      return *this;
   }

};

///////////////////////////////////////////////////////////////////////////////
// Vector

template<typename T>
struct Vector
{

   T x;
   T y;
   T z;

   Vector()
      : x(0)
      , y(0)
      , z(0)
   {
   }

   Vector(T _x, T _y, T _z)
      : x(_x)
      , y(_y)
      , z(_z)
   {
   }

   Vector(const Vector<T> &other)
      : x(other.x)
      , y(other.y)
      , z(other.z)
   {
   }

   Vector<T> &init(T _x, T _y, T _z)
   {
      x = _x;
      y = _y;
      z = _z;
      return *this;
   }

   Vector<T> operator*(const Vector<T> &rhs) const
   {
      Vector<T> r;
      r.x = x * rhs.x;
      r.y = y * rhs.y;
      r.z = z * rhs.z;
      return r;
   }

   Vector<T> &operator*=(const Vector<T> &rhs)
   {
      x *= rhs.x;
      y *= rhs.y;
      z *= rhs.z;
      return *this;
   }

   Vector<T> &operator*=(const T &rhs)
   {
      x *= rhs;
      y *= rhs;
      z *= rhs;
      return *this;
   }

   Vector<T> &operator+=(const Vector<T> &rhs)
   {
      x += rhs.x;
      y += rhs.y;
      z += rhs.z;
      return *this;
   }

   Vector<T> &operator+=(const T &rhs)
   {
      x += rhs;
      y += rhs;
      z += rhs;
      return *this;
   }

   Vector<T> &operator=(const Vector<T> &rhs)
   {
      if (this != &rhs)
      {
         x = rhs.x;
         y = rhs.y;
         z = rhs.z;
      }
      return *this;
   }

   Vector<T> operator-()
   {
      return Vector<T>(-x, -y, -z);
   }

   Vector<T> &normalize()
   {
      T d = length();
      if (d == 0.0f)
         return *this;
      T l = 1.0f / d;
      x *= l; y *= l; z *= l;
      return *this;
   }

   Vector<T> norm() const
   {
      T d = length();
      if (d == 0.0f)
         return Vector<T>();
      T l = 1.0f / d;
      return Vector<T>(x * l, y * l, z * l);
   }

   Vector<T> &norm(Vector<T> &vec) const
   {
      T d = length();
      if (d == 0.0f)
         return vec;
      T l = 1.0f / d;
      return vec.set(x * l, y * l, z * l);
   }

   T length() const
   {
      return sqrt(x * x + y * y + z * z);
   }

   T squaredLength() const
   {
      return (x * x + y * y + z * z);
   }

   Vector<T> &set(const T &_x, const T &_y, const T &_z)
   {
      x = _x; y = _y; z = _z;
      return *this;
   }

};

typedef Vector<int32> Vec3i;
typedef Vector<uint32> Vec3ui;
typedef Vector<float> Vec3f;

typedef Point2d<int32> Pnt2i;
typedef Point2d<uint32> Pnt2ui;
typedef Point2d<float> Pnt2f;

static const Vec3f sZeros;

}
