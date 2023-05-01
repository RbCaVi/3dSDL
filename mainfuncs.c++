#include "main.h++"

void rotate(Window *window, void *d){
  sdata *data=(sdata*)d;
  data->model=getRotation(1,2,0.01)*data->model;
  window->addUniformMat4x4("model",data->model);
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
  printf("%s",opt);
  if(strcmp(opt,"--capture")==0){
    data->capture=true;
  }else if(strcmp(opt,"--frames")==0){
    data->frames=atoi(args[0]);
  }
}

void addarg(const char* arg,void* d){
  argdata *data=(argdata*)d;
  printf("Did not expect %s\n",arg);
  data->error=true;
}
