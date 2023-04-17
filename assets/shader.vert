#version 120

uniform mat4x4 model;
uniform mat4x4 view;
uniform mat4x4 projection;

uniform mat4x4 mvp;

attribute vec3 coord;
attribute vec3 normal;
attribute vec4 color;
varying vec4 frag_color;
varying vec3 frag_normal;

void main() {
//      gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  gl_Position = projection * view * model * vec4(coord, 1.0);
  frag_color = color;
  frag_normal = normal;
}
