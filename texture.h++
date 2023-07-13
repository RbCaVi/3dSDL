#ifndef TEXTURE_H
#define TEXTURE_H

#include "shared.h++"

#include <filesystem>

#include <GL/glew.h>

#ifdef TEX_DEBUG
#define _TEX_DEBUG 1
#else
#define _TEX_DEBUG 0
#endif

GLuint loadTexture(std::filesystem::path path);
GLchar *readTexture(GLuint texture);

#endif
