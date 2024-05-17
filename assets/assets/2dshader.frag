#version 120

uniform sampler2D img;

//varying vec4 frag_color;
varying vec2 frag_uv;

void main() {
  // if(frag_color.a<0.1){
  //   discard;
  // }
  //vec2 c = (frag_uv+vec2(0.2,0.2))/vec2(0.4,0.4);
  vec2 c = (frag_uv);
  gl_FragData[0] = //(texture2D(img,c)+vec4(1.0,1.0,1.0,1.0))/2.0;
  gl_FragData[0] = vec4(c,0.0,1.0);
  //gl_FragData[0] = frag_color;
}
