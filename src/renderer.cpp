#include "renderer.h"
#include <iostream>
#include "vertex_shader.h"
#include "fragment_shader.h"

#define PROGRAM_NAME "Block Race"



using namespace std;

renderer::renderer() : current_level(NULL), render_width(1280), render_height(800) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
 
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
 
    mainwindow = SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        render_width, render_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
 
    maincontext = SDL_GL_CreateContext(mainwindow);

    glewInit();

    if (!GLEW_VERSION_2_0) {
        fprintf(stderr, "Error: your graphic card does not support OpenGL 2.0\n");
    }
 
    SDL_GL_SetSwapInterval(1);

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &VERTEX_SHADER_SOURCE, NULL);
    glCompileShader(vs);

    GLint status;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
    
    if(status == GL_FALSE)
    {
        printf("\nfailed to compile vertex shader");
        
        GLint logLength;
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &logLength);
        
        if(logLength > 0)
        {
            unsigned char* log = new unsigned char[logLength];
            glGetShaderInfoLog(vs, logLength, &logLength, (GLchar*)&log[0]);
            printf("\n%s", log);
            
            delete log;
        }
    }

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &FRAGMENT_SHADER_SOURCE, NULL);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
    
    if(status == GL_FALSE)
    {
        printf("\nfailed to compile fragment shader");
        
        GLint logLength;
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &logLength);
        
        if(logLength > 0)
        {
            unsigned char* log = new unsigned char[logLength];
            glGetShaderInfoLog(fs, logLength, &logLength, (GLchar*)&log[0]);
            printf("\n%s", log);
            
            delete log;
        }
    }

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE) {
        printf("\nLinking shader failed");
        
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        
        if(logLength > 0)
        {
            unsigned char* log = new unsigned char[logLength];
            glGetProgramInfoLog(program, logLength, &logLength, (GLchar*)&log[0]);
            printf("\n%s", log);
            
            delete log;
        }
    }
    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);

    glClearColor ( 0.0, 0.0, 0.0, 1.0 );
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

renderer::~renderer() {
	glDeleteProgram(program);
    SDL_GL_DeleteContext(maincontext);
    SDL_DestroyWindow(mainwindow);
    SDL_Quit();
}

void renderer::set_level(level& level) {
    current_level = &level;
    level.upload(*this);
}

void renderer::render() {
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(current_level)
        current_level->render();

    SDL_GL_SwapWindow(mainwindow);
}

SDL_Window* renderer::get_window() {
    return mainwindow;
}

void renderer::get_render_size(int& width, int& height) {
    width = render_width;
    height = render_height;
}