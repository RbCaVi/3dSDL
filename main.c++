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
  
  matrix4x4 model=getIdentity();
  matrix4x4 view=getTranslation(0,0,-5);
  matrix4x4 projection=getPerspective(45.0f, (float)HEIGHT/WIDTH, 0.1f, 100.0f);
  
  window->addUniformMat4x4("model",model);
  window->addUniformMat4x4("view",view);
  window->addUniformMat4x4("projection",projection);
  window->addUniformMat4x4("mvp",projection*view*model);
  
  window->addVertexData("coord2d",vertices,sizeof(vertices),3);
  window->addVertexData("in_color",colors,sizeof(colors),3);

  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  
  //window->mainLoop();
  for(auto attr:window->handles){
    glEnableVertexAttribArray(glGetAttribLocation(window->program,attr.first));
  }
  
  bool closed=false;
  
  while (!closed) {
    model=getRotation(1,2,0.01)*model;
    window->addUniformMat4x4("model",model);
    glClear(GL_COLOR_BUFFER_BIT // clear the background
        | GL_DEPTH_BUFFER_BIT); // and the depth buffer
    glDrawArrays(window->draw_mode, 0, window->draw_vertices);
    SDL_GL_SwapWindow(window->window); // draw the graphics buffer to the screen
    
    SDL_Event event;
 
    // Events management
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
 
       case SDL_QUIT:
        // handling of close button
        closed = true;
        break;
       default:
        break;
      }
    }
  }
  
  for(auto attr:window->handles){
    glDisableVertexAttribArray(glGetAttribLocation(window->program,attr.first));
  }
  
  delete window;
  
  SDL_Quit();
  return EXIT_SUCCESS;
}

