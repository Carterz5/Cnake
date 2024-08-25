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
    GLFWwindow* window = init_glfw();
    srand(time(0));

   
    player p1;
    box coin;
    init_objects(&p1, &coin);

    unsigned int shader = SetShader();

    int framecount = 1;
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


