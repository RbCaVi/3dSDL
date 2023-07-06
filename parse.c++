#include "shared.h++"

#include <cstdlib>
#include <string.h>
#include <string>
#include <vector>
#include <cmath>

#include "file.h++"

class obj_exception: public std::exception{
public:
  explicit obj_exception(const char* message);
  virtual ~obj_exception() noexcept;
  virtual const char* what() const noexcept;

  const char* msg;
};

obj_exception::obj_exception(const char* message): msg(message) {}

obj_exception::~obj_exception() noexcept {}

const char* obj_exception::what() const noexcept {
   return msg;
}

class obj{
private:
  struct f{
    const obj* parent;
    int *vertindexes;
    int *verttexindexes;
    int *vertnormindexes;
    int size;
    f(obj* p,int vi[],int vti[],int vni[],int size):parent(p),
    vertindexes(vi),verttexindexes(vti),vertnormindexes(vni),
    size(size){}
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

    vn(obj* p,float x,float y,float z):parent(p),x(x),y(y),z(z){
      float di=1.0/sqrt(x*x+y*y+z*z);
      x*=di;
      y*=di;
      z*=di;
    }
  };

  typedef f f;
  typedef v v;
  typedef vt vt;
  typedef vn vn;

  std::vector<f*> *faces;
  std::vector<v*> *verts;
  std::vector<vt*> *verttexs;
  std::vector<vn*> *vertnorms;

public:
  struct renderdata{
    int size; // number of points
    int *vs; // packed (stride 4)
    int *vts; // packed (stride 3)
    int *vns; // packed (stride 3)
  };

  obj(){
    faces=new std::vector<f*>();
    verts=new std::vector<v*>();
    verttexs=new std::vector<vt*>();
    vertnorms=new std::vector<vn*>();
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
      while(start[len1]!=' '&&start[len1]!='\n'&&start[len1]!='\0'){
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
      DEBUGP("%s\n",s);
      if(strcmp(s,"v")==0){
        DEBUGP("s == 'v'\n");
        int i;
        float *f=(float*)malloc(4*sizeof(float));
        f[3]=1.0;
        char* str=start+len1;
        char* ss=str;
        
        for((i=0),(f[i]=strtof(str,&ss));i<4&&str!=ss;f[++i]=strtof(str,&ss)){
          DEBUGP("%i %f\n",i,f[i]);
          str=ss;
        }
        if(i<3){
          throw obj_exception("Error in 'v': Not enough coords\n");
        }
        v *vert=new v(this,f[0],f[1],f[2],f[3]);
        verts->push_back(vert);
        free(f);
      }else if(strcmp(s,"vt")==0){
        DEBUGP("s == 'vt'\n");
        int i;
        float *f=(float*)malloc(3*sizeof(float));
        f[1]=0.0;
        f[2]=0.0;
        char* str=start+len1;
        char* ss=str;
        
        for((i=0),(f[i]=strtof(str,&ss));i<4&&str!=ss;f[++i]=strtof(str,&ss)){
          DEBUGP("%i %f\n",i,f[i]);
          str=ss;
        }
        if(i<1){
          throw obj_exception("Error in 'vt': Not enough coords\n");
        }
        vt *verttex=new vt(this,f[0],f[1],f[2]);
        verttexs->push_back(verttex);
        free(f);
      }else if(strcmp(s,"vn")==0){
        DEBUGP("s == 'vn'\n");
        int i;
        float *f=(float*)malloc(3*sizeof(float));
        char* str=start+len1;
        char* ss=str;
        
        for((i=0),(f[i]=strtof(str,&ss));i<4&&str!=ss;f[++i]=strtof(str,&ss)){
          DEBUGP("%i %f\n",i,f[i]);
          str=ss;
        }
        if(i<3){
          throw obj_exception("Error in 'vn': Not enough coords\n");
        }
        vn *vertnorm=new vn(this,f[0],f[1],f[2]);
        vertnorms->push_back(vertnorm);
        free(f);
      }else if(strcmp(s,"f")==0){
        DEBUGP("s == 'f'\n");
        char* str=start+len1+1;
        char* ss=str;
        bool last=false;
        // for i=1-3
          // switch next space for null
          // get number (if none, -1)
          // find slash
          // get number (if none, -1)
          // find slash
          // get number (if none, -1)
          // move to after null
        std::vector<int> vil,vtil,vnil;
        while(!last){
          int val;
          char *slash;
          char *space=strchr(str,' ');
          if(space==NULL){
            last=true;
          }else{
            *space='\0';
          }
          val=strtol(str,&ss,0);
          if(str==ss){
            val=-1;
          }
          vil.push_back(val);
          slash=strchr(str,'/');
          if(slash==NULL){
            str=space+1;
            vtil.push_back(-1);
            vnil.push_back(-1);
            continue;
          }
          str=slash+1;
          val=strtol(str,&ss,0);
          if(str==ss){
            val=-1;
          }
          vtil.push_back(val);
          slash=strchr(str,'/');
          if(slash==NULL){
            str=space+1;
            vnil.push_back(-1);
            continue;
          }
          str=slash+1;
          val=strtol(str,&ss,0);
          if(str==ss){
            val=-1;
          }
          vnil.push_back(val);
          str=space+1;
        }
        unsigned int i;
        int* vi=(int*)malloc(vil.size()*sizeof(int));
        auto vil_front = vil.begin();
        for (i=0;i<vil.size();i++){
          DEBUGP("%i ",*vil_front);
          vi[i]=*vil_front;
          std::advance(vil_front,1);
        }
        DEBUGP("\n");
        int* vti=(int*)malloc(vtil.size()*sizeof(int));
        auto vtil_front = vtil.begin();
        for (i=0;i<vtil.size();i++){
          DEBUGP("%i ",*vtil_front);
          vti[i]=*vtil_front;
          std::advance(vtil_front,1);
        }
        DEBUGP("\n");
        int* vni=(int*)malloc(vnil.size()*sizeof(int));
        auto vnil_front = vnil.begin();
        for (i=0;i<vnil.size();i++){
          DEBUGP("%i ",*vnil_front);
          vni[i]=*vnil_front;
          std::advance(vnil_front,1);
        }
        DEBUGP("\n");
        f *face=new f(this,vi,vti,vni,vil.size());
        faces->push_back(face);
      }
      start+=len2+1;
      if(last){
        break;
      }
    }
    DEBUGR(
      for (auto const &v : *verts) printf("v %f %f %f %f\n",*v.x,*v.y,*v.z,*v.w);
      printf("\n");
      for (auto const &v : *verttexs) printf("vt %f %f %f\n",*v.u,*v.v,*v.w);
      printf("\n");
      for (auto const &v : *vertnorms) printf("vn %f %f %f\n",*v.x,*v.y,*v.z);
      printf("\n");
      for (auto const &v : *faces) printf("f %i\n",*v.size);
      printf("\n");
    )
    free(data);
  }

  renderdata *makeRenderData(){
    int i;
    DEBUGR(
      auto faces_front = faces->begin();
      
      for (i=0;i<faces->size();i++){
        DEBUGP("%i ",*faces_front);
        auto const &face=*faces_front;
        printf("face1 - %i\n",face);
        printf("%i\n",face->size);
        printf("%i\n",face->vertindexes[0]);
        std::advance(faces_front,1);
      }
    )

    vector<float> vs,vts,vns;
    for(auto const &face:*faces){
      DEBUGP("face - %i ",face);
      DEBUGP("%i ",face->size);
      DEBUGP("%i\n",face->vertindexes[0]);
      if(face->size!=3){
        throw obj_exception("There is a non triangular face\n");
      }
      vs.append(face.vertindexes,3);
      for(i=0;i<face.size;i++){
        vs.push_back([face.vertindexes[i]])
      }
    }
    return NULL;
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
  std::filesystem::path file=std::filesystem::path(".")/"assets"/"cat_v2.obj";
  obj *o=new obj();
  o->load(file);
  obj::renderdata *r=o->makeRenderData();
}
