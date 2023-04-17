#ifndef MATRIX_H
#define MATRIX_H

#include "shared.h"

struct matrix4x4{ // a 4x4 column major matrix
  float contents[16];
  
  matrix4x4(){}
  
  matrix4x4 &operator=(const matrix4x4 &other){
    int i;
    for(i=0;i<16;i++){
      this->contents[i]=other.contents[i];
    }
    return *this;
  }
  
  float &operator [](int i){
    return contents[i];
  }
  
  matrix4x4 &operator *(int n){
    int i;
    matrix4x4 &m=*this;
    for(i=0;i<16;i++){
      m[i]*=n;
    }
    return m;
  }
  
  matrix4x4 &operator *(matrix4x4 &other){
    int i,j,k;
    matrix4x4 &m=*new matrix4x4();
    for(i=0;i<4;i++){ // row of this
      for(j=0;j<4;j++){ // col of other
        for(k=0;k<4;k++){ // index in row/column
          m[i+4*j]+=((*this)[i+4*k])*(other[4*j+k]);
        }
      }
    }
    return m;
  }
};
typedef matrix4x4 matrix4x4;

void setPerspective(matrix4x4 &matrix, float fovyInDegrees, float aspectRatio, float znear, float zfar);
void setFrustrum(matrix4x4 &matrix, float left, float right, float bottom, float top, float znear, float zfar);

#endif
