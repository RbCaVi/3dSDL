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

class obj{
private:
  struct f{
    const obj* parent;
    int *vertindexes;
    int *verttexindexes;
    int *vertnormindexes;
    int size;
    f(obj* p,int vi[],int vti[],int vni[],int size);
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

  bool loaded;

public:
  struct renderdata{
    int size; // number of points
    float *vs; // packed (stride 4)
    float *vts; // packed (stride 3)
    float *vns; // packed (stride 3)
  };

  obj();

  void load(std::filesystem::path path);

  renderdata *makeRenderData();
};