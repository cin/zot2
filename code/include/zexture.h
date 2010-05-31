#pragma once

#include <SDL_opengl.h>

struct SDL_Surface;

namespace Zot
{

class Zexture
{

public:

   Zexture();
   Zexture(const char *filename);
   virtual ~Zexture();

   virtual bool load(const char *filename);

   void destroyGlTexture();
   void destroySdlSurface();

   void gen();
   void bind();

   GLuint operator()();

protected:

   SDL_Surface *m_pSdlSurface;
   GLuint m_glTexture;

   GLenum m_format;
   GLenum m_target;
   GLint m_numColors;

};

}
