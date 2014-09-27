#ifndef FRAGMENT_SHADER_H
#define FRAGMENT_SHADER_H

#define GLSL(version, shader) "#version " #version "\n" #shader
const GLchar* FRAGMENT_SHADER_SOURCE = GLSL(120,


uniform vec3 colour;

varying vec3 v_normal;
varying vec3 v_pos;

vec3 light_pos = vec3(0.0f, 10.0f, 0.0f);
vec3 light_col = vec3(1.0f, 1.0f, 1.0f);

void main(void) {

	vec3 normalDirection = normalize(v_normal);
    vec3 lightDirection = normalize(light_pos - v_pos);
 
    vec3 diffuse = light_col * colour * max(0.0, dot(normalDirection, lightDirection));
    gl_FragColor = vec4(diffuse, 1.0);
}


);
#endif