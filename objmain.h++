#ifndef OBJMAIN_H
#define OBJMAIN_H

#include "shared.h++"

#include "matrix.h++"
#include "window.h++"

Window* window;

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
