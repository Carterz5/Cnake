#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include "snake.h"
#include <ft2build.h>
#include <cglm/cglm.h>
#include <cglm/vec3.h>
#include <cglm/mat4.h> 
#include <cglm/cam.h> 
#include <cglm/affine.h>
#include FT_FREETYPE_H


typedef struct {
    GLuint TextureID;
    int Size[2];
    int Bearing[2];
    unsigned int Advance;
} Character;


Character Characters[255];

unsigned int vbo[2];
unsigned int vao[2];


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


unsigned int SetShaders(char* filepath){
    
    
    ShaderProgramSource source = ParseShader(filepath);
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);



    return shader;
}


void GLDrawBox(unsigned int shaderProgram, box* object){
    glUseProgram(shaderProgram);
    
    float positions[20] = {
        object->xPos - object->size, (object->yPos - object->size)*RES_RATIO, object->color[0], object->color[1], object->color[2],
        object->xPos + object->size, (object->yPos - object->size)*RES_RATIO, object->color[0], object->color[1], object->color[2],
        object->xPos + object->size, (object->yPos + object->size)*RES_RATIO, object->color[0], object->color[1], object->color[2],
        object->xPos - object->size, (object->yPos + object->size)*RES_RATIO, object->color[0], object->color[1], object->color[2],

    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBindVertexArray(vao[0]);
    glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), positions, GL_STATIC_DRAW);


    glDrawArrays(GL_QUADS, 0, 4);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



}

void GLDrawSnake(unsigned int shaderProgram, player* head){
    int draw = 1;
    while (head != NULL){

        GLDrawBox(shaderProgram, &head->position);
        //printf(" i draw %d \n", draw);
        draw++;
        head = head->next;
    }
    

}

GLFWwindow* init_glfw(){

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return NULL;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(RES_X, RES_Y, "Snake", NULL, NULL);
    if (!window){
        glfwTerminate();
        return NULL;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK){
        printf("Error!\n");
    }
    const unsigned char *glversion = glGetString(GL_VERSION);
    printf("GL version is |%s|\n", glversion);

    glfwSetKeyCallback(window, key_callback);

    return window;
}

void init_opengl(){

    glGenBuffers(2, vbo);
    glGenVertexArrays(2, vao);

    //box rendering
    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), NULL, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //text rendering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    //unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


}

void load_fonts(){
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        fprintf(stderr, "Could not init FreeType Library\n");
        exit(EXIT_FAILURE);
    }

    FT_Face face;
    if (FT_New_Face(ft, "Roboto-Regular.ttf", 0, &face)) {
        fprintf(stderr, "Failed to load font\n");
        exit(EXIT_FAILURE);
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            fprintf(stderr, "Failed to load Glyph\n");
            continue;
        }
        
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            {face->glyph->bitmap.width, face->glyph->bitmap.rows},
            {face->glyph->bitmap_left, face->glyph->bitmap_top},
            face->glyph->advance.x
        };
        Characters[c] = character;
        //printf("glyh: %c bitmap width: %d, bitmap pitch: %d\n", c, face->glyph->bitmap.width, face->glyph->bitmap.pitch);
        //printf("loaded glyph %c, size: %d - %d, bearing: %d - %d, advance: %ld\n", c, face->glyph->bitmap.width, face->glyph->bitmap.rows,face->glyph->bitmap_left, face->glyph->bitmap_top, face->glyph->advance.x);
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void RenderText(GLuint shaderProgram, const char* text, float x, float y, float scale, float color[3]) {
    glUseProgram(shaderProgram);
    mat4 projection;
    glm_ortho(0.0f, RES_X, 0.0f, RES_Y, -1.0f, 1.0f, projection);
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float*)projection);

    GLint textColorLocation = glGetUniformLocation(shaderProgram, "textColor");
    glUniform3f(textColorLocation, color[0], color[1], color[2]);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao[1]);

    for (const char* p = text; *p; p++) {
        Character ch = Characters[*p];

        float xpos = x + ch.Bearing[0] * scale;
        float ypos = y - (ch.Size[1] - ch.Bearing[1]) * scale;

        float w = ch.Size[0] * scale;
        float h = ch.Size[1] * scale;
        //printf("letter = %c, xpos: %f, ypos: %f, w: %f, h: %f\n", *p, xpos, ypos, w, h);
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        //printf("letter = %c, textureID = %c\n", *p, ch.TextureID );
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.Advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}