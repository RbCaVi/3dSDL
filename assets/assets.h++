#ifndef ASSETS_H
#define ASSETS_H
#include "shared.h++"

#include <filesystem>

#ifdef ASSETS_DEBUG
#define _ASSETS_DEBUG 1
#else
#define _ASSETS_DEBUG 0
#endif

namespace assets{
  struct sizeasset{
    char *asset;
    int size; // hopefully enough
  };
  void setroot(std::filesystem::path newroot);
  char *getasset(std::filesystem::path path);
  sizeasset getassetwithsize(std::filesystem::path path);
}
#endif