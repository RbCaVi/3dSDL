#include "shared.h++"

#include <cstdlib>
#include <string.h>
#include <string>
#include <list>

#include "file.h++"

class obj{
private:
  struct f{
    const obj* parent;
    int *vertindexes;
    int *verttexindexes;
    int *vertnormindexes;
    f(obj* p,int vi[],int vti[],int vni[]):parent(p),vertindexes(vi),verttexindexes(vti),vertnormindexes(vni){}
  };

  struct v{
    const obj* parent;
    float x,y,z,w;

    v(obj* p,float x,float y,float z,float w=1):parent(p),x(x),y(y),z(z),w(w){}
  };

  struct vt{
    const obj* parent;
    float u,v,w;

    vt(obj* p,float u,float v=0,float w=0):parent(p),u(u),v(v),w(w){}
  };

  struct vn{
    const obj* parent;
    float x,y,z;

    vn(obj* p,float x,float y,float z):parent(p),x(x),y(y),z(z){}
  };

  typedef f f;
  typedef v v;
  typedef vt vt;
  typedef vn vn;

  std::list<f> *faces;
  std::list<v> *verts;
  std::list<vt> *verttexs;
  std::list<vn> *vertnorms;

public:
  obj(){
    faces=new std::list<f>();
    verts=new std::list<v>();
    verttexs=new std::list<vt>();
    vertnorms=new std::list<vn>();
  }

  void load(std::filesystem::path path){
    printf("%s\n",path.string().c_str());
    char *data=pathtobuf(path);
    char *start=data;

    // for each line
    while (true){
      // check if it starts with:
      // f
      // v
      // vn
      // vt
      int len2=0; // until newline
      while(start[len2]!='\n'&&start[len2]!='\0'){
        //printf("2 %c %i\n",start[len2],len2);
        len2++;
      }
      bool last=false;
      if(start[len2]=='\0'){
        last=true;
      }
      start[len2]='\0';

      // replace # with null
      // simple trick
      // don't need to revert because the data is being deleted at the end anyway
      int i3=0;
      while(start[i3]!='\n'&&start[i3]!='\0'){
        //printf("3 %c %i\n",start[i3],i3);
        if(start[i3]=='#'){
          start[i3]='\0';
          break;
        }
        i3++;
      }

      int len1=0;
      while(start[len1]!=' '&&start[len1]!='\0'){
        //printf("1 %c %i\n",start[len1],len1);
        len1++;
      }
      if(start[len1]=='\0'){
        start+=len2+1;
        if(last){
          break;
        }
        continue;
      }
      char *s=(char*)malloc(len1);
      strncpy(s,start,len1);
      //printf("%s\n",s);
      if(strcmp(s,"v")==0){
        printf("s == 'v'\n");
        int i;
        float *f=(float*)malloc(4*sizeof(float));
        f[3]=1.0;
        char* ss=s;
        
        for((i=0),(f[i]=strtof(s,&ss));i<4&&s!=ss;f[++i]=strtof(s,&ss)){
          s=ss;
        }
        if(i<3){
          printf("Error in 'v': Not enough coords\n")
        }
      }
      start+=len2+1;
      if(last){
        break;
      }
    }
    free(data);
  }
};

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
  std::filesystem::path file=std::filesystem::path("./assets")/"cat_v2.obj";
  obj *o=new obj();
  o->load(file);
}
