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

bool Zexture::createSdlSurface(const char *filename)
{
   m_pSdlSurface = IMG_Load(filename);
   if (!m_pSdlSurface)
      return false;

   // code below is based off of code found at the following site:
   // http://gpwiki.org/index.php/SDL:Tutorials:Using_SDL_with_OpenGL
	// check that the image's width is a power of 2
   if ((m_pSdlSurface->w & (m_pSdlSurface->w - 1)) != 0)
   {
      ostringstream os;
      os << "Zexture::createSdlSurface: unable to load " << filename
         << " as its width is not a power of 2." << endl;
      Zogger::get()->zog(os.str());
      destroySdlSurface();
      return false;
   }

   // also check if the height is a power of 2
   if ((m_pSdlSurface->h & (m_pSdlSurface->h - 1)) != 0)
   {
      ostringstream os;
      os << "Zexture::createSdlSurface: unable to load " << filename
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
      os << "Zexture::createSdlSurface: " << filename << " is not truecolor." << endl;
      Zogger::get()->zog(os.str());
      destroySdlSurface();
      return false;
   }

#if 0
   // the following code is from
   // http://www.idevgames.com/forum/archive/index.php/t-9034.html
   // although the last post in this thread does indicate that it is extremely slow

   // check the format
   switch (m_pSdlSurface->format->BitsPerPixel)
   {
   case 8:
      //It's 8 bit, do whatever
      break;
   case 16:
      //It's 16 bit, do whatever
      break;
   case 24:
   {
      //It's 24 bit, so always convert (someone check my pixel format for me!)
      //Note that this scoping is used to prevent the declaration of format twice....
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
      SDL_PixelFormat format = { NULL, 32, 4, 0, 0, 0, 0, 0, 8, 16, 24, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF, 0, 255 };
#else
      SDL_PixelFormat format = { NULL, 32, 4, 0, 0, 0, 0, 0, 8, 16, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000, 0, 255 };
#endif
      SDL_Surface *temp = SDL_ConvertSurface(m_pSdlSurface, &format, SDL_SWSURFACE);
      SDL_FreeSurface(m_pSdlSurface);
      m_pSdlSurface = temp;
      break;
   }
   case 32:
   {
      //It's 32 bit, so convert only if it's ABGR (and really, check this pixel format!)
      if (m_pSdlSurface->format->Rshift > m_pSdlSurface->format->Bshift)
      {
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
         SDL_PixelFormat format = { NULL, 32, 4, 0, 0, 0, 0, 0, 8, 16, 24, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF, 0, 255 };
#else
         SDL_PixelFormat format = { NULL, 32, 4, 0, 0, 0, 0, 0, 8, 16, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000, 0, 255 };
#endif
         SDL_Surface *temp = SDL_ConvertSurface(m_pSdlSurface, &format, SDL_SWSURFACE);
         SDL_FreeSurface(m_pSdlSurface);
         m_pSdlSurface = temp;
      }
      break;
   }
   default:
      break;
   }
#endif
   return true;
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

bool Zexture::load(const char *filename, bool bKeepSdlSurface)
{
   bool ret = createSdlSurface(filename) && setupGl();

   // clean up intermediate surface
   if (!bKeepSdlSurface)
      destroySdlSurface();

   return ret;
}

void Zexture::gen()
{
   glGenTextures(1, &m_glTexture);
}

GLuint Zexture::operator()()
{
   return m_glTexture;
}

bool Zexture::setupGl()
{
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
