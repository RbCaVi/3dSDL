#version 120

attribute vec3 coord2d;
attribute vec3 in_color;
varying vec3 color;

void main() {
//      gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
 gl_Position = vec4(coord2d, 1.0);
 color = in_color;
}
