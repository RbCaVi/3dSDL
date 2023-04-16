#include <unordered_map>
#include <filesystem>
#include <cerrno>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "shaders.h"
#include "window.h"

void __printArray__(GLfloat data[],int length){
  int i;
  for(i=0;i<length;i++){
    DEBUGP("%f ",data[i]);
  }
}

#define printArray(data,len) DEBUG(__printArray__(data,len))

Window::Window(int width, int height, const char* name):program(0),handles_array(NULL),numhandles(0){
  window = SDL_CreateWindow(name, 0, 0,
          width, height, SDL_WINDOW_OPENGL);
  gl_context = SDL_GL_CreateContext(window);
  glewInit();
  // Global draw state 
  glViewport(0, 0, width, height); // use 0,0,width,height as the rectangle to draw in (x,y,w,h)
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // set the background color
  draw_mode=GL_TRIANGLES;
  draw_vertices=12;
}

void Window::makeShader(std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path){
  if(program!=0){
    glDeleteProgram(program); // delete the program
    program=0;
  }
  // Shader setup. 
  try{
    program = compile_shader(vertex_shader_path, fragment_shader_path);
  }catch(shader_compile_exception& e){
    printf("exception in shader compilation: %s\n", e.what());
    printf("log: \n%s\n\n", e.log);
    exit(EXIT_FAILURE);
  }
  glUseProgram(program); // use the compiled program
}

void Window::mainLoop(){
  for(auto attr:handleis){
    DEBUGP("%s %i\n",attr.first,attr.second);
    DEBUG(errno=0);
    glEnableVertexAttribArray(glGetAttribLocation(program,attr.first));
    DEBUGP("enable %i\n",errno);
  }
  
  bool closed=false;
  
  while (!closed) {
    glClear(GL_COLOR_BUFFER_BIT // clear the background
        | GL_DEPTH_BUFFER_BIT); // and the depth buffer
    glDrawArrays(draw_mode, 0, draw_vertices);
    SDL_GL_SwapWindow(window); // draw the graphics buffer to the screen
    
    SDL_Event event;
 
    // Events management
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
 
       case SDL_QUIT:
        // handling of close button
        closed = true;
        break;
       default:
        break;
      }
    }
  }
  
  for(auto attr:handleis){
    DEBUGP("%s %i\n",attr.first,attr.second);
    glDisableVertexAttribArray(glGetAttribLocation(program,attr.first));
  }
}

void Window::addVertexData(const char* name,GLfloat data[],GLint size,GLint floatspervertex){
  GLuint handle;
  printArray(data,size/sizeof(GLfloat));
  DEBUG(errno=0);
  GLint attribute = glGetAttribLocation(program, name);
  DEBUGP("getattrib %i\n",errno);
  DEBUG(errno=0);
  glGenBuffers(1, &handle);
  DEBUGP("genbuffers %i\n",errno);
  DEBUG(errno=0);
  glBindBuffer(GL_ARRAY_BUFFER, handle);
  DEBUGP("bindbuffer %i\n",errno);
  DEBUG(errno=0);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  DEBUGP("bufferdata %i\n",errno);
  DEBUG(errno=0);
  glVertexAttribPointer(attribute, floatspervertex, GL_FLOAT, GL_FALSE, 0, 0);
  DEBUGP("vertexattrib %i\n",errno);
  DEBUG(errno=0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  DEBUGP("unbind %i\n",errno);
  int handlei=numhandles++;
  handles_array=(GLuint*)realloc(handles_array,numhandles*sizeof(GLuint));
  handleis[name]=handlei;
  handles_array[handlei]=handle;
  DEBUG(errno=0);
  DEBUGP("%i\n",errno);
}

Window::~Window(){
  glDeleteBuffers(numhandles, handles_array); // delete all the buffers
  glDeleteProgram(program); // delete the program
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  free(handles_array);
}
