#include <unordered_map>
#include <filesystem>
#include <cerrno>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "shaders.h++"
#include "window.h++"

#ifdef DEBUG
void __printArray__(GLfloat data[],int length){
  int i;
  for(i=0;i<length;i++){
    printf("%f ",data[i]);
  }
  printf("\n");
}
#define printArray(data,len) DEBUGR(__printArray__(data,len))
#else
#define printArray(data,len) DEBUGR(data),DEBUGR(len)
#endif

Window::Window(int width, int height, const char* name, bool gifmode):program(0),handles_array(NULL),numhandles(0),gif(gifmode){
  window = SDL_CreateWindow(name, 0, 0,
          width, height, SDL_WINDOW_OPENGL);
  gl_context = SDL_GL_CreateContext(window);
  glewInit();
  // Global draw state 
  glViewport(0, 0, width, height); // use 0,0,width,height as the rectangle to draw in (x,y,w,h)
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // set the background color
  draw_mode=GL_TRIANGLES;
  draw_vertices=12;
  
  if(gif){
    Window::setupGIF();
  }
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
  for(auto attr:handles){
    DEBUGP("%s %i\n",attr.first,attr.second);
    DEBUGR(errno=0);
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
  
  for(auto attr:handles){
    DEBUGP("%s %i\n",attr.first,attr.second);
    glDisableVertexAttribArray(glGetAttribLocation(program,attr.first));
  }
}

void Window::addVertexData(const char* name,GLfloat data[],GLint size,GLint floatspervertex){
  GLuint handle;
  GLint attribute = glGetAttribLocation(program, name);
  if(!handles.contains(name)){
    glGenBuffers(1, &handle);
    int handlei=numhandles++;
    handles_array=(GLuint*)realloc(handles_array,numhandles*sizeof(GLuint));
    handles[name]=handlei;
    handles_array[handlei]=handle;
  }else{
    handle=handles_array[handles[name]];
  }
  glBindBuffer(GL_ARRAY_BUFFER, handle);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  glVertexAttribPointer(attribute, floatspervertex, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Window::addUniformMat4x4(const char* name,matrix4x4 &matrix){
  GLint uniform = glGetUniformLocation(program, name);
  DEBUGP("%s",name);
  printArray(matrix.contents,4);
  printArray(matrix.contents+4,4);
  printArray(matrix.contents+8,4);
  printArray(matrix.contents+12,4);
  glUniformMatrix4fv(uniform, 1, GL_FALSE, matrix.contents);
}

void Window::setupGIF(){
  // create a framebuffer
  GLuint framebuffer=0;
  glGenFramebuffers(1,&framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);


  // create a texture to render the framebuffer to
  GLuint texture=0;
  glGenTextures(1,&texture);
  glBindTexture(GL_TEXTURE_2D,texture);

  // actually initialize the texture
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,GL_RGB,GL_UNSIGNED_BYTE,0);

  // set interpolation
  glTexParameteri(GL_TEXTURE_2D,GL_TESTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TESTURE_MIN_FILTER,GL_NEAREST);


  // create a depth buffer
  GLuint depthbuffer=0;
  glGenRenderbuffers(1,&depthbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER,depthbuffer);

  // allocate memory for the depth buffer
  glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,width,height);


  // attach the depth buffer to the frame buffer
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthbuffer);

  // attach the texture to the frame buffer
  glFramebufferTexture(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT0,texture,0);


  // set the framebuffer to be drawn
  GLenum drawBuffers[1]={GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1,drawBuffers);


  // check if the frame buffer is initialized properly
  // and if not, exit
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE){
    printf("Framebuffer not initialized properly");
    exit(EXIT_FAILURE);
  }


  // set the size of the screen
  glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
  glViewport(width,height);
}

Window::~Window(){
  glDeleteBuffers(numhandles, handles_array); // delete all the buffers
  glDeleteProgram(program); // delete the program
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  free(handles_array);
}
