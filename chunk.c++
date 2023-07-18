#include "shared.h++"

#include <stddef.h>
#include <stdlib.h>

#include "chunk.h++"

chunk_exception::chunk_exception(const char* message,const int i): msg(message),_idx(i) {}

chunk_exception::~chunk_exception() noexcept {}

const char* chunk_exception::what() const noexcept {
   return msg;
}

template<typename T>
line<T>::line(T *data,size_t zsize):zsize(zsize),data(data){}

template<typename T>
T &line<T>::operator[](size_t z){
	if(z>=zsize){
		throw new chunk_exception("z index out of range",z);
	}
	return data[z];
}


template<typename T>
plane<T>::plane(T *data,size_t ysize,size_t zsize):ysize(ysize),zsize(zsize),data(data){}

template<typename T>
line<T> &plane<T>::operator[](size_t y){
	if(y>=ysize){
		throw new chunk_exception("y index out of range",y);
	}
	line<T> *l=new line(data+(y*zsize),zsize);
	return *l;
}

template<typename T>
chunk<T>::chunk(size_t xsize,size_t ysize,size_t zsize):xsize(xsize),ysize(ysize),zsize(zsize),data((bool*)malloc(xsize*ysize*zsize*sizeof(T))){}

template<typename T>
plane<T> &chunk<T>::operator[](size_t x){
	if(x>=xsize){
		throw new chunk_exception("x index out of range",x);
	}
	plane<T> *p=new plane(data+(x*ysize*zsize),ysize,zsize);
	return *p;
}