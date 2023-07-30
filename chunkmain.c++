#include "shared.h++"

#include <filesystem>
#include <exception>
#include <signal.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "chunkmain.h++"
#include "shaders.h++"
#include "window.h++"
#include "matrix.h++"
#include "parseargs.h++"
#include "obj.h++"
#include "assets.h++"
#include "random.h++"

#include "chunkmainfuncs.c++"

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
  window=new Window(WIDTH,HEIGHT,__FILE__,adata.capture,adata.framecount);
  window->data=(void*)(&data);
  window->onframe=onframe;
  window->onkeydown=onkeydown;
  window->onkeyup=onkeyup;
  window->draw=draw;
  glEnable(GL_DEPTH_TEST);

  glClearColor (0.0, 0.0, 0.0, 0.0);
  
  char *vertsource=assets::getasset("chunkshader.vert");
  char *fragsource=assets::getasset("objshader.frag");
  window->makeShaderFromSource(vertsource,fragsource);
  //data.rot=getIdentity();
  //data.trans=getIdentity();
  data.model=getIdentity();
  data.view=getTranslation(0,0,-200);
  data.projection=getPerspective(45.0f, (float)HEIGHT/WIDTH, 0.1f, 800.0f);

  obj o;
  //o.loadstr(assets::getasset("cat.obj"));
  o.loadstr(assets::getasset("cube.obj"));
  obj::renderdata* rdata=o.makeRenderData();

  Rand *r=new randchance(0.2);
  chunk<bool> *c=new chunk<bool>(5,5,5);
  size_t i,j,k;
  for(i=0;i<5;i++){
    for(j=0;j<5;j++){
      for(k=0;k<5;k++){
        (*c)[i][j][k]=(*r)();
      }
    }
  }

  data.grid=c;

  window->setUniformMat4x4("model",data.model);
  window->setUniformMat4x4("view",data.view);
  window->setUniformMat4x4("projection",data.projection);
  window->setUniformMat4x4("mvp",data.projection*data.view*data.model);
  
  DEBUGR(MAIN_DEBUG,
    int i;
    for(i=0;i<rdata->size*4;i+=4){
      printf("vertex coord (%f,%f,%f,%f)\n",rdata->vs[i],rdata->vs[i+1],rdata->vs[i+2],rdata->vs[i+3]);
    }
  );
  window->draw_vertices=rdata->size*4;
  window->addVertexData("coord",rdata->vs,rdata->size*4*sizeof(float),3,4*sizeof(float));
  window->addVertexData("normal",rdata->vns,rdata->size*3*sizeof(float),3);
  DEBUGP(MAIN_DEBUG,"vertex count %i\n",rdata->size);
  
  window->mainLoop();
  
  delete window;
  
  SDL_Quit();
  
  return EXIT_SUCCESS;
}

