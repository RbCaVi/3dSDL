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
private:
  SDL_GLContext gl_context;
  SDL_Window *window;
  int width;
  int height;
  GLuint program;
  std::unordered_map<const char*,int> handles;
  GLuint* handles_array;
  int numhandles;
  GLenum draw_mode;
public:
  GLsizei draw_vertices;
private:
  bool saveframes;
  GLuint frameTexture;
  int framestosave;
  int framesdone;
  cv::VideoWriter *writer;
public:
  void (*onframe)(Window*,void*)=NULL;
  void (*onkeyup)(Window*,SDL_Keysym,void*)=NULL;
  void (*onkeydown)(Window*,SDL_Keysym,void*)=NULL;
  void *data;

public:
  bool closed;
  Window(int width, int height, const char* name, bool saveframes, int frames);
  void makeShader(std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path);
  void makeShaderFromSource(char *vertex_shader_source, char *fragment_shader_source);
  void mainLoop();
  void addVertexData(const char* name,GLfloat data[],GLint size,GLint floatspervertex,GLint stride=0);
  void addUniformMat4x4(const char* name,matrix4x4 &matrix);
  void setupSaveFrames();
  void writeFrame();
  ~Window();
};

#endif
