#include "shared.h++"

#include <filesystem>
#include <exception>
#include <signal.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "objmain.h++"
#include "shaders.h++"
#include "window.h++"
#include "matrix.h++"
#include "parseargs.h++"
#include "obj.h++"
#include "assets/assets.h++"

#include "objmainfuncs.c++"

static const GLuint WIDTH = 512;
static const GLuint HEIGHT = 512;

void p(argdata adata){
  printf("%i %i %i\n",adata.error,adata.capture,adata.framecount);
}

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
  glEnable(GL_DEPTH_TEST);

  glClearColor (0.0, 0.0, 0.0, 0.0);
  
  char *vertsource=assets::getasset("objshader.vert");
  char *fragsource=assets::getasset("objshader.frag");
  window->makeShaderFromSource(vertsource,fragsource);
  free(vertsource);
  free(fragsource);
  data.rot=getIdentity();
  data.trans=getIdentity();
  data.view=getTranslation(0,0,-200);
  data.projection=getPerspective(45.0f, ((float)WIDTH)/HEIGHT, 0.1f, 800.0f);

  obj o;
  o.loadasset("cat.obj");
  obj::renderdata rdata=o.makeRenderData();

  window->setUniformMat4x4("model",data.model);
  window->setUniformMat4x4("view",data.view);
  window->setUniformMat4x4("projection",data.projection);
  window->setUniformMat4x4("mvp",data.projection*data.view*data.model);
  
  DEBUGR(MAIN_DEBUG,
    int i;
    for(i=0;i<rdata.size*10;i+=10){
      printf("vertex coord (%f,%f,%f,%f)\n",rdata.vs[i],rdata.vs[i+1],rdata.vs[i+2],rdata.vs[i+3]);
      printf("tex coord (%f,%f,%f)\n",rdata.vs[i+4],rdata.vs[i+5],rdata.vs[i+6]);
      printf("normal (%f,%f,%f)\n",rdata.vs[i+7],rdata.vs[i+8],rdata.vs[i+9]);
    }
  );
  window->draw_vertices=rdata.size*4;
  Window::vertexdata vertices = window->addVertexData(rdata.vs,rdata.size,10);
  window->applyVertexData(vertices,"coord",3);
  window->applyVertexData(vertices,"normal",3,7);
  DEBUGP(MAIN_DEBUG,"vertex count %i\n",rdata.size);
  
  window->mainLoop();
  
  delete window;
  
  SDL_Quit();
  
  return EXIT_SUCCESS;
}

