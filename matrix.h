#ifndef MATRIX_H
#define MATRIX_H

#include "shared.h"

struct matrix4x4{
  float contents[16];
  float &operator [](int i){
    return contents[i];
  }
};
typedef matrix4x4 matrix4x4;

void setPerspective(matrix4x4 &matrix, float fovyInDegrees, float aspectRatio, float znear, float zfar);
void setFrustrum(matrix4x4 &matrix, float left, float right, float bottom, float top, float znear, float zfar);

#endif
