#include "shared.h++"

#include <filesystem>

#include "file.h++"
#include "assets.h++"

std::filesystem::path root;

void assets::setroot(std::filesystem::path newroot){
  root=newroot;
}

char *assets::getasset(std::filesystem::path path){
  std::filesystem::path rootpath=root/path;
  return pathtobuf(rootpath);
}