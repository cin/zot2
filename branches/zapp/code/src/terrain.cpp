#include "zot.h"
#include "Terrain.h"

#include <CEGUIDefaultResourceProvider.h>
#ifdef __CEGUI_0_6_2__
#include <OpenGLGUIRenderer/openglrenderer.h>
#else
#include <OpenGL/CEGUIOpenGLRenderer.h>
#endif

using namespace Zot;

Pnt2f gDefExt[2] =
{
   Pnt2f(-100.0f,  100.0f),
   Pnt2f( 100.0f, -100.0f)
};

Vec3f sZeros;

int Terrain::sMaxLevel = 2;
int Terrain::sMinLevel = 2;

///////////////////////////////////////////////////////////////////////////////
// Quad

Quad::Quad(int _parent, int _level)
   : parent(_parent)
   , level(_level)
{
   for (int i = 0; i < sMaxVerts; i++)
      tind[i] = 0;
}

int Quad::getParent() const
{
   return parent;
}

void Quad::setParent(int p)
{
   parent = p;
}

int Quad::get(int i) const
{
   if (i < 0 || i >= sMaxVerts)
      return -1;
   return tind[i];
}

void Quad::set(int i, int v)
{
   if (i < 0 || i >= sMaxVerts)
      return;
   tind[i] = v;
}

///////////////////////////////////////////////////////////////////////////////
// Terrain

Terrain::Terrain()
   : grid(NULL)
   , txtc(NULL)
   , nrms(NULL)
   , dims(8, 8)
   , spacing(1.0f, 1.0f)
   //, dims(gDefExt[1].x - gDefExt[0].x, gDefExt[1].y - gDefExt[1].y)
{
   extents[0] = gDefExt[0];
   extents[1] = gDefExt[1];
}

Terrain::~Terrain()
{
   for (QIter it = quads.begin(); it != quads.end();)
   {
      delete *it;
      it = quads.erase(it);
   }
}

void Terrain::initTerrain()
{
#if 1
   const float hgts[81] =
   {
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
   };

   int sz = sizeof(hgts) / sizeof(float), idx = 0, gidx = 0;
   grid = new Vec3f[sz];
   dims.init(8, 8);
   for (float y = 8.0f, ya = 0.0f; y >= 0.0f; y -= 1.0f, ya += 1.0f)
   {
      int col = int(y * 9);
      for (float x = 0.0f; x < 9.0f; x += 1.0f, gidx++)
      {
         idx = int(col + x);
         grid[gidx].init(x, ya, hgts[idx]);
      }
   }

   // initialize normals
   // initialize texture coordinates
#else
   // load from file
#endif
}

void Terrain::build()
{
   buildR(new Quad(-1, 0), int(dims.x * 4.5f + dims.x * 0.5f));
}

int Terrain::buildR(Quad *q, int center)
{
   // sets up the vertices for the data
   int lvl = q->getLevel();
   // big d (dims) is the width whole thing
   // little d is the width of this particular quad
   int d = dims.x >> lvl;
   int dx = d / 2, dy = d * (dims.x + 1) / 2;

   q->tind[0] = center;
   q->tind[1] = center - dx - dy;
   q->tind[2] = center - dy;
   q->tind[3] = center + dx - dy;
   q->tind[4] = center + dx;
   q->tind[5] = center + dx + dy;
   q->tind[6] = center + dy;
   q->tind[7] = center - dx + dy;
   q->tind[8] = center - dx;

   quads.push_back(q);

   // this quad's index
   int idx = quads.size() - 1;
   if (lvl < Terrain::sMaxLevel)
   {
      int clvl = lvl + 1;
      dx = d / 4, dy = d * (dims.x + 1) / 4;
      q->children[0] = buildR(new Quad(idx, clvl), center - dx - dy);
      q->children[1] = buildR(new Quad(idx, clvl), center + dx - dy);
      q->children[2] = buildR(new Quad(idx, clvl), center + dx + dy);
      q->children[3] = buildR(new Quad(idx, clvl), center - dx + dy);
   }
   return idx;
}

void Terrain::draw()
{
   QIter it = quads.begin();
   drawR(*it);
}

void Terrain::drawR(Quad *q)
{
   int lvl = q->getLevel();
   if (lvl < Terrain::sMaxLevel)
   {
      for (int i = Quad::EQuadSW; i < Quad::EQuadLast; i++)
         drawR(quads[q->children[i]]);
   }
   else
   {
      glBegin(GL_TRIANGLE_FAN);
         for (int i = 0; i < 9; i++)
            glVertex3fv((float *)&grid[q->tind[i]]);
         glVertex3fv((float *)&grid[q->tind[1]]);
      glEnd();
   }
}
