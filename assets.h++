#ifndef ASSETS_H
#define ASSETS_H
#include "shared.h++"

namespace assets{
  void setroot(std::filesystem::path newroot);
  char *getasset(std::filesystem::path path);
}
#endif