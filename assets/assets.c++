#include "shared.h++"

#include "file.h++"
#include "assets/assets.h++"

std::filesystem::path root;

void assets::setroot(std::filesystem::path newroot){
  root=newroot;
}

char *assets::getasset(std::filesystem::path path){
  std::filesystem::path rootpath=root/path;
  return pathtobuf(rootpath);
}

assets::sizeasset assets::getassetwithsize(std::filesystem::path path){
  std::filesystem::path rootpath=root/path;
  int size;
  char *buf = pathtobufsize(rootpath,&size);
  return {buf,size};
}