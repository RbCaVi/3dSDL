#include "shared.h++"

#include <filesystem>
#include <string.h>
#include <stdlib.h>

#include "assets.h++"
#include "file.h++"

extern char *assetsdata;
extern int *assetlengths;
extern char *assetnames;
extern int assetcount;

std::filesystem::path root;

void assets::setroot(std::filesystem::path newroot){
  root=newroot;
}

char *assets::getasset(std::filesystem::path stdpath){
  const char *path=stdpath.c_str();
  int i;
  for(i=0;i<assetcount;i++){
    int size=*((&assetlengths)[i]);
    char *name=(&assetnames)[i];
    DEBUGP(ASSETS_DEBUG,"index: %i\nsize: %i\nname: %s\n\n",i,size,path);
    DEBUGP(ASSETS_DEBUG,"Checking asset %s\n",name);
    if(strcmp(path,name)==0){
      char *blob=(&assetsdata)[i];
      char *s=(char*)malloc(size+1);
      strncpy(s,blob,size);
      s[size]='\0';
      DEBUGP(ASSETS_DEBUG,"index: %i\nsize: %i\nname: %s\n\n",i,size,path);
      DEBUGP(ASSETS_DEBUG,"Loaded asset %s\n",name);
      //DEBUGP("%s\n",s);
      return s;
    }
  }
  std::filesystem::path rootpath=root/stdpath;
  return pathtobuf(rootpath);
}
