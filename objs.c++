#include "shared.h++"

#include "assets/assets.h++"
#include "objs.h++"

objs_exception::objs_exception(const char *message): msg(message) {}

objs_exception::~objs_exception() noexcept {}

const char *objs_exception::what() const noexcept {
   return msg;
}

objs::f::f(objs* p,int vi[],int vti[],int vni[],unsigned int size):parent(p),
  vertindexes(vi),verttexindexes(vti),vertnormindexes(vni),
  size(size){}

objs::f::~f(){
  DEBUGP(OBJ_DEBUG,"f deleted\n");
  free(vertindexes);
  free(verttexindexes);
  free(vertnormindexes);
}

objs::v::v(objs* p,float x,float y,float z,float w=1):parent(p),x(x),y(y),z(z),w(w){}

float *objs::v::data(){
  float* d=(float*)malloc(4*sizeof(float));
  d[0]=x;
  d[1]=y;
  d[2]=z;
  d[3]=w;
  return d;
}

objs::vt::vt(objs* p,float u,float v=0,float w=0):parent(p),u(u),v(v),w(w){}

float *objs::vt::data(){
  float* d=(float*)malloc(3*sizeof(float));
  d[0]=u;
  d[1]=v;
  d[2]=w;
  return d;
}

objs::vn::vn(objs* p,float x,float y,float z):parent(p),x(x),y(y),z(z){
  float di=1.0/sqrt(x*x+y*y+z*z);
  x*=di;
  y*=di;
  z*=di;
}

float *objs::vn::data(){
  float* d=(float*)malloc(3*sizeof(float));
  d[0]=x;
  d[1]=y;
  d[2]=z;
  return d;
}

template<typename T,int blocksize = 4>
class dynamiclist{
private:
  int length;
  int alloclength;

public:
  T *items;
  dynamiclist():
    length(0),alloclength(blocksize),
    items((T*)malloc(alloclength*sizeof(T))){}

  operator T *(){
    return items;
  }

  void append(T item){
    if(length==alloclength){
      alloclength+=blocksize;
      items=(T*)realloc(items,alloclength*sizeof(T));
    }
    items[length]=item;
    length++;
  }

  // add count items from newitems to the list
  void append(T *newitems,int count){
    if(length+count>alloclength){
      while(length+count>alloclength){
        alloclength+=blocksize;
      }
      items=(T*)realloc(items,alloclength*sizeof(T));
    }
    int i;
    for(i=0;i<count;i++){
      items[length+i]=newitems[i];
    }
    length+=count;
  }

  int getLength(){
    return length;
  }

  ~dynamiclist(){
    // you can free your own arrays
    //free(items);
  }
};

objs::objs(){
  faces=new std::vector<f*>();
  verts=new std::vector<v*>();
  verttexs=new std::vector<vt*>();
  vertnorms=new std::vector<vn*>();
  facestartidxs=new std::vector<int>();

  verts->push_back(new v(0,0,0,0));
  verttexs->push_back(new vt(0,0,0));
  vertnorms->push_back(new vn(0,0,0,0));
  facestartidxs->push_back(0);
}

void objs::load(std::filesystem::path path){
  DEBUGP(OBJ_DEBUG,"loading %s\n",path.string().c_str());
  char *s=pathtobuf(path);
  _loadstr(s);
  free(s);
}

void objs::loadasset(const char *asset){
  char *s=assets::getasset(asset);
  _loadstr(s);
  free(s);
}

void objs::loadstr(const char *source){
  int len=strlen(source);
  char *s=(char*)malloc(len+1);
  strcpy(s,source);
  _loadstr(s);
  free(s);
}

void objs::_loadstr(char *source){
  // the -1 is to account for the extra 'null' vertex at the start
  int voffset=verts->size()-1;
  int vtoffset=verttexs->size()-1;
  int vnoffset=vertnorms->size()-1;

  char *data=source;
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
      DEBUGP(OBJ_DEBUG,"2 %c %i\n",start[len2],len2);
      len2++;
    }
    bool last=false;
    if(start[len2]=='\0'){
      last=true;
    }
    start[len2]='\0';
    DEBUGP(OBJ_DEBUG,"start: %s\n",start);

    // replace # with null
    // simple trick
    // don't need to revert because the data is being deleted at the end anyway
    int i3=0;
    while(start[i3]!='\n'&&start[i3]!='\0'){
      DEBUGP(OBJ_DEBUG,"3 %c %i\n",start[i3],i3);
      if(start[i3]=='#'){
        start[i3]='\0';
        break;
      }
      i3++;
    }

    int len1=0;
    while(start[len1]!=' '&&start[len1]!='\0'){
      DEBUGP(OBJ_DEBUG,"1 %c %i\n",start[len1],len1);
      len1++;
    }
    if(start[len1]=='\0'){
      start+=len2+1;
      if(last){
        break;
      }
      continue;
    }
    char *s=(char*)malloc(len1+1);
    s[len1]='\0';
    strncpy(s,start,len1);
    DEBUGP(OBJ_DEBUG,"%s\n",s);
    if(strcmp(s,"v")==0){
      DEBUGP(OBJ_DEBUG,"s == 'v'\n");
      int i;
      float *f=(float*)malloc(4*sizeof(float));
      char* str=start+len1;
      char* ss=str;
      
      for((i=0),(f[i]=strtof(str,&ss));i<4&&str!=ss;f[++i]=strtof(str,&ss)){
        DEBUGP(OBJ_DEBUG,"%i %f\n",i,f[i]);
        str=ss;
      }
      if(i<3){
        throw objs_exception("Error in 'v': Not enough coords\n");
      }
      if(i<4){
        f[3]=1.0;
      }
      DEBUGP(OBJ_DEBUG,"v (%f,%f,%f,%f)\n",f[0],f[1],f[2],f[3]);
      v *vert=new v(this,f[0],f[1],f[2],f[3]);
      verts->push_back(vert);
      free(f);
    }else if(strcmp(s,"vt")==0){
      DEBUGP(OBJ_DEBUG,"s == 'vt'\n");
      int i;
      float *f=(float*)malloc(3*sizeof(float));
      f[1]=0.0;
      f[2]=0.0;
      char* str=start+len1;
      char* ss=str;
      
      for((i=0),(f[i]=strtof(str,&ss));i<4&&str!=ss;f[++i]=strtof(str,&ss)){
        DEBUGP(OBJ_DEBUG,"%i %f\n",i,f[i]);
        str=ss;
      }
      if(i<1){
        throw objs_exception("Error in 'vt': Not enough coords\n");
      }
      vt *verttex=new vt(this,f[0],f[1],f[2]);
      verttexs->push_back(verttex);
      free(f);
    }else if(strcmp(s,"vn")==0){
      DEBUGP(OBJ_DEBUG,"s == 'vn'\n");
      int i;
      float *f=(float*)malloc(3*sizeof(float));
      char* str=start+len1;
      char* ss=str;
      
      for((i=0),(f[i]=strtof(str,&ss));i<4&&str!=ss;f[++i]=strtof(str,&ss)){
        DEBUGP(OBJ_DEBUG,"%i %f\n",i,f[i]);
        str=ss;
      }
      if(i<3){
        throw objs_exception("Error in 'vn': Not enough coords\n");
      }
      vn *vertnorm=new vn(this,f[0],f[1],f[2]);
      vertnorms->push_back(vertnorm);
      free(f);
    }else if(strcmp(s,"f")==0){
      DEBUGP(OBJ_DEBUG,"s == 'f'\n");
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
          throw objs_exception("No vertex index");
          val=0;
        }
        vil.push_back(val+voffset);
        slash=strchr(str,'/');
        if(slash==NULL){
          str=space+1;
          vtil.push_back(0);
          vnil.push_back(0);
          continue;
        }
        str=slash+1;
        val=strtol(str,&ss,0);
        if(str==ss){
          val=0;
        }
        vtil.push_back(val+vtoffset);
        slash=strchr(str,'/');
        if(slash==NULL){
          str=space+1;
          vnil.push_back(0);
          continue;
        }
        str=slash+1;
        val=strtol(str,&ss,0);
        if(str==ss){
          val=0;
        }
        vnil.push_back(val+vnoffset);
        str=space+1;
      }
      unsigned int i;
      int* vi=(int*)malloc(vil.size()*sizeof(int));
      auto vil_front = vil.begin();
      for (i=0;i<vil.size();i++){
        DEBUGP(OBJ_DEBUG,"%i ",*vil_front);
        vi[i]=*vil_front;
        std::advance(vil_front,1);
      }
      DEBUGP(OBJ_DEBUG,"\n");
      int* vti=(int*)malloc(vtil.size()*sizeof(int));
      auto vtil_front = vtil.begin();
      for (i=0;i<vtil.size();i++){
        DEBUGP(OBJ_DEBUG,"%i ",*vtil_front);
        vti[i]=*vtil_front;
        std::advance(vtil_front,1);
      }
      DEBUGP(OBJ_DEBUG,"\n");
      int* vni=(int*)malloc(vnil.size()*sizeof(int));
      auto vnil_front = vnil.begin();
      for (i=0;i<vnil.size();i++){
        DEBUGP(OBJ_DEBUG,"%i ",*vnil_front);
        vni[i]=*vnil_front;
        std::advance(vnil_front,1);
      }
      DEBUGP(OBJ_DEBUG,"\n");
      f *face=new f(this,vi,vti,vni,vil.size());
      faces->push_back(face);
    }
    free(s);
    start+=len2+1;
    if(last){
      break;
    }
  }
  DEBUGR(OBJ_DEBUG,
    for (auto const &v : *verts) printf("v %f %f %f %f\n",v->x,v->y,v->z,v->w);
    printf("\n");
    for (auto const &v : *verttexs) printf("vt %f %f %f\n",v->u,v->v,v->w);
    printf("\n");
    for (auto const &v : *vertnorms) printf("vn %f %f %f\n",v->x,v->y,v->z);
    printf("\n");
    for (auto const &v : *faces) printf("f %i\n",v->size);
    printf("\n");
  );
  facestartidxs->push_back(faces->size());
}

objs::renderdata::renderdata(int numberofshapes,int *starts, int *lengths, float *vs):numberofshapes(numberofshapes),starts(starts),lengths(lengths),vs(vs),totalsize(0){
  int i;
  for(i=0;i<numberofshapes;i++){
    totalsize+=lengths[i];
  }
}

objs::renderdata objs::makeRenderData(){
  unsigned int i;
  DEBUGR(OBJ_DEBUG,
    auto faces_front = faces->begin();
    
    for (i=0;i<faces->size();i++){
      printf("%lu ",(unsigned long)(*faces_front));
      auto const &face=*faces_front;
      printf("face1 - %lu\n",(unsigned long)face);
      printf("%i\n",face->size);
      printf("%i\n",face->vertindexes[0]);
      std::advance(faces_front,1);
    }
  );

  int size=0;
  unsigned int startsi=1;
  int fi=0;
  dynamiclist<int> starts,lengths;
  dynamiclist<float> vs;
  starts.append(0);
  for(auto const &face:*faces){
    fi++;
    DEBUGP(OBJ_DEBUG,"face - %lu ",(unsigned long)face);
    DEBUGP(OBJ_DEBUG,"%i ",face->size);
    DEBUGP(OBJ_DEBUG,"%i\n",face->vertindexes[0]);
    if(face->size!=3){
      throw objs_exception("There is a non triangular face\n");
    }
    for(i=0;i<face->size;i++){
      DEBUGP(OBJ_DEBUG,"vertex indices: %i %i %i\n",face->vertindexes[i],face->verttexindexes[i],face->vertnormindexes[i]);
      DEBUGP(OBJ_DEBUG,"vertex list lengths: %lu %lu %lu\n",verts->size(),verttexs->size(),vertnorms->size());
      float *vdata=(*verts)[face->vertindexes[i]]->data();
      float *vtdata=(*verttexs)[face->verttexindexes[i]]->data();
      float *vndata=(*vertnorms)[face->vertnormindexes[i]]->data();
      vs.append(vdata,4);
      vs.append(vtdata,3);
      vs.append(vndata,3);
      free(vdata);
      free(vtdata);
      free(vndata);
    }
    size+=face->size;
    if(fi==(*facestartidxs)[startsi]){
      if(startsi<facestartidxs->size()-1){
        starts.append(vs.getLength()/4);
      }
      lengths.append(size);
      size=0;
      startsi++;
    }
  }
  renderdata r(facestartidxs->size()-1,starts,lengths,vs);
  return r;
}

int objs::renderdata::size(){
  return totalsize;
}

objs::~objs(){
  DEBUGP(OBJ_DEBUG,"objs deleted\n");
  int i;
  for(i=faces->size()-1;i>0;i--){
    delete (f*)((*faces)[i]);
  }
  for(i=verts->size()-1;i>0;i--){
    delete (v*)((*verts)[i]);
  }
  for(i=verttexs->size()-1;i>0;i--){
    delete (vt*)((*verttexs)[i]);
  }
  for(i=vertnorms->size()-1;i>0;i--){
    delete (vn*)((*vertnorms)[i]);
  }
  DEBUGP(OBJ_DEBUG,"%li %li %li %li\n",faces->size(),verts->size(),verttexs->size(),vertnorms->size());
  delete faces;
  delete verts;
  delete verttexs;
  delete vertnorms;
  delete facestartidxs;
}