#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <GL/glew.h>
#include "snake.h"
#include <ft2build.h>
#include <cglm/cglm.h>
#include <cglm/vec3.h>
#include <cglm/mat4.h> 
#include <cglm/cam.h> 
#include <cglm/affine.h>
#include FT_FREETYPE_H







typedef enum {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
} ShaderType;


typedef struct {
    char* VertexSource;
    char* FragmentSource;
} ShaderProgramSource;


GLFWwindow* init_glfw();
ShaderProgramSource ParseShader(const char* filepath);
unsigned int CompileShader(unsigned int type, const char* source);
unsigned int CreateShader(const char* vertexShader, const char* fragmentShader);
void GLDrawBox(unsigned int shaderProgram, box* object);
unsigned int SetShaders();
void init_opengl();
void RenderText(GLuint shaderProgram, const char* text, float x, float y, float scale, float color[3]);
void load_fonts();
void GLDrawSnake(unsigned int shaderProgram, player* head);

#endif