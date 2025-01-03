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

class objs_exception: public std::exception{
public:
  explicit objs_exception(const char* message);
  virtual ~objs_exception() noexcept;
  virtual const char* what() const noexcept;

  const char* msg;
};

class objs{
private:
  struct f{
    const objs* parent;
    int *vertindexes;
    int *verttexindexes;
    int *vertnormindexes;
    unsigned int size;
    f(objs* p,int vi[],int vti[],int vni[],unsigned int size);
    ~f();
  };

  struct v{
    const objs* parent;
    float x,y,z,w;

    v(objs* p,float x,float y,float z,float w);

    float *data();
  };

  struct vt{
    const objs* parent;
    float u,v,w;

    vt(objs* p,float u,float v,float w);

    float *data();
  };

  struct vn{
    const objs* parent;
    float x,y,z;

    vn(objs* p,float x,float y,float z);

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
  std::vector<int> *facestartidxs;

  void _loadstr(char *source);

public:
  struct renderdata{
  public:
    int numberofshapes;
    int *starts; // start index for each shape
    int *lengths; // number of points in each shape
    float *vs; // packed (stride 10) - vertex 4 floats + uv 3 floats + normal 3 floats
    int totalsize;

  private:
    renderdata(int numberofshapes,int *starts, int *lengths, float *vs);

  public:
    ~renderdata(){
      free(vs);
    }

    int size();

    friend objs;
  };

  objs();

  void load(std::filesystem::path path);

  void loadstr(const char *path);
  void loadasset(const char *path);

  renderdata makeRenderData();

  ~objs();
};