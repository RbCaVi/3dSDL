#ifndef OBJMAIN_H
#define OBJMAIN_H

#include "shared.h++"

#include "matrix.h++"
#include "window.h++"

#ifdef MAIN_DEBUG
#define _MAIN_DEBUG 1
#else
#define _MAIN_DEBUG 0
#endif

#ifdef MAIN_OPT_DEBUG
#define _MAIN_OPT_DEBUG 1
#else
#define _MAIN_OPT_DEBUG 0
#endif

Window* window;

struct sdata{
  //matrix4x4 trans;
  //matrix4x4 rot;
  matrix4x4 model;
  matrix4x4 view;
  matrix4x4 projection;
  signed char rl=0;
  signed char ud=0;
  //float horizAngle;
  //float vertAngle;
  float forward=0;
};
typedef sdata sdata;

struct argdata{
  bool error;
  bool capture;
  int framecount;
};
typedef argdata argdata;

#endif
