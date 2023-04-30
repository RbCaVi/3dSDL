#include "shared.h++"

#include <filesystem>
#include <exception>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "shaders.h++"
#include "window.h++"
#include "matrix.h++"

static const GLuint WIDTH = 512;
static const GLuint HEIGHT = 512;
static GLfloat vertices[] = {
   0.3,  0.3,  0.0,
   0.3, -0.3,  0.0,
  -0.3,  0.3,  0.0,
   0.3, -0.3,  0.0,
  -0.3,  0.3,  0.0,
  -0.3, -0.3,  0.0,

   0.7,  0.8,  0.1,
   0.8, -0.8,  0.0,
  -0.8,  0.8,  0.0,
   0.8, -0.8,  0.0,
  -0.8,  0.8,  0.0,
  -0.7, -0.8, -0.1,
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
  1.0, 1.0, 1.0, 1.0,
  1.0, 1.0, 0.0, 1.0,
  0.0, 1.0, 1.0, 1.0,
  1.0, 1.0, 0.0, 1.0,
  0.0, 1.0, 1.0, 1.0,
  0.0, 1.0, 0.0, 1.0,

  1.0, 0.0, 1.0, 1.0,
  1.0, 0.0, 0.0, 1.0,
  0.0, 0.0, 1.0, 1.0,
  1.0, 0.0, 0.0, 1.0,
  0.0, 0.0, 1.0, 1.0,
  0.0, 0.0, 0.0, 1.0,
};

void rotate(Window*,void*);

static GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat light_diffuse[] = { 0.0, 0.0, 0.0, 1.0 };
static GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };
//static GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
//static GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat light_position[] = { -1.0, -1.0, -1.0, 0.0 };

static GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat mat_shininess[] = { 50.0 };

struct sdata{
  matrix4x4 model;
  matrix4x4 view;
  matrix4x4 projection;
};
typedef sdata sdata;

int main(int argc, char *argv[]) {
  // assuming that argv[0] is the program's path
  std::filesystem::path basedir=std::filesystem::path(argv[0]).parent_path();
  std::filesystem::path assetsdir=basedir/"assets";
  SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
  
  sdata data;
  Window *window;
  window=new Window(WIDTH,HEIGHT,__FILE__,rotate,(void*)(&data),true);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel (GL_SMOOTH);

  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  
  window->makeShader(assetsdir/"shader.vert", assetsdir/"shader.frag");
  data.model=getIdentity();
  data.view=getTranslation(0,0,-5);
  data.projection=getPerspective(45.0f, (float)HEIGHT/WIDTH, 0.1f, 100.0f);
  
  window->addUniformMat4x4("model",data.model);
  window->addUniformMat4x4("view",data.view);
  window->addUniformMat4x4("projection",data.projection);
  window->addUniformMat4x4("mvp",data.projection*data.view*data.model);
  
  window->addVertexData("coord",vertices,sizeof(vertices),3);
  window->addVertexData("normal",vertices,sizeof(normals),3);
  window->addVertexData("color",colors,sizeof(colors),4);

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  
  window->mainLoop();
  
  delete window;
  
  SDL_Quit();
  return EXIT_SUCCESS;
}

void rotate(Window *window, void *d){
  sdata *data=(sdata*)d;
  data->model=getRotation(1,2,0.01)*data->model;
  window->addUniformMat4x4("model",data->model);
}
