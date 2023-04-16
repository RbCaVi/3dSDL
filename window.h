#ifndef WINDOW_H
#define WINDOW_H

#include "shared.h"

#include <unordered_map>
#include <filesystem>

#include <SDL2/SDL.h>
#include <GL/glew.h>

class Window{
public:
  SDL_GLContext gl_context;
  SDL_Window *window;
  GLuint program;
  std::unordered_map<const char*,int> handleis;
  std::unordered_map<const char*,GLint> attrs;
  GLuint* handles_array;
  int numhandles;
  GLenum draw_mode;
  GLsizei draw_vertices;
  
  Window(int width, int height, const char* name);
  void makeShader(std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path);
  void mainLoop();
  void addVertexData(const char* name,GLfloat data[],GLint size,GLint floatspervertex);
  ~Window();
};

#endif
