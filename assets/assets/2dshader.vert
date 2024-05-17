#version 120

uniform float x;
uniform float y;

attribute vec2 coord;
varying vec4 frag_color;

void main() {
  gl_Position = vec4(coord+vec2(x,y),0,1.0);
  frag_color=vec4(1.0,0.0,0.0,1.0);
}
