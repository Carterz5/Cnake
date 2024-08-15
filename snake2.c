// test.c
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define SPEED_DEFAULT  0.01
#define TARGET_FPS 60
// damn colors are cool i wish i had them.
typedef struct {
    char* VertexSource;
    char* FragmentSource;
} ShaderProgramSource;

typedef enum {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
} ShaderType;

typedef struct {
    float xMin;
    float xMax;
    float yMin;
    float yMax;
    float xPos;
    float yPos;
    float size;
    float speed;
} player;

typedef struct {
    float xMin;
    float xMax;
    float yMin;
    float yMax;
    float xPos;
    float yPos;
    float size;
} box;

int UpState = 0;
int DownState = 0;
int LeftState = 0;
int RightState = 0;
int SpaceState = 0;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    
    switch (key){
    case GLFW_KEY_UP:
        UpState = action;
        break;
    case GLFW_KEY_DOWN:
        DownState = action;
        break;
    case GLFW_KEY_LEFT:
        LeftState = action;
        break;
    case GLFW_KEY_RIGHT:
        RightState = action;
        break;
    case GLFW_KEY_SPACE:
        SpaceState = action;
        break;
    default:
        break;
    }

    

}

bool check_collision(player player, box box){
    

    // Check if one quad is to the left of the other
    if (box.xMax < player.xMin || player.xMax < box.xMin) {
        return false;
    }
    
    // Check if one quad is above the other
    if (box.yMax < player.yMin || player.yMax < box.yMin) {
        return false;
    }

    return true;

}


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

static unsigned int CompileShader(unsigned int type, const char* source){

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

static unsigned int CreateShader(const char* vertexShader, const char* fragmentShader){

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

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;





    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1024, 768, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK){
        printf("Error!\n");
    }
    const char *glversion = glGetString(GL_VERSION);
    printf("GL version is |%s|\n", glversion);

    glfwSetKeyCallback(window, key_callback);

    int framecount = 0;
    int score = 0;

    player p1;
    p1.xPos = 0.0;
    p1.yPos = 0.0;
    p1.speed = SPEED_DEFAULT;
    p1.size = 0.08;
    
    box coin;
    coin.xPos = 0.0;
    coin.yPos = 0.0;
    coin.size = 0.02;


    ShaderProgramSource source = ParseShader("./res/shaders/Basic.shader");

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);


    unsigned int buffer;
    glGenBuffers(1, &buffer);

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    
    int tick = 0;
    unsigned int boostTick = 0;
    double lasttime = glfwGetTime();
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        
        float positions[16] = {
            //player
            p1.xPos - p1.size, (p1.yPos - p1.size)*1.333,
            p1.xPos + p1.size, (p1.yPos - p1.size)*1.333,
            p1.xPos + p1.size, (p1.yPos + p1.size)*1.333,
            p1.xPos - p1.size, (p1.yPos + p1.size)*1.333,

            //box
            coin.xPos - coin.size, (coin.yPos - coin.size)*1.333,
            coin.xPos + coin.size, (coin.yPos - coin.size)*1.333,
            coin.xPos + coin.size, (coin.yPos + coin.size)*1.333,
            coin.xPos - coin.size, (coin.yPos + coin.size)*1.333,

        };


        p1.xMax = p1.xPos + p1.size;
        p1.xMin = p1.xPos - p1.size;
        p1.yMax = p1.yPos + p1.size;
        p1.yMin = p1.yPos - p1.size;


        coin.xMax = coin.xPos + coin.size;
        coin.xMin = coin.xPos - coin.size;
        coin.yMax = coin.yPos + coin.size;
        coin.yMin = coin.yPos - coin.size;

        // tick += 1;
        // if (tick > 1000){
        //     printf("ymax is %f\n", p1.yMax);
        //     printf("ymin is %f\n", p1.yMin);
        //     printf("xmax is %f\n", p1.xMax);
        //     printf("xmin is %f\n", p1.xMin);
        //     tick = 0;
        // }




        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBindVertexArray(vao);
        glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), positions, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
        glDrawArrays(GL_QUADS, 0, 8);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        if(check_collision(p1, coin) == true){
            //printf("I have collided!\n");
            framecount += 1;
            if (framecount == 1){
                score +=1;
                printf("Score: %d\n", score);
            }

            srand(time(0));

            double random_y = ((double)rand() / RAND_MAX) * ((0.75 - coin.size) * 2) - (0.75 - coin.size);
            double random_x = ((double)rand() / RAND_MAX) * ((1 - coin.size) * 2) - (1 - coin.size);

            coin.xPos = random_x;
            coin.yPos = random_y;

        } else {
            framecount = 0;
        }


        while (glfwGetTime() < lasttime + 1.0/TARGET_FPS) {
        // TODO: Put the thread to sleep, yield, or simply do nothing
        }
        lasttime += 1.0/TARGET_FPS;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        
        if(UpState > GLFW_RELEASE && p1.yMax + p1.speed < 0.75){
            p1.yPos += p1.speed;
        }
        if(DownState > GLFW_RELEASE && p1.yMin - p1.speed > -0.75){
            p1.yPos -= p1.speed;
        }
        if(RightState > GLFW_RELEASE && p1.xMax + p1.speed < 1){
            p1.xPos += p1.speed;
        }
        if(LeftState > GLFW_RELEASE && p1.xMin - p1.speed > -1){
            p1.xPos -= p1.speed;
        }
        if(SpaceState > GLFW_RELEASE && boostTick < 100){
            p1.speed = SPEED_DEFAULT * 2;
            boostTick += 1;
            //printf("boost is %d\n", boostTick);
        } else {
            p1.speed = SPEED_DEFAULT;
            if (boostTick > 0 && SpaceState == GLFW_RELEASE){
                boostTick -= 1;
            }
            
        }


    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}


