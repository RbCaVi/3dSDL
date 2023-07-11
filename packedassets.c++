#include "shared.h++"

#include <string.h>
#include <stdlib.h>

#include "assets.h++"

extern char *assetsdata;
extern int *assetlengths;
extern char *assetnames;
extern int assetcount;

void assets::setroot(std::filesystem::path newroot){}

char *assets::getasset(std::filesystem::path stdpath){
  char *path=stdpath.c_str();
  int i;
  for(i=0;i<assetcount;i++){
    char *name=(&assetnames)[i];
    if(strcmp(path,name)==0){
      int size=*((&assetlengths)[i]);
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