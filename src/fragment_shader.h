#ifndef FRAGMENT_SHADER_H
#define FRAGMENT_SHADER_H

#define GLSL(version, shader) "#version " #version "\n" #shader
const GLchar* FRAGMENT_SHADER_SOURCE = GLSL(120,


uniform vec3 colour;

varying vec3 v_normal;
varying vec3 v_pos;

void main(void) {
  gl_FragColor = vec4(colour, 1.0);
}


);
#endif