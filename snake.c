// test.c
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "snake.h"

#define SPEED_DEFAULT  0.01
#define TARGET_FPS 60

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
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    
    int tick = 0;
    unsigned int boostTick = 0;
    double lasttime = glfwGetTime();
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        
        float positions[48] = {
            //player
            p1.xPos - p1.size, (p1.yPos - p1.size)*1.333, 1.0, 0.0, 0.0,
            p1.xPos + p1.size, (p1.yPos - p1.size)*1.333, 1.0, 0.0, 0.0,
            p1.xPos + p1.size, (p1.yPos + p1.size)*1.333, 1.0, 0.0, 0.0,
            p1.xPos - p1.size, (p1.yPos + p1.size)*1.333, 1.0, 0.0, 0.0,

            //box
            coin.xPos - coin.size, (coin.yPos - coin.size)*1.333, 1.0, 1.0, 0.0,
            coin.xPos + coin.size, (coin.yPos - coin.size)*1.333, 1.0, 1.0, 0.0,
            coin.xPos + coin.size, (coin.yPos + coin.size)*1.333, 1.0, 1.0, 0.0,
            coin.xPos - coin.size, (coin.yPos + coin.size)*1.333, 1.0, 1.0, 0.0,

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
        glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(float), positions, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(2 * sizeof(float)));
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


