#ifndef MAIN_H
#define MAIN_H

#include "shared.h++"

#include "matrix.h++"

struct sdata{
  matrix4x4 model;
  matrix4x4 view;
  matrix4x4 projection;
};
typedef sdata sdata;

struct argdata{
  bool error;
  bool capture;
  int framecount;
};
typedef argdata argdata;

#endif
