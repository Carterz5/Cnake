#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include "snake.h"




ShaderProgramSource ParseShader(const char* filepath){
    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("Could not open file: %s\n", filepath);
        exit(1);
    }

    ShaderProgramSource source;
    source.VertexSource = NULL;
    source.FragmentSource = NULL;

    ShaderType type = NONE;
    char* line = NULL;
    size_t len = 0;
    int read;
    char* vertexShader = NULL;
    char* fragmentShader = NULL;
    size_t vertexSize = 0;
    size_t fragmentSize = 0;

    while ((read = getline(&line, &len, file)) != -1) {
        if (strstr(line, "#shader") != NULL) {
            if (strstr(line, "vertex") != NULL) {
                type = VERTEX;
            } else if (strstr(line, "fragment") != NULL) {
                type = FRAGMENT;
            }
        } else {
            if (type == VERTEX) {
                vertexShader = realloc(vertexShader, vertexSize + read + 1);
                strcpy(vertexShader + vertexSize, line);
                vertexSize += read;
            } else if (type == FRAGMENT) {
                fragmentShader = realloc(fragmentShader, fragmentSize + read + 1);
                strcpy(fragmentShader + fragmentSize, line);
                fragmentSize += read;
            }
        }
    }

    fclose(file);
    free(line);

    source.VertexSource = vertexShader;
    source.FragmentSource = fragmentShader;

    return source;
}


unsigned int CompileShader(unsigned int type, const char* source){

    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        if (type == GL_VERTEX_SHADER){
            printf("Failed to compile vertex shader!\n");
        } else {
            printf("Failed to compile fragment shader!\n");
        }
        printf("%s\n", message);
        glDeleteShader(id);
        return 0;

    }

    return id;
}


unsigned int CreateShader(const char* vertexShader, const char* fragmentShader){

    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}