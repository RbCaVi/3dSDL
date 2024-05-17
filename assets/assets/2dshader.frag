#version 120

uniform sampler2D img;

varying vec2 frag_uv;

void main() {
  vec2 uv = frag_uv;
  uv.y = 1 - uv.y;
  gl_FragData[0] = texture2D(img,uv).abgr; // the data is reverse order i guess
}
