#version 120

uniform mat4x4 model;
uniform mat4x4 view;
uniform mat4x4 projection;

uniform mat4x4 mvp;

attribute vec3 coord2d;
attribute vec4 in_color;
varying vec4 color;

void main() {
//      gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
 gl_Position = projection * view * model * vec4(coord2d, 1.0);
 color = in_color;
}
