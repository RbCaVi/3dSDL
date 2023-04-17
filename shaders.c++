#include "shaders.h++"

shader_compile_exception::shader_compile_exception(const char* message,const char* log): log(log), msg(message) {}

shader_compile_exception::~shader_compile_exception() noexcept {}

const char* shader_compile_exception::what() const noexcept {
   return msg;
}

// A function that will read a file at a path into an allocated char pointer buffer 
char* pathtobuf(std::filesystem::path path) {
  FILE *fptr;
  long length;
  char *buf;

  fptr = fopen(path.string().c_str(), "rb"); // Open file for reading 
  if (!fptr) // Return NULL on failure 
    return NULL;
  fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
  length = ftell(fptr); // Find out how many bytes into the file we are 
  buf = (char*)malloc(length+1); // Allocate a buffer for the entire length of the file and a null terminator 
  fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
  fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer 
  fclose(fptr); // Close the file 
  buf[length] = 0; // Null terminator 

  return buf; // Return the buffer 
}

GLuint compile_vert_shader(const char *vertex_shader_source){
  //printf("%s",vertex_shader_source);
  GLchar *log = NULL;
  GLint log_length, success;
  GLuint vertex_shader;
  // Vertex shader 
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertex_shader);
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_length);
  log = (GLchar*)malloc(log_length);
  glGetShaderInfoLog(vertex_shader, log_length, NULL, log);
  if (log_length > 0) {
    printf("vertex shader log:\n\n%s\n", log);
  }
  if (!success) {
    printf("vertex shader compile error\n");
    throw shader_compile_exception("vertex shader compile exception",log);
  }
  free(log);
  return vertex_shader;
}

GLuint compile_frag_shader(const char *fragment_shader_source){
  GLchar *log = NULL;
  GLint log_length, success;
  GLuint fragment_shader;
  // Fragment shader 
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_length);
  log = (GLchar*)malloc(log_length);
  glGetShaderInfoLog(fragment_shader, log_length, NULL, log);
  if (log_length > 0) {
    printf("fragment shader log:\n\n%s\n", log);
  }
  if (!success) {
    printf("fragment shader compile error\n");
    throw shader_compile_exception("fragment shader compile exception",log);
  }
  free(log);
  return fragment_shader;
}

GLuint compile_vert_shader(std::filesystem::path vertex_shader_path){
  char *source=pathtobuf(vertex_shader_path);
  GLuint shader=compile_vert_shader(source);
  free(source);
  return shader;
}

GLuint compile_frag_shader(std::filesystem::path fragment_shader_path){
  char *source=pathtobuf(fragment_shader_path);
  GLuint shader=compile_frag_shader(source);
  free(source);
  return shader;
}

GLuint compile_shader(GLuint vertex_shader,GLuint fragment_shader){
  GLchar *log = NULL;
  GLint log_length, success;
  GLuint program;
  // Link shaders 
  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
  log = (GLchar*)malloc(log_length);
  glGetProgramInfoLog(program, log_length, NULL, log);
  if (log_length > 0) {
    printf("shader link log:\n\n%s\n", log);
  }
  if (!success) {
    printf("shader link error");
    throw shader_compile_exception("program linking exception",log);
  }
  // Cleanup. 
  free(log);
  // queue the shaders for deletion when the program is deleted
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  return program;
}

GLuint compile_shader(const char *vertex_shader_source, const char *fragment_shader_source){
  GLuint vertex_shader=compile_vert_shader(vertex_shader_source);
  GLuint fragment_shader=compile_frag_shader(fragment_shader_source);
  return compile_shader(vertex_shader,fragment_shader);
}

GLuint compile_shader(std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path){
  GLuint vertex_shader=compile_vert_shader(vertex_shader_path);
  GLuint fragment_shader=compile_frag_shader(fragment_shader_path);
  return compile_shader(vertex_shader,fragment_shader);
}

void deleteProgram(GLuint program){ // not needed unless shaders are kept for multiple programs
  GLint numShaders;
  GLuint *shaders=NULL;
  glGetProgramiv(program,GL_ATTACHED_SHADERS,&numShaders);
   // get the shaders attached to 'program' and store them in 'shaders'
  glGetAttachedShaders(program,(GLsizei)numShaders,NULL,shaders);
  int i;
  for(i=0;i<numShaders;i++){
    glDeleteShader(shaders[i]);
  }
  free(shaders);
  glDeleteProgram(program);
}
