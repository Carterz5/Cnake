// test.c
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "snake.h"



int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;





    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(RES_X, RES_Y, "Hello World", NULL, NULL);
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

    int framecount = 1;
    
    player p1;
    p1.xPos = 0.025;
    p1.yPos = 0.025;
    p1.speed = SPEED_DEFAULT;
    p1.size = 0.025;
    
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
    unsigned int difficulty = 30;
    double lasttime = glfwGetTime();
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        
        float positions[48] = {
            //player
            p1.xPos - p1.size, (p1.yPos - p1.size)*RES_RATIO, 1.0, 0.0, 0.0,
            p1.xPos + p1.size, (p1.yPos - p1.size)*RES_RATIO, 1.0, 0.0, 0.0,
            p1.xPos + p1.size, (p1.yPos + p1.size)*RES_RATIO, 1.0, 0.0, 0.0,
            p1.xPos - p1.size, (p1.yPos + p1.size)*RES_RATIO, 1.0, 0.0, 0.0,

            //box
            coin.xPos - coin.size, (coin.yPos - coin.size)*RES_RATIO, 1.0, 1.0, 0.0,
            coin.xPos + coin.size, (coin.yPos - coin.size)*RES_RATIO, 1.0, 1.0, 0.0,
            coin.xPos + coin.size, (coin.yPos + coin.size)*RES_RATIO, 1.0, 1.0, 0.0,
            coin.xPos - coin.size, (coin.yPos + coin.size)*RES_RATIO, 1.0, 1.0, 0.0,

        };


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


        create_coin(&p1, &coin, &difficulty);

        while (glfwGetTime() < lasttime + 1.0/TARGET_FPS) {
        // TODO: Put the thread to sleep, yield, or simply do nothing
        }
        lasttime += 1.0/TARGET_FPS;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        
        process_inputs(&p1);
        
        if (framecount % difficulty == 0){
            process_movement(&p1);
        }

        framecount += 1;
        if(framecount > 60){
            framecount = 1;
        }
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}


