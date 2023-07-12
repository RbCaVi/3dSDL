#include "shared.h++"

#ifdef DEBUG
#include <cerrno>
#endif

#include "shaders.h++"
#include "window.h++"
#include "texture.h++"

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
#define printArray(data,len) IGNORE(data),IGNORE(len)
#endif

void printGlError(){
  while(true){
    GLenum err=glGetError();
    printf("gl error code: %i\n",err);
    if(err==GL_NO_ERROR){
      break;
    }
    switch(err){
     case GL_NO_ERROR:
      printf("GL_NO_ERROR\n");
      break;
     case GL_INVALID_ENUM:
      printf("GL_INVALID_ENUM\n");
      break;
     case GL_INVALID_VALUE:
      printf("GL_INVALID_VALUE\n");
      break;
     case GL_INVALID_OPERATION:
      printf("GL_INVALID_OPERATION\n");
      break;
     case GL_INVALID_FRAMEBUFFER_OPERATION:
      printf("GL_INVALID_FRAMEBUFFER_OPERATION\n");
      break;
     case GL_OUT_OF_MEMORY:
      printf("GL_OUT_OF_MEMORY");
      break;
     case GL_STACK_UNDERFLOW:
      printf("GL_STACK_UNDERFLOW");
      break;
     case GL_STACK_OVERFLOW:
      printf("GL_STACK_OVERFLOW");
      break;
     default:
      printf("GL error");
      break;
    }
  }
}

bool checkFramebuffer(){
  // check if the frame buffer is initialized properly
  // and if not, exit
  switch(glCheckFramebufferStatus(GL_FRAMEBUFFER)){
   case GL_FRAMEBUFFER_COMPLETE:
    printf("No error\n");
    return false;
   case GL_FRAMEBUFFER_UNDEFINED:
    printf("GL_FRAMEBUFFER_UNDEFINED\n");
    return true;
   case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
    printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
    return true;
   case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
    printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");
    return true;
   case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
    printf("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n");
    return true;
   case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
    printf("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n");
    return true;
   case GL_FRAMEBUFFER_UNSUPPORTED:
    printf("GL_FRAMEBUFFER_UNSUPPORTED\n");
    return true;
   case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
    printf("GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE\n");
    return true;
   case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
    printf("GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS\n");
    return true;
   default:
    printf("Framebuffer not initialized properly\n");
    return true;
  }
}

Window::Window(int width, int height, const char* name, bool tosaveframes, int frames):
    width(width),height(height),program(0),
    handles_array(NULL),numhandles(0),
    saveframes(tosaveframes),frameTexture(0),framestosave(frames),framesdone(0),
    closed(false){
  window = SDL_CreateWindow(name, 0, 0,
          width, height, SDL_WINDOW_OPENGL);
  gl_context = SDL_GL_CreateContext(window);
  glewInit();
  // Global draw state 
  glViewport(0, 0, width, height); // use 0,0,width,height as the rectangle to draw in (x,y,w,h)
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // set the background color
  draw_mode=GL_TRIANGLES;
  draw_vertices=12;
  
  if(saveframes){
    Window::setupSaveFrames();
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

void Window::makeShaderFromSource(char *vertex_shader_source, char *fragment_shader_source){
  if(program!=0){
    glDeleteProgram(program); // delete the program
    program=0;
  }
  // Shader setup. 
  try{
    program = compile_shader_from_source(vertex_shader_source, fragment_shader_source);
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
  
  closed=false;
  
  while (!closed) {
    if(framestosave>0&&framesdone==framestosave){
      break;
    }
    if(onframe!=NULL){
      (*onframe)(this,data);
    }
    DEBUGP("frame\n");
    glClear(GL_COLOR_BUFFER_BIT // clear the background
        | GL_DEPTH_BUFFER_BIT); // and the depth buffer
    glDrawArrays(draw_mode, 0, draw_vertices);
    if(saveframes){
      DEBUGP("frame written\n");
      Window::writeFrame();
    }
    SDL_GL_SwapWindow(window); // draw the graphics buffer to the screen
    
    SDL_Event event;
 
    // Events management
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
       case SDL_KEYDOWN:
        DEBUGP("down %c %i\n",event.key.keysym.sym,event.key.keysym.sym);
        if(onkeydown!=NULL){
          (*onkeydown)(this,event.key.keysym,data);
        }
        break;
       case SDL_KEYUP:
        DEBUGP("up %c %i\n",event.key.keysym.sym,event.key.keysym.sym);
        if(onkeyup!=NULL){
          (*onkeyup)(this,event.key.keysym,data);
        }
        break;

 
       case SDL_QUIT:
        // handling of close button
        closed = true;
        break;
       default:
        break;
      }
    }
    
    framesdone++;
  }
  
  for(auto attr:handles){
    DEBUGP("%s %i\n",attr.first,attr.second);
    glDisableVertexAttribArray(glGetAttribLocation(program,attr.first));
  }
}

void Window::addVertexData(const char* name,GLfloat data[],GLint size,GLint floatspervertex,GLint stride){
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
  glVertexAttribPointer(attribute, floatspervertex, GL_FLOAT, GL_FALSE, stride, 0);
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

void Window::setupSaveFrames(){
  // create a framebuffer
  GLuint framebuffer=0;
  glGenFramebuffers(1,&framebuffer);DEBUGR(printGlError());
  glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);DEBUGR(printGlError());DEBUGR(checkFramebuffer());


  // create a texture to render the framebuffer to
  frameTexture=0;
  glGenTextures(1,&frameTexture);DEBUGR(printGlError());DEBUGR(checkFramebuffer());
  glBindTexture(GL_TEXTURE_2D,frameTexture);DEBUGR(printGlError());DEBUGR(checkFramebuffer());

  // actually initialize the texture
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);DEBUGR(printGlError());DEBUGR(checkFramebuffer());
  
  DEBUGR(
  int w;
  glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&w);
  int h;
  glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&h);
  printf("%i %i %i\n",frameTexture,w,h);
  );

  // set interpolation
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);DEBUGR(printGlError());DEBUGR(checkFramebuffer());
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);DEBUGR(printGlError());DEBUGR(checkFramebuffer());


  // create a depth buffer
  GLuint depthbuffer=0;
  glGenRenderbuffers(1,&depthbuffer);DEBUGR(printGlError());DEBUGR(checkFramebuffer());
  glBindRenderbuffer(GL_RENDERBUFFER,depthbuffer);DEBUGR(printGlError());DEBUGR(checkFramebuffer());

  // allocate memory for the depth buffer
  glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT32F,width,height);DEBUGR(printGlError());DEBUGR(checkFramebuffer());


  // attach the depth buffer to the frame buffer
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthbuffer);DEBUGR(printGlError());DEBUGR(checkFramebuffer());

  // attach the texture to the frame buffer
  glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,frameTexture,0);DEBUGR(printGlError());DEBUGR(checkFramebuffer());


  // set the framebuffer to be drawn
  GLenum drawBuffers[1]={GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1,drawBuffers);DEBUGR(printGlError());DEBUGR(checkFramebuffer());

  checkFramebuffer();

  // set the size of the screen
  glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
  glViewport(0,0,width,height);
  
  // initialize an opencv video writer
  const char* fourcc="MP42";
  int codec = CV_FOURCC(fourcc[0],fourcc[1],fourcc[2],fourcc[3]);
  const char* filename = "output/file.avi";
  cv::Size frameSize(width,height);
  writer=new cv::VideoWriter(filename, codec, 60, frameSize, true);
}

void Window::writeFrame(){
  DEBUGP("frame\n");
  GLchar *pixels=readTexture(frameTexture);
  DEBUGP("frame\n");
  cv::Mat *image=new cv::Mat(height,width,CV_8UC3,pixels);
  DEBUGP("frame\n");
  writer->write(*image);
  DEBUGP("frame\n");
  delete image;
  DEBUGP("frame\n");
  free(pixels);
  DEBUGP("frame\n");
}

Window::~Window(){
  glDeleteBuffers(numhandles, handles_array); // delete all the buffers
  glDeleteProgram(program); // delete the program
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  free(handles_array);
  
  if(saveframes){
    printf("Write complete !");
    writer->release();
  }
}
