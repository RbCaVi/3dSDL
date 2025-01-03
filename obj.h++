#include "shared.h++"

#include <string.h>
#include <vector>
#include <cmath>

#include "file.h++"

#ifdef OBJ_DEBUG
#define _OBJ_DEBUG 1
#else
#define _OBJ_DEBUG 0
#endif

class obj_exception: public std::exception{
public:
  explicit obj_exception(const char* message);
  virtual ~obj_exception() noexcept;
  virtual const char* what() const noexcept;

  const char* msg;
};

class obj{
private:
  struct f{
    const obj* parent;
    int *vertindexes;
    int *verttexindexes;
    int *vertnormindexes;
    unsigned int size;
    f(obj* p,int vi[],int vti[],int vni[],unsigned int size);
    ~f();
  };

  struct v{
    const obj* parent;
    float x,y,z,w;

    v(obj* p,float x,float y,float z,float w);

    float *data();
  };

  struct vt{
    const obj* parent;
    float u,v,w;

    vt(obj* p,float u,float v,float w);

    float *data();
  };

  struct vn{
    const obj* parent;
    float x,y,z;

    vn(obj* p,float x,float y,float z);

    float *data();
  };

  typedef f f;
  typedef v v;
  typedef vt vt;
  typedef vn vn;

  std::vector<f*> *faces;
  std::vector<v*> *verts;
  std::vector<vt*> *verttexs;
  std::vector<vn*> *vertnorms;

  void _loadstr(char *source);

  bool loaded;

public:
  struct renderdata{
    int size; // number of points
    float *vs; // packed (stride 10) - vertex 4 floats + uv 3 floats + normal 3 floats

  private:
    renderdata(int size, float *vs):size(size),vs(vs){}

  public:
    ~renderdata(){
      free(vs);
    }

    friend obj;
  };

  obj();

  void load(std::filesystem::path path);

  void loadstr(const char *path);
  void loadasset(const char *path);

  renderdata makeRenderData();

  ~obj();
};