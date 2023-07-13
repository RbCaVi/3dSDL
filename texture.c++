#include "shared.h++"

#include <filesystem>

#include <SDL2/SDL_image.h>
#include <GL/glew.h>

#include "texture.h++"

GLuint loadTexture(std::filesystem::path path){
  SDL_Surface *image=IMG_Load(path.string().c_str());
  image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA8888, 0);
  GLuint object;
  glGenTextures(1,&object);
  glBindTexture(GL_TEXTURE_2D,object);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,image->w,image->h,0,GL_RGBA,GL_UNSIGNED_BYTE,image->pixels);
  SDL_FreeSurface(image);
  return object;
}

GLchar *readTexture(GLuint texture){
  DEBUGR(TEX_DEBUG,errno=0);
  glBindTexture(GL_TEXTURE_2D,texture);
  int width;
  glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&width);
  int height;
  glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&height);
  int size=width*height*3*sizeof(GLchar);
  GLchar *pixels=(GLchar*)malloc(size);
  DEBUGP(TEX_DEBUG,"size=%i\n",size);
  DEBUGP(TEX_DEBUG,"texerrno=%i\n",errno);
  glGetTexImage(GL_TEXTURE_2D,0,GL_RGB,GL_BYTE,pixels);
  return pixels;
}
