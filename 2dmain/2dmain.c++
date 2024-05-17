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
  -0.2,-0.2,
  -0.2, 0.2,
   0.2, 0.2,
   0.2,-0.2,
  -0.2,-0.2,
   0.2, 0.2,
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

  char *imagedata=assets::getasset("yahia-potato.png");
  SDL_RWops *imagerw=SDL_RWFromConstMem(imagedata, strlen(imagedata));
  SDL_Surface *sdlimg = IMG_Load_RW(imagerw,true);
  if(!sdlimg){
    exit(1);
  }
  // texture ret;
  // glGenTextures(1, &ret);
  // glBindTexture(GL_TEXTURE_2D, ret);
  // glTexImage2D(GL_TEXTURE_2D, 0, 3, tex_surf->w, tex_surf->h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_surf->pixels);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // SDL_FreeSurface(tex_surf);
  // return ret;
  
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

  window->setUniformFloat("x",data.x);
  window->setUniformFloat("y",data.y);

  window->addVertexData("coord",verts,6*2*sizeof(float),2,2*sizeof(float));
  
  window->mainLoop();
  
  delete window;
  
  SDL_Quit();
  
  return EXIT_SUCCESS;
}

