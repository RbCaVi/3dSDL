#ifndef TEXTURE_H
#define TEXTURE_H

#include "shared.h++"

#include <filesystem>

#include <GL/glew.h>

GLuint loadTexture(std::filesystem::path path);
GLchar *readTexture(GLuint texture);

#endif
