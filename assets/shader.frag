#version 120

varying vec4 frag_color;
varying vec3 frag_normal;

void main() {
  if(frag_color.a<0.1){
    discard;
  }
 //gl_FragColor = frag_color;
 gl_FragData[0] = frag_color;
}
