#ifndef CHUNK_H
#define CHUNK_H
#include "shared.h++"

#include <exception>
#include <stddef.h>

class chunk_exception: public std::exception{
public:
  explicit chunk_exception(const char* message,const int i);
  virtual ~chunk_exception() noexcept;
  virtual const char* what() const noexcept;

  const char* msg;
  const int _idx;
};

template<typename T> class line;
template<typename T> class plane;

template<typename T>
class chunk{
public:
	size_t xsize,ysize,zsize;

private:
	T *data;

public:
	chunk(size_t xsize,size_t ysize,size_t zsize);

	plane<T> &operator[](size_t x);
};

template<typename T>
class line{
public:
  size_t zsize;

private:
  T *data;
  line(T *data,size_t zsize);

public:
	T &operator[](size_t z);

	friend line<T> &plane<T>::operator[](size_t);
};

template<typename T>
class plane{
public:
  size_t ysize,zsize;

private:
  T *data;
  plane(T *data,size_t ysize,size_t zsize);

public:
	line<T> &operator[](size_t y);

	friend plane<T> &chunk<T>::operator[](size_t);
};

template<typename T>
line<T>::line(T *data,size_t zsize):zsize(zsize),data(data){}

template<typename T>
plane<T>::plane(T *data,size_t ysize,size_t zsize):ysize(ysize),zsize(zsize),data(data){}

template<typename T>
chunk<T>::chunk(size_t xsize,size_t ysize,size_t zsize):xsize(xsize),ysize(ysize),zsize(zsize),data((T*)malloc(xsize*ysize*zsize*sizeof(T))){}

template<typename T>
T &line<T>::operator[](size_t z){
  if(z>=zsize){
    throw new chunk_exception("z index out of range",z);
  }
  return data[z];
}

template<typename T>
line<T> &plane<T>::operator[](size_t y){
  if(y>=ysize){
    throw new chunk_exception("y index out of range",y);
  }
  line<T> *l=new line(data+(y*zsize),zsize);
  return *l;
}

template<typename T>
plane<T> &chunk<T>::operator[](size_t x){
  if(x>=xsize){
    throw new chunk_exception("x index out of range",x);
  }
  plane<T> *p=new plane(data+(x*ysize*zsize),ysize,zsize);
  return *p;
}
#endif