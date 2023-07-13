#ifndef ASSETS_H
#define ASSETS_H
#include "shared.h++"

#ifdef ASSETS_DEBUG
#define _ASSETS_DEBUG 1
#else
#define _ASSETS_DEBUG 0
#endif

namespace assets{
  void setroot(std::filesystem::path newroot);
  char *getasset(std::filesystem::path path);
}
#endif