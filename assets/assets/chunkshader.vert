#version 120

uniform vec3 position;
uniform mat4x4 model;
uniform mat4x4 view;
uniform mat4x4 projection;

uniform mat4x4 mvp;

attribute vec3 coord;
attribute vec3 normal;
varying vec4 frag_color;
//varying vec3 frag_normal;

void main() {
//      gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  gl_Position = projection * view * model * (vec4(coord+position,1.0));
  //frag_color = vec4(1.0,1.0,1.0,1.0);
  //frag_color=vec4((mvp*vec4(abs(normal),1.0)).xyz,1.0);
  frag_color=vec4((normal+1.0)/2.0,1.0);
  //frag_normal = normal;
}
