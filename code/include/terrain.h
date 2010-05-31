#pragma once

#include <vector>
#include "vector.h"

namespace Zot
{

class Terrain;
class Quad;
class Zexture;

class Quad
{

public:

   typedef enum EQuad
   {
      EQuadSW,
      EQuadSE,
      EQuadNE,
      EQuadNW,
      EQuadLast,
   } EQuad;

   static const int sMaxVerts = 9;

   Quad(int _parent, int _level);

   int getParent() const;
   void setParent(int i);

   int getLevel() const { return level; }
   void setLevel(int lvl) { level = lvl; }

   int get(int i) const;
   void set(int i, int v);

protected:

   // 0 index is center
   int tind[sMaxVerts];
   int parent;
   int children[4];
   int level;

protected:

   friend class Terrain;

};

class Terrain
{

public:

   typedef std::vector<Quad *> Quads;
   typedef Quads::iterator QIter;
   typedef Quads::const_iterator QCIter;

public:

   Terrain();
   virtual ~Terrain();

   void build();
   void draw();
   void initTerrain();

protected:

   virtual int buildR(Quad *q, int center);
   virtual void drawR(Quad *q);

   // gl arrays
   Vec3f *grid; // 1d in memory but really 2d, % dim.x local coords
   Pnt2f *txtc; // texture coordinates, u, v
   Vec3f *nrms; // normals, 0, 1, 0 initially
   Quads quads; // vector quads

   // attributes
   Pnt2i dims; // x = width, y = heigth
   Pnt2f spacing; // spacing in meters
   //Vec3f center; // in world space

   static int sMaxLevel;
   static int sMinLevel;

   // 0 is llc, 1 is urc (lower left and upper right corner)
   // global coordinate min/max
   Pnt2f extents[2];

   Zexture *tex;

protected:

   friend class Quad;

};

}
