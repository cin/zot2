#include "zot.h"
#include "zexture.h"
#include "zutil.h"
#include "zogger.h"

#include <sstream>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;
using namespace Zot;

Zexture::Zexture()
   : m_pSdlSurface(NULL)
   , m_glTexture(0)
   , m_format(GL_RGB)
   , m_target(GL_TEXTURE_2D)
   , m_numColors(0)
{
}

Zexture::~Zexture()
{
   // intentionally not cleaning up texture data here as
   // they may be used outside of this class. really should
   // just look at this class as a wrapper for encapsulating
   // simple, common texture functions
}

void Zexture::bind()
{
   glBindTexture(m_target, m_glTexture);
}

void Zexture::destroyGlTexture()
{
   glDeleteTextures(1, &m_glTexture);
}

void Zexture::destroySdlSurface()
{
   if (m_pSdlSurface)
   {
      SDL_FreeSurface(m_pSdlSurface);
      m_pSdlSurface = NULL;
   }
}

bool Zexture::load(const char *filename)
{
#if 0 // wrote this before i realized sdl_image can load bmps too
   const char *dot = strrchr(filename, '.');
   if (dot == NULL)
      return false;

   const char *ext = ++dot;
   char buf[16];
   strncpy_s(buf, sizeof(buf), ext, strlen(ext));
   tolower(buf);

   // if it's a bmp, call SDL's LoadBmp
   // otherwise, call SDL_image's load
   if (0 == strncmp(buf, "bmp", strlen(buf)))
      m_pSdlSurface = SDL_LoadBMP(filename);
   else
      m_pSdlSurface = IMG_Load(filename);
#endif

   m_pSdlSurface = IMG_Load(filename);
   if (!m_pSdlSurface)
      return false;

   // code below is based off of code found at the following site:
   // http://gpwiki.org/index.php/SDL:Tutorials:Using_SDL_with_OpenGL
	// check that the image's width is a power of 2
   if ((m_pSdlSurface->w & (m_pSdlSurface->w - 1)) != 0)
   {
      ostringstream os;
      os << "Zexture::load: unable to load " << filename
         << " as its width is not a power of 2." << endl;
      Zogger::get()->zog(os.str());
      destroySdlSurface();
      return false;
   }

   // also check if the height is a power of 2
   if ((m_pSdlSurface->h & (m_pSdlSurface->h - 1)) != 0)
   {
      ostringstream os;
      os << "Zexture::load: unable to load " << filename
         << " as its height is not a power of 2." << endl;
      Zogger::get()->zog(os.str());
      destroySdlSurface();
      return false;
   }

   // get the number of channels in the SDL surface
   m_numColors = m_pSdlSurface->format->BytesPerPixel;
   if (m_numColors == 4) // contains an alpha channel
   {
      if (m_pSdlSurface->format->Rmask == 0x000000ff)
         m_format = GL_RGBA;
      else
         m_format = GL_BGRA;
   }
   else if (m_numColors == 3) // no alpha channel
   {
      if (m_pSdlSurface->format->Rmask == 0x000000ff)
         m_format = GL_RGB;
      else
         m_format = GL_BGR;
   }
   else
   {
      ostringstream os;
      os << "Zexture::load: " << filename << " is not truecolor." << endl;
      Zogger::get()->zog(os.str());
      destroySdlSurface();
      return false;
   }

   gen();
   bind();

   // set the texture's stretching properties
   glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // edit the texture object's image data using the information SDL_Surface gives us
   glTexImage2D(m_target, 0, m_numColors, m_pSdlSurface->w, m_pSdlSurface->h, 0,
      m_format, GL_UNSIGNED_BYTE, m_pSdlSurface->pixels);

   return true;
}

void Zexture::gen()
{
   glGenTextures(1, &m_glTexture);
}

GLuint Zexture::operator()()
{
   return m_glTexture;
}
