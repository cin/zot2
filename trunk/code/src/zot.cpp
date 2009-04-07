// zot.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <SDL.h>
#include <windows.h>

#include <stdio.h>

#include <gl/gl.h>
#include <gl/glu.h>

static int width = 800, height = 600;

bool init_sdl()
{
   const SDL_VideoInfo *video;

   if (SDL_Init(SDL_INIT_VIDEO) < 0)
   {
      fprintf(stderr, "SDL_Init() failed\n");
      return false;
   }
   atexit(SDL_Quit);

   video = SDL_GetVideoInfo();
   if (!video)
   {
      fprintf(stderr, "SDL_GetVideoInfo() failed\n");
      return false;
   }

   SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
   SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
   SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
   SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
   if (SDL_SetVideoMode(width, height, video->vfmt->BitsPerPixel, SDL_OPENGL) == 0)
   {
      fprintf(stderr, "SDL_SetVideoMode() failed\n");
      return false;
   }
   return true;
}

void init_gl()
{
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.0f, (float)width / (float)height, 1.0, 1000.0);
   glMatrixMode(GL_MODELVIEW);

   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glEnable(GL_DEPTH_TEST);
}

void repaint()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();
   glTranslatef(0, 0, -20);
   glBegin(GL_POLYGON);
   glVertex3i(5, 5, 0);
   glVertex3i(-5, 5, 0);
   glVertex3i(-5, -5, 0);
   glVertex3i(5, -5, 0);
   glEnd();

   SDL_GL_SwapBuffers();
}

void main_loop()
{
   SDL_Event event;

   while (1)
   {
      while (SDL_PollEvent(&event))
      {
         switch (event.type)
         {
         case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
               exit(0);
               break;
            default:
               break;
            }
            break;
         case SDL_QUIT:
            exit(0);
            break;
         }
      }
      repaint();
      SDL_Delay(50);
   }
}

int main(int argc, char *argv[])
{
   if (!init_sdl())
   {
      return -1;
   }
   
   init_gl();

   main_loop();

   return 0;
}
