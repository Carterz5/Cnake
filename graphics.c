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


unsigned int SetShader(){
    
    
    ShaderProgramSource source = ParseShader("./res/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);


    return shader;
}


void GLDrawBox(box* object){
    static int calls = 0;
    static unsigned int vbo[2];
    static unsigned int vao[2];
    
    if (calls == 0){
        glGenBuffers(2, vbo);
        glGenVertexArrays(2, vao);
        glBindVertexArray(vao[0]);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glBindVertexArray(vao[1]);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);

    };
    
    float positions[20] = {
        object->xPos - object->size, (object->yPos - object->size)*RES_RATIO, object->color[0], object->color[1], object->color[2],
        object->xPos + object->size, (object->yPos - object->size)*RES_RATIO, object->color[0], object->color[1], object->color[2],
        object->xPos + object->size, (object->yPos + object->size)*RES_RATIO, object->color[0], object->color[1], object->color[2],
        object->xPos - object->size, (object->yPos + object->size)*RES_RATIO, object->color[0], object->color[1], object->color[2],

    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBindVertexArray(vao[0]);
    glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), positions, GL_STATIC_DRAW);

    if(calls == 0){
        printf("calls is %d\n", calls);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(2 * sizeof(float)));
    };

    glDrawArrays(GL_QUADS, 0, 4);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    calls++;

}

GLFWwindow* init_glfw(){

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return NULL;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(RES_X, RES_Y, "Hello World", NULL, NULL);
    if (!window){
        glfwTerminate();
        return NULL;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK){
        printf("Error!\n");
    }
    const char *glversion = glGetString(GL_VERSION);
    printf("GL version is |%s|\n", glversion);

    glfwSetKeyCallback(window, key_callback);

    return window;
}