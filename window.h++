#ifndef WINDOW_H
#define WINDOW_H

#include "shared.h++"

#include <list>
#include <unordered_map>
#include <filesystem>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <opencv2/highgui/highgui.hpp>

#include "matrix.h++"

class Window{
public:
  SDL_GLContext gl_context;
  SDL_Window *window;
  int width;
  int height;
  GLuint program;
  std::unordered_map<const char*,int> handles;
  GLuint* handles_array;
  int numhandles;
  GLenum draw_mode;
  GLsizei draw_vertices;
  bool saveframes;
  GLuint frameTexture;
  int framestosave;
  int framesdone;
  cv::VideoWriter *writer;
  void (&onframe)(Window*,void*);
  void *data;
  
  Window(int width, int height, const char* name, void (&framefunc)(Window*,void*), void *data, bool saveframes, int frames);
  void makeShader(std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path);
  void mainLoop();
  void addVertexData(const char* name,GLfloat data[],GLint size,GLint floatspervertex);
  void addUniformMat4x4(const char* name,matrix4x4 &matrix);
  void setupSaveFrames();
  void writeFrame();
  ~Window();
};

#endif
