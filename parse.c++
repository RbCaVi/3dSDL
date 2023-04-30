#include <cstdlib>
#include <string.h>
#include <string>

float *getFloats(char* s,char** sEnd,int n){
  int i;
  float *f=(float*)malloc(n*sizeof(float));
  char* ss=s;
  
  for((i=0),(f[i]=strtof(s,&ss));i<n&&s!=ss;f[++i]=strtof(s,&ss)){
    s=ss;
  }
  
  if(sEnd!=NULL){
    *sEnd=ss;
  }
  
  return f;
}

int main(){
  char* s=(char*)malloc(strlen("1234 5241 5.23 6")+1);
  strcpy(s,"1234 5241 5.23 6");
  
  int i;
  
  int n=3;
  float *fs=getFloats(s,NULL,n);
  for(i=0;i<n;i++){
    printf("%f ",fs[i]);
  }
  printf("\n");
}
