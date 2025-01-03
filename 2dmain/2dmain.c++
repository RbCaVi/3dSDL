#include "shared.h++"

#include <filesystem>
#include <exception>
#include <signal.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_rwops.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>

#include "2dmain.h++"
#include "shaders.h++"
#include "window.h++"
#include "matrix.h++"
#include "parseargs.h++"
#include "assets/assets.h++"
#include "random.h++"

#include "2dmainfuncs.c++"

static const GLuint WIDTH = 512;
static const GLuint HEIGHT = 512;

void p(argdata adata){
  printf("%i %i %i\n",adata.error,adata.capture,adata.framecount);
}

static float verts[] = {
  -0.2,-0.2, 0,0,
  -0.2, 0.2, 0,1,
   0.2, 0.2, 1,1,
   0.2,-0.2, 1,0,
  -0.2,-0.2, 0,0,
   0.2, 0.2, 1,1,
};

int main(int argc, char *argv[]) {
  IGNORE(argc);
  // assuming that argv[0] is the program's path
  std::filesystem::path basedir=std::filesystem::path(argv[0]).parent_path();
  std::filesystem::path assetsdir=basedir/"assets";
  assets::setroot(assetsdir);
  
  struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = handlesigint;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);
  
  // argument parser
  argdata adata(false,false,0);
  parseargs(argc,(const char**)argv,isopt,optargs,addopt,addarg,(void*)(&adata));
  if(adata.error){
    exit(1);
  }
  
  DEBUGR(MAIN_DEBUG,p(adata));
  
  SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
  
  sdata data;
  window=new Window(WIDTH,HEIGHT,__FILE__);
  if(adata.capture){
    window->enableSaveFrames(adata.framecount);
  }
  window->data=(void*)(&data);
  window->onframe=onframe;
  window->onkeydown=onkeydown;
  window->onkeyup=onkeyup;
  window->draw=draw;
  glEnable(GL_DEPTH_TEST);

  glClearColor (0.0, 0.0, 0.0, 0.0);
  
  char *vertsource=assets::getasset("2dshader.vert");
  char *fragsource=assets::getasset("2dshader.frag");
  window->makeShaderFromSource(vertsource,fragsource);
  free(vertsource);
  free(fragsource);
  data.x=0;
  data.y=0;

  assets::sizeasset imagedata = assets::getassetwithsize("hecker.png");
  SDL_RWops *imagerw=SDL_RWFromConstMem(imagedata.asset, imagedata.size);
  SDL_Surface *image = IMG_Load_RW(imagerw,true);
  if(!image){
    exit(1);
  }

  image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA8888, 0);
  GLuint gltex;
  glGenTextures(1,&gltex);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D,gltex);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image->w,image->h,0,GL_RGBA,GL_UNSIGNED_BYTE,image->pixels);
  SDL_FreeSurface(image);

  window->setUniformInt("img",gltex);

  window->setUniformFloat("x",data.x);
  window->setUniformFloat("y",data.y);

  Window::vertexdata vertices = window->addVertexData(verts,6,4);
  window->applyVertexData(vertices,"coord",2);
  window->applyVertexData(vertices,"uv",2,2);
  
  window->mainLoop();
  
  delete window;
  
  SDL_Quit();
  
  return EXIT_SUCCESS;
}

