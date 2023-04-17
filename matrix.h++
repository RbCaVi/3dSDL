#ifndef MATRIX_H
#define MATRIX_H

#include "shared.h++"

#include <exception>

class matrix_exception: public std::exception{
public:
  matrix_exception(const char* message);
  ~matrix_exception() noexcept;
  const char* what() const noexcept;
  
  const char* msg;
};

struct matrix4x4{ // a 4x4 column major matrix
  float contents[16];
  
  matrix4x4();
  matrix4x4(const matrix4x4 &other);
  matrix4x4 &operator=(const matrix4x4 &other);
  float &operator [](int i);
  matrix4x4 &operator *(int n);
  matrix4x4 &operator *(matrix4x4 &other);
};
typedef matrix4x4 matrix4x4;

matrix4x4 &getPerspective(float fovyInDegrees, float aspectRatio, float znear, float zfar);
matrix4x4 &getFrustrum(float left, float right, float bottom, float top, float znear, float zfar);
matrix4x4 &getIdentity();
matrix4x4 &getRotation(int a1,int a2,float theta);
matrix4x4 &getTranslation(float x,float y,float z);

#endif
