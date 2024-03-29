#ifndef SHADERS_H
#define SHADERS_H

#include "shared.h++"

#include <filesystem>

#include <GL/glew.h>

#ifdef SHADER_DEBUG
#define _SHADER_DEBUG 1
#else
#define _SHADER_DEBUG 0
#endif

class shader_compile_exception: public std::exception{
public:
  explicit shader_compile_exception(const char* message,const char* log);
  virtual ~shader_compile_exception() noexcept;
  virtual const char* what() const noexcept;
  
  const char* log;
  const char* msg;
};

GLuint compile_vert_shader_from_source(const char *vertex_shader_source);
GLuint compile_frag_shader_from_source(const char *fragment_shader_source);
GLuint compile_vert_shader(std::filesystem::path vertex_shader_path);
GLuint compile_frag_shader(std::filesystem::path fragment_shader_path);
GLuint compile_shader(GLuint vertex_shader,GLuint fragment_shader);
GLuint compile_shader_from_source(const char *vertex_shader_source, const char *fragment_shader_source);
GLuint compile_shader(std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path);
void deleteProgram(GLuint program);
#endif
