#include "matrix.h"

#include <cmath>
#include <cstdio>

matrix_exception::matrix_exception(const char* message): msg(message) {}

matrix_exception::~matrix_exception() noexcept {}

const char* matrix_exception::what() const noexcept {
   return msg;
}

matrix4x4::matrix4x4(){
  int i;
  for(i=0;i<16;i++){
    this->contents[i]=0;
  }
}

matrix4x4 &matrix4x4::operator=(const matrix4x4 &other){
  int i;
  for(i=0;i<16;i++){
    this->contents[i]=other.contents[i];
  }
  return *this;
}

float &matrix4x4::operator [](int i){
  return contents[i];
}

matrix4x4 &matrix4x4::operator *(int n){
  int i;
  matrix4x4 &m=*this;
  for(i=0;i<16;i++){
    m[i]*=n;
  }
  return m;
}

matrix4x4 &matrix4x4::operator *(matrix4x4 &other){
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


// Matrix will receive the calculated perspective matrix. in column major
// You would have to upload to your shader
// or use glLoadMatrixf if you aren't using shaders.
matrix4x4 &getPerspective(float fovyInDegrees, float aspectRatio, float znear, float zfar){
    float ymax, xmax;
    ymax = znear * tanf(fovyInDegrees * M_PI / 360.0);
    xmax = ymax * aspectRatio;
    return getFrustrum(-xmax, xmax, -ymax, ymax, znear, zfar);
}

matrix4x4 &getFrustrum(float left, float right, float bottom, float top, float znear, float zfar){
  matrix4x4 &matrix=*new matrix4x4();
  float temp, temp2, temp3, temp4;
  temp = 2.0 * znear;
  temp2 = right - left;
  temp3 = top - bottom;
  temp4 = zfar - znear;
  matrix[0] = temp / temp2;
  matrix[1] = 0.0;
  matrix[2] = 0.0;
  matrix[3] = 0.0;
  matrix[4] = 0.0;
  matrix[5] = temp / temp3;
  matrix[6] = 0.0;
  matrix[7] = 0.0;
  matrix[8] = (right + left) / temp2;
  matrix[9] = (top + bottom) / temp3;
  matrix[10] = (-zfar - znear) / temp4;
  matrix[11] = -1.0;
  matrix[12] = 0.0;
  matrix[13] = 0.0;
  matrix[14] = (-temp * zfar) / temp4;
  matrix[15] = 0.0;
  return matrix;
}

matrix4x4 &getIdentity(){
  int i;
  matrix4x4 &m=*new matrix4x4();
  for(i=0;i<4;i++){
    m[i*5]=1;
  }
  return m;
}

matrix4x4 &getRotation(int a1,int a2,float theta){
  if(std::min(a1,a2)<0){
    throw new matrix_exception("invalid plane: a1 or a2 is less than 0");
  }
  if(std::max(a1,a2)>3){
    throw new matrix_exception("invalid plane: a1 or a2 is greater than 3");
  }
  if(a1==a2){
    throw new matrix_exception("invalid plane: a1 is equal to a2");
  }
  matrix4x4 &m=getIdentity();
  float c=cos(theta),s=sin(theta);
  m[a1*5]=c;
  m[a1*4+a2]=s;
  m[a1+a2*4]=-s;
  m[a2*5]=c;
  return m;
}

matrix4x4 &getTranslation(float x,float y,float z){
  matrix4x4 &m=getIdentity();
  m[12]=x;
  m[13]=y;
  m[14]=z;
  return m;
}
