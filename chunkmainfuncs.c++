#include "shared.h++"

#include <atomic>

#include <SDL2/SDL.h>

#include "chunkmain.h++"
#include "window.h++"

struct closedata{
  void (*close_action)(void*);
  void *data;
  struct closedata *previous; // just in case two handlers are called
  closedata(void (*close_action)(void*), void *data, struct closedata *previous):
    close_action(close_action),
    data(data),
    previous(previous){}
};
std::atomic<closedata*> cdata;

struct intdata{
  Window *w;
  int sig;
  intdata(Window *w, int sig):w(w),sig(sig){}
};


void close_window(void* data){
  intdata *d=(intdata*)data;
  printf("Caught signal %d\n",d->sig);
  if(d->w!=NULL){
    d->w->closed=true;
  }
}

void handlesigint(int signal){
  intdata *id=new intdata(window,signal);
  closedata *cd=new closedata(close_window,id,cdata.load());
  cdata=cd;
}

void onframe(Window *window, void *d){
  sdata *data=(sdata*)d;
  //data->vertAngle+=data->ud;
  //data->horizAngle+=data->rl;
  data->view=getRotation(1,2,-0.02*data->ud)*data->view;
  data->view=getRotation(0,2,-0.02*data->rl)*data->view;
  data->view=getTranslation(0,0,1*data->forward)*data->view;
  data->view=getTranslation(0,-1*data->mud,0)*data->view;
  data->view=getTranslation(-1*data->mrl,0,0)*data->view;
  //data->rot=getRotation(1,2,0.1*data->ud)*data->rot;
  //data->rot=getRotation(0,2,0.1*data->rl)*data->rot;
  //data->trans=getTranslation(1*data->rl,0,0)*data->trans;
  //data->trans=getTranslation(0,1*data->ud,0)*data->trans;
  //data->model=data->trans*data->rot;
  //window->addUniformMat4x4("model",data->model);
  window->addUniformMat4x4("view",data->view);
  window->addUniformMat4x4("mvp",data->projection*data->view*data->model);
  
  closedata *cd=cdata.load();
  while(cd!=NULL){
    cd->close_action(cd->data);
    cd=cd->previous;
  }
}

void draw(Window *window, void *d){
  sdata *data=(sdata*)d;
  chunk<bool> *c=data->grid;
  int i,j,k;
  for(i=0;i<5;i++){
    for(j=0;j<5;j++){
      for(k=0;k<5;k++){
        if((*c)[i][j][k]){
          float *vec=(float*)malloc(3*sizeof(float));
          //vec[0]=i*30;
          //vec[1]=j*100;
          //vec[2]=k*60;
          vec[0]=i;
          vec[1]=j;
          vec[2]=k;
          window->addUniformVec3("position",vec);
          glDrawArrays(GL_TRIANGLES, 0, window->draw_vertices);
        }
      }
    }
  }
}

void onkeydown(Window *w, SDL_Keysym ks, void *d){
  IGNORE(w);
  sdata *data=(sdata*)d;
  switch(ks.sym){
   case SDLK_SPACE:
    data->forward=1;
    break;
   case SDLK_BACKSPACE:
    data->forward=-1;
    break;
   case SDLK_UP:
    data->ud=1;
    break;
   case SDLK_DOWN:
    data->ud=-1;
    break;
   case SDLK_LEFT:
    data->rl=-1;
    break;
   case SDLK_RIGHT:
    data->rl=1;
    break;
   case SDLK_w:
    data->mud=1;
    break;
   case SDLK_s:
    data->mud=-1;
    break;
   case SDLK_a:
    data->mrl=-1;
    break;
   case SDLK_d:
    data->mrl=1;
    break;
  }
}

void onkeyup(Window *w, SDL_Keysym ks, void *d){
  IGNORE(w);
  sdata *data=(sdata*)d;
  switch(ks.sym){
   case SDLK_SPACE:
    if(data->forward>0){
      data->forward=0;
    }
    break;
   case SDLK_BACKSPACE:
    if(data->forward<0){
      data->forward=0;
    }
    break;
   case SDLK_UP:
    if(data->ud>0){
      data->ud=0;
    }
    break;
   case SDLK_DOWN:
    if(data->ud<0){
      data->ud=0;
    }
    break;
   case SDLK_LEFT:
    if(data->rl<0){
      data->rl=0;
    }
    break;
   case SDLK_RIGHT:
    if(data->rl>0){
      data->rl=0;
    }
    break;
   case SDLK_w:
    if(data->mud>0){
      data->mud=0;
    }
    break;
   case SDLK_s:
    if(data->mud<0){
      data->mud=0;
    }
    break;
   case SDLK_a:
    if(data->mrl<0){
      data->mrl=0;
    }
    break;
   case SDLK_d:
    if(data->mrl>0){
      data->mrl=0;
    }
    break;
  }
}

bool isopt(const char* opt,void*){
  return opt[0]=='-' && opt[1]=='-';
}

int optargs(const char* arg,void*){
  if(strcmp(arg,"--frames")==0){
    return 1;
  }
  return 0;
}

void addopt(const char* opt,int,const char** args,void* d){
  argdata *data=(argdata*)d;
  // capture frames record input-file 
  DEBUGP(MAIN_OPT_DEBUG,"%s\n",opt);
  if(strcmp(opt,"--capture")==0){
    data->capture=true;
    return;
  }else if(strcmp(opt,"--frames")==0){
    data->framecount=atoi(args[0]);
    return;
  }
  printf("Did not expect %s\n",opt);
  data->error=true;
}

void addarg(const char* arg,void* d){
  argdata *data=(argdata*)d;
  printf("Did not expect %s\n",arg);
  data->error=true;
}
