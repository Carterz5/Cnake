#include "graphics.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#define BUFFER_SIZE 1024

typedef struct {
    GLuint TextureID;
    int Size[2];
    int Bearing[2];
    unsigned int Advance;
} Character;


Character Characters[255];

unsigned int vbo[2];
unsigned int vao[2];
unsigned int texture_ID_array[4];
unsigned int testtex;


ShaderProgramSource ParseShader(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("Could not open file: %s\n", filepath);
        exit(1);
    }

    ShaderProgramSource source;
    source.VertexSource = NULL;
    source.FragmentSource = NULL;

    ShaderType type = NONE;
    char buffer[BUFFER_SIZE];
    char* vertexShader = NULL;
    char* fragmentShader = NULL;
    size_t vertexSize = 0;
    size_t fragmentSize = 0;

    while (fgets(buffer, BUFFER_SIZE, file)) {
        size_t len = strlen(buffer);
        
        // if (buffer[len - 1] == '\n') {
        //     buffer[len - 1] = '\0';  // Remove trailing newline character
        // }

        if (strstr(buffer, "#shader") != NULL) {
            if (strstr(buffer, "vertex") != NULL) {
                type = VERTEX;
            } else if (strstr(buffer, "fragment") != NULL) {
                type = FRAGMENT;
            }
        } else {
            if (type == VERTEX) {
                vertexShader = realloc(vertexShader, vertexSize + len + 1);
                if (!vertexShader) {
                    printf("Memory allocation failed\n");
                    exit(1);
                }
                strcpy(vertexShader + vertexSize, buffer);
                vertexSize += len;
            } else if (type == FRAGMENT) {
                fragmentShader = realloc(fragmentShader, fragmentSize + len + 1);
                if (!fragmentShader) {
                    printf("Memory allocation failed\n");
                    exit(1);
                }
                strcpy(fragmentShader + fragmentSize, buffer);
                fragmentSize += len;
            }
        }
    }

    fclose(file);

    // Ensure the buffers are null-terminated
    if (vertexShader) {
        vertexShader[vertexSize] = '\0';
    }
    if (fragmentShader) {
        fragmentShader[fragmentSize] = '\0';
    }

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


void GLDrawBox(unsigned int shaderProgram, unsigned int texture, box* object){
    glUseProgram(shaderProgram);
    
    float positions[28] = {
        //bottom left
        object->xPos - object->size, (object->yPos - object->size)*RES_RATIO, object->color[0], object->color[1], object->color[2], 1.0f, 1.0f,
        //bottom right
        object->xPos + object->size, (object->yPos - object->size)*RES_RATIO, object->color[0], object->color[1], object->color[2], 0.0f, 1.0f,
        //top right
        object->xPos + object->size, (object->yPos + object->size)*RES_RATIO, object->color[0], object->color[1], object->color[2], 0.0f, 0.0f,
        //top left
        object->xPos - object->size, (object->yPos + object->size)*RES_RATIO, object->color[0], object->color[1], object->color[2], 1.0f, 0.0f,

    };

    switch (object->direction){
    case DOWN :
        positions[5] = 0.0f,
        positions[6] = 0.0f,
        positions[12] = 1.0f;
        positions[13] = 0.0f;
        positions[19] = 1.0f;
        positions[20] = 1.0f;
        positions[26] = 0.0f;
        positions[27] = 1.0f;
        break;
    case RIGHT :
        positions[5] = 0.0f,
        positions[6] = 1.0f,
        positions[12] = 0.0f;
        positions[13] = 0.0f;
        positions[19] = 1.0f;
        positions[20] = 0.0f;
        positions[26] = 1.0f;
        positions[27] = 1.0f;
        break;
    case LEFT :
        positions[5] = 1.0f,
        positions[6] = 0.0f,
        positions[12] = 1.0f;
        positions[13] = 1.0f;
        positions[19] = 0.0f;
        positions[20] = 1.0f;
        positions[26] = 0.0f;
        positions[27] = 0.0f;
        break;
    
    default:
        break;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBindVertexArray(vao[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_ID_array[texture]);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);  


    glDrawArrays(GL_QUADS, 0, 4);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



}

void GLDrawSnake(unsigned int shaderProgram, player* head){
    while (head != NULL){

        GLDrawBox(shaderProgram, head->texture, &head->position);
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
    glBufferData(GL_ARRAY_BUFFER, 28 * sizeof(float), NULL, GL_STATIC_DRAW);
        //position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);
        //color
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)(2 * sizeof(float)));
        //texture coordinate
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

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

void load_textures(){

    glGenTextures(4, texture_ID_array);
    glBindTexture(GL_TEXTURE_2D, texture_ID_array[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;

    unsigned char *data = stbi_load("../assets/snake_head.png", &width, &height, &nrChannels, 0);
    if (data){
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        printf("Texture loaded successfully: %d x %d, channels: %d\n", width, height, nrChannels);

    }else{
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);



    glBindTexture(GL_TEXTURE_2D, texture_ID_array[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    data = stbi_load("../assets/snake_body_1.png", &width, &height, &nrChannels, 0);
    if (data){
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        printf("Texture loaded successfully: %d x %d, channels: %d\n", width, height, nrChannels);

    }else{
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);



    glBindTexture(GL_TEXTURE_2D, texture_ID_array[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    data = stbi_load("../assets/snake_body_2.png", &width, &height, &nrChannels, 0);
    if (data){
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        printf("Texture loaded successfully: %d x %d, channels: %d\n", width, height, nrChannels);

    }else{
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);


    glBindTexture(GL_TEXTURE_2D, texture_ID_array[3]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    data = stbi_load("../assets/snake_food.png", &width, &height, &nrChannels, 0);
    if (data){
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        printf("Texture loaded successfully: %d x %d, channels: %d\n", width, height, nrChannels);

    }else{
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);


}


void load_fonts(){
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        fprintf(stderr, "Could not init FreeType Library\n");
        exit(EXIT_FAILURE);
    }

    FT_Face face;
    if (FT_New_Face(ft, "../assets/Roboto-Regular.ttf", 0, &face)) {
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