#include "main.h++"

void rotate(Window *window, void *d){
  sdata *data=(sdata*)d;
  data->model=getRotation(1,2,0.01)*data->model;
  window->addUniformMat4x4("model",data->model);
}

bool isopt(const char* opt,void*){
  return opt[0]=='-' && opt[1]=='-';
}

int optargs(const char*,void*){
  return 0;
}

void addopt(const char* opt,int,const char**,void* d){
  // capture record input-file 
  printf("%s",opt);
}

void addarg(const char* arg,void* d){
  optdata *data=(optdata*)d;
  printf("Did not expect %s\n",arg);
  data->error=true;
}
