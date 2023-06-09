#include "shared.h++"

#include <filesystem>
#include <string.h>
#include <stdlib.h>

#include "assets.h++"

extern char *assetsdata;
extern int *assetlengths;
extern char *assetnames;
extern int assetcount;

void assets::setroot(std::filesystem::path newroot){
  IGNORE(newroot);
}

char *assets::getasset(std::filesystem::path stdpath){
  const char *path=stdpath.c_str();
  int i;
  for(i=0;i<assetcount;i++){
    int size=*((&assetlengths)[i]);
    char *name=(&assetnames)[i];
    DEBUGP("index: %i\nsize: %i\nname: %s\n\n",i,size,path);
    DEBUGP("Checking asset %s\n",name);
    if(strcmp(path,name)==0){
      char *blob=(&assetsdata)[i];
      char *s=(char*)malloc(size+1);
      strncpy(s,blob,size);
      s[size]='\0';
      DEBUGP("index: %i\nsize: %i\nname: %s\n\n",i,size,path);
      DEBUGP("Loaded asset %s\n",name);
      //DEBUGP("%s\n",s);
      return s;
    }
  }
  return NULL;
}