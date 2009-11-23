#pragma once

#include "vector.h"

namespace Zot
{

template<typename T>
class Matrix3
{

public:

   T m[9];

   Matrix3()
   {
      identity();
   }

   Matrix3(const Matrix3 &other)
   {
      for (int i = 0; i < 9; i++)
         m[i] = other.m[i];
   }

   T &operator[](int i)
   {
      if (i < 0 || i > 8)
         return m[0];
      return m[i];
   }

   Matrix3 &operator=(const Matrix3 &rhs)
   {
      if (this != &rhs)
      {
         for (int i = 0; i < 9; i++)
            m[i] = other.m[i];
      }
      return *this;
   }

   Matrix3 &identity()
   {
      for (int i = 0; i < 9; i++)
         m[i] = (i % 5) == 0 ? 1.0f : 0.0f;
      return *this;
   }
};

///////////////////////////////////////////////////////////////////////////////
// Matrix4

template<typename T>
class Matrix4
{

public:

   T m[16];

   Matrix4()
   {
      identity();
   }

   Matrix4(const Matrix4 &other)
   {
      for (int i = 0; i < 16; i++)
         m[i] = other.m[i];
   }

   //Matrix4(boost::math::quaternion<T> &other)
   //{
   //   other.gl
   //}

   T &operator[](int i)
   {
      if (i < 0 || i > 15)
         return m[0];
      return m[i];
   }

   Matrix4 &operator=(const Matrix4 &rhs)
   {
      if (this != &rhs)
      {
         for (int i = 0; i < 16; i++)
            m[i] = other.m[i];
      }
      return *this;
   }

   Matrix4<T> &identity()
   {
      for (int i = 0; i < 16; i++)
         m[i] = (i % 5) == 0 ? 1.0f : 0.0f;
      return *this;
   }

};

typedef Matrix3<float> Mat3f;
typedef Matrix3<double> Mat3d;

typedef Matrix4<float> Mat4f;
typedef Matrix4<double> Mat4d;

}
