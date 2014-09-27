#ifndef VERTEX_SHADER_H
#define VERTEX_SHADER_H
#define GLSL(version, shader) "#version " #version "\n" #shader

const GLchar* VERTEX_SHADER_SOURCE = GLSL(120, 

	
attribute vec3 position;
attribute vec3 normal;

uniform mat4 m;
uniform mat3 minv;
uniform mat4 mvp;

varying vec3 v_normal;
varying vec3 v_pos;

void main(void) {
    v_normal = normalize(minv * normal);
    v_pos = vec3(m * vec4(position, 1.0));
    gl_Position = mvp * vec4(position, 1.0);
};


);
#endif