#ifndef CHUNKOBJSMAIN_H
#define CHUNKOBJSMAIN_H

#include "shared.h++"

#include "matrix.h++"
#include "window.h++"
#include "chunk.h++"
#include "objs.h++"

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
  signed char rl=0;
  signed char ud=0;
  float x=0;
  float y=0;
};
typedef sdata sdata;

struct argdata{
  bool error;
  bool capture;
  int framecount;

  argdata(bool e,bool c,int f):error(e),capture(c),framecount(f){}
};
typedef argdata argdata;

#endif
