#ifndef WINDOW_H
#define WINDOW_H

#include "shared.h++"

#include <unordered_map>
#include <filesystem>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#if HAS_OPENCV
#include <opencv2/highgui/highgui.hpp>
#endif

#include "matrix.h++"

#ifdef WINDOW_DEBUG
#define _WINDOW_DEBUG 1
#else
#define _WINDOW_DEBUG 0
#endif

#ifdef WINDOW_ATTR_DEBUG
#define _WINDOW_ATTR_DEBUG 1
#else
#define _WINDOW_ATTR_DEBUG 0
#endif

#ifdef WINDOW_GL_DEBUG
#define _WINDOW_GL_DEBUG 1
#else
#define _WINDOW_GL_DEBUG 0
#endif

#ifdef WINDOW_KEY_DEBUG
#define _WINDOW_KEY_DEBUG 1
#else
#define _WINDOW_KEY_DEBUG 0
#endif

#ifdef WINDOW_WRITEFRAME_DEBUG
#define _WINDOW_WRITEFRAME_DEBUG 1
#else
#define _WINDOW_WRITEFRAME_DEBUG 0
#endif

class window_exception: public std::exception{
public:
  explicit window_exception(const char* message);
  virtual ~window_exception() noexcept;
  virtual const char* what() const noexcept;

  const char* msg;
};

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

public:
  GLsizei draw_vertices;

#if HAS_OPENCV
private:
  bool saveframes;
  GLuint frameTexture;
  GLuint frameBuffer;
  int framestosave;
  int framesdone;
  cv::VideoWriter *writer;
#endif

public:
  void (*onframe)(Window*,void*)=NULL;
  void (*draw)(Window*,void*)=NULL;
  void (*onkeyup)(Window*,SDL_Keysym,void*)=NULL;
  void (*onkeydown)(Window*,SDL_Keysym,void*)=NULL;
  void *data;
  bool closed;

  Window(int width, int height, const char* name);
  void makeShader(std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path);
  void makeShaderFromSource(char *vertex_shader_source, char *fragment_shader_source);
  void mainLoop();
  void addVertexData(const char* name,GLfloat data[],GLint size,GLint floatspervertex,GLint stride=0);
  void setUniformMat4x4(const char* name,const matrix4x4 &matrix);
  void setUniformVec3(const char* name,float *vec);
  void enableSaveFrames(int frames);

#if HAS_OPENCV
private:
  void setupSaveFrames();
  void writeFrame();
#endif

public:
  ~Window();
};

#endif
