#include "shared.h++"

#include <atomic>

#include "main.h++"
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
  data->model=getRotation(1,2,0.01)*data->model;
  window->addUniformMat4x4("model",data->model);
  
  closedata *cd=cdata.load();
  while(cd!=NULL){
    cd->close_action(cd->data);
    cd=cd->previous;
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
  DEBUGP("%s\n",opt);
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
