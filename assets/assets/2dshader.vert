#version 120

uniform float x;
uniform float y;

attribute vec2 coord;
attribute vec2 uv;
varying vec2 frag_uv;

void main() {
  gl_Position = vec4(coord+vec2(x,y),0,1.0);
  frag_uv = uv;
}
