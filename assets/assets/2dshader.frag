#version 120

varying vec4 frag_color;

void main() {
  if(frag_color.a<0.1){
    discard;
  }
  gl_FragData[0] = frag_color;
}
