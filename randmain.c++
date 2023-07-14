#include "shared.h++"

#include <stddef.h>

#include "random.h++"

class chunk_exception: public std::exception{
public:
  explicit chunk_exception(const char* message,const int i);
  virtual ~chunk_exception() noexcept;
  virtual const char* what() const noexcept;

  const char* msg;
  const int _idx;
};

chunk_exception::chunk_exception(const char* message,const int i): msg(message),_idx(i) {}

chunk_exception::~chunk_exception() noexcept {}

const char* chunk_exception::what() const noexcept {
   return msg;
}

template<typename T>
class chunk{
public:
	size_t xsize,ysize,zsize;

private:
	T *data;

	class line{
	public:
	  size_t zsize;

	private:
	  T *data;

	public:
	  line(T *data,size_t zsize):zsize(zsize),data(data){}

  	T &operator[](size_t z){
  		if(z>=zsize){
  			throw new chunk_exception("z index out of range",z);
  		}
			return data[z];
		}
	};

	class plane{
	public:
	  size_t ysize,zsize;

	private:
	  T *data;

	public:
	  plane(T *data,size_t ysize,size_t zsize):ysize(ysize),zsize(zsize),data(data){}

  	line &operator[](size_t y){
  		if(y>=ysize){
  			throw new chunk_exception("y index out of range",y);
  		}
			line *l=new line(data+(y*zsize),zsize);
			return *l;
		}
	};

public:
	chunk(size_t xsize,size_t ysize,size_t zsize):xsize(xsize),ysize(ysize),zsize(zsize),data((bool*)malloc(xsize*ysize*zsize*sizeof(T))){}

	plane &operator[](size_t x){
		if(x>=xsize){
			throw new chunk_exception("x index out of range",x);
		}
		plane *p=new plane(data+(x*ysize*zsize),ysize,zsize);
		return *p;
	}
};

int main(){
	Rand *r=new randchance(0.2);
	chunk<bool> c(5,5,5);
	size_t i,j,k;
	for(i=0;i<5;i++){
		for(j=0;j<5;j++){
			for(k=0;k<5;k++){
				c[i][j][k]=(*r)();
			}
		}
	}

	for(i=0;i<5;i++){
		for(j=0;j<5;j++){
			for(k=0;k<5;k++){
				printf("%i",c[i][j][k]);
			}
			printf("\n");
		}
		printf("\n");
	}
	return 0;
}