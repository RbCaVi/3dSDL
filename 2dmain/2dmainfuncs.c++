#include "shared.h++"

#include <atomic>

#include <SDL2/SDL.h>

#include "2dmain.h++"
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
  data->x += 0.02 * data->rl;
  data->y += 0.02 * data->ud;
  window->setUniformFloat("x",data->x);
  window->setUniformFloat("y",data->y);
  
  closedata *cd=cdata.load();
  while(cd!=NULL){
    cd->close_action(cd->data);
    cd=cd->previous;
  }
}

void draw(Window *window, void *d){
  sdata *data=(sdata*)d;
  IGNORE(data);
  IGNORE(window);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void onkeydown(Window *w, SDL_Keysym ks, void *d){
  IGNORE(w);
  sdata *data=(sdata*)d;
  switch(ks.sym){
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
    data->ud=1;
    break;
   case SDLK_s:
    data->ud=-1;
    break;
   case SDLK_a:
    data->rl=-1;
    break;
   case SDLK_d:
    data->rl=1;
    break;
  }
}

void onkeyup(Window *w, SDL_Keysym ks, void *d){
  IGNORE(w);
  sdata *data=(sdata*)d;
  switch(ks.sym){
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
    if(data->ud>0){
      data->ud=0;
    }
    break;
   case SDLK_s:
    if(data->ud<0){
      data->ud=0;
    }
    break;
   case SDLK_a:
    if(data->rl<0){
      data->rl=0;
    }
    break;
   case SDLK_d:
    if(data->rl>0){
      data->rl=0;
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
