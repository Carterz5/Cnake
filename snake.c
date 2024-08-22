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
    srand(time(0));

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
   
    player p1;
    p1.position.xPos = P1_XSTART;
    p1.position.yPos = P1_YSTART;
    p1.speed = SPEED_DEFAULT;
    p1.position.size = 0.025;
    p1.position.color[0] = 1.0;
    p1.position.color[1] = 0.0;
    p1.position.color[2] = 0.0;
    
    box coin;
    coin.xPos = -0.2;
    coin.yPos = -0.2;
    coin.size = 0.02;
    coin.color[0] = 1.0;
    coin.color[1] = 1.0;
    coin.color[2] = 0.0;

    unsigned int shader = SetShader();

    int framecount = 1;
    int tick = 0;
    unsigned int difficulty = 20;
    double lasttime = glfwGetTime();
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        GLDrawBox(&p1.position);
        GLDrawBox(&coin);

        create_coin(&p1, &coin, &difficulty);

        while (glfwGetTime() < lasttime + 1.0/TARGET_FPS) {

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
        if(framecount > 120){
            framecount = 1;
        }
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}


