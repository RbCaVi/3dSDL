#include "shared.h"

#include <filesystem>
#include <exception>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "shaders.h"
#include "window.h"

static const GLuint WIDTH = 512;
static const GLuint HEIGHT = 512;
static GLfloat vertices[] = {
   0.3,  0.3, -2.5,
   0.3, -0.3, -2.5,
  -0.3,  0.3, -2.5,
   0.3, -0.3, -2.5,
  -0.3,  0.3, -2.5,
  -0.3, -0.3, -2.5,
  
   0.7,  0.7, -2.6,
   0.8, -0.8, -2.5,
  -0.8,  0.8, -2.5,
   0.8, -0.8, -2.5,
  -0.8,  0.8, -2.5,
  -0.8, -0.8, -2.4,
};
static GLfloat normals[] = {
   0.0,  0.0, 1.0,
   0.0, -0.0, 1.0,
  -0.0,  0.0, 1.0,
   0.0, -0.0, 1.0,
  -0.0,  0.0, 1.0,
  -0.0, -0.0, 1.0,
   0.0,  0.0, 1.0,
   0.0, -0.0, 1.0,
  -0.0,  0.0, 1.0,
   0.0, -0.0, 1.0,
  -0.0,  0.0, 1.0,
  -0.0, -0.0, 1.0,
};
static GLfloat colors[] = {
  1.0, 1.0, 1.0,
  1.0, 1.0, 0.0,
  0.0, 1.0, 1.0,
  1.0, 1.0, 0.0,
  0.0, 1.0, 1.0,
  0.0, 1.0, 0.0,
  1.0, 0.0, 1.0,
  1.0, 0.0, 0.0,
  0.0, 0.0, 1.0,
  1.0, 0.0, 0.0,
  0.0, 0.0, 1.0,
  0.0, 0.0, 0.0,
};



static GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat light_diffuse[] = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };
//static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
//static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_position[] = { -1.0, -1.0, -1.0, 0.0 };

static GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat mat_shininess[] = { 50.0 };


int main(int argc, char *argv[]) {
  // assuming that argv[0] is the program's path
  std::filesystem::path basedir=std::filesystem::path(argv[0]).parent_path();
  std::filesystem::path assetsdir=basedir/"assets";
  SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
  Window *window;
  window=new Window(WIDTH,HEIGHT,__FILE__);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  
  window->makeShader(assetsdir/"shader.vert", assetsdir/"shader.frag");
  
  matrix4x4 m;
  
  setPerspective(m,45.0f, (float)HEIGHT/WIDTH, 0.1f, 100.0f);
  
  window->addUniformMat4x4("projection",m);
  
  window->addVertexData("coord2d",vertices,sizeof(vertices),3);
  window->addVertexData("in_color",colors,sizeof(colors),3);

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  
  window->mainLoop();
  
  delete window;
  
  SDL_Quit();
  return EXIT_SUCCESS;
}

