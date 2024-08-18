// test.c
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct object {
    float xmin;
    float xmax;
    float ymin;
    float ymax;
};

bool check_collision(struct object player, struct object box);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

float xOffset = 0;
float yOffset = 0;
int upOn = 0;
int rightOn = 0;
int leftOn = 0;
int downOn = 0;
float speed = 0.001;

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

    struct object player;
    struct object box;
    int score = 0;
    int framecount = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //player

        glBegin(GL_QUADS);
        glColor3f(0.0, 1.0, 0.0);
        glVertex2f(-0.1 + xOffset, (-0.1 + yOffset)*1.333);
        glColor3f(0.0, 1.0, 0.0);
        glVertex2f(0.1 + xOffset, (-0.1 + yOffset)*1.333);
        glColor3f(0.0, 1.0, 0.0);
        glVertex2f(0.1 + xOffset, (0.1 + yOffset)*1.333);
        glColor3f(0.0, 1.0, 0.0);
        glVertex2f(-0.1 + xOffset, (0.1 + yOffset)*1.333);
        glEnd();
   
        player.xmax = 0.1 + xOffset;
        player.xmin = -0.1 + xOffset;
        player.ymax = (0.1 + yOffset)*1.333;
        player.ymin = (-0.1 + yOffset)*1.333;


        //box
        glBegin(GL_QUADS);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(0.0, 0.2);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(0.2, 0.2);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(0.2, 0.4);
        glColor3f(1.0, 0.0, 0.0);
        glVertex2f(0.0, 0.4);
        glEnd();

        box.xmax = 0.2;
        box.xmin = 0.0;
        box.ymax = 0.4;
        box.ymin = 0.2;

        if(check_collision(player, box) == true){
            //printf("I have collided!\n");
            framecount += 1;
            if (framecount > 5000 || framecount == 1){
                score +=1;
                printf("Score: %d\n", score);
                if(framecount != 1){
                    framecount = 0;
                }
            }  

        } else {
            framecount = 0;
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        if(upOn == 1){
            yOffset += speed;
        }
        if(downOn == 1){
            yOffset -= speed;
        }
        if(rightOn == 1){
            xOffset += speed;
        }
        if(leftOn == 1){
            xOffset -= speed;
        }


    }

    glfwTerminate();
    return 0;
}

bool check_collision(struct object player, struct object box){
    

        // Check if one quad is to the left of the other
    if (box.xmax < player.xmin || player.xmax < box.xmin) {
        return false;
    }
    
    // Check if one quad is above the other
    if (box.ymax < player.ymin || player.ymax < box.ymin) {
        return false;
    }

    return true;

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){

    if (key == GLFW_KEY_UP && action == GLFW_PRESS){
        //printf("Up is pressed\n");
        upOn = 1;
    }
    if (key == GLFW_KEY_UP && action == GLFW_RELEASE){
        //printf("Up is released\n");
        upOn = 0;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS){
        //printf("Down is pressed\n");
        downOn = 1;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE){
        //printf("Down is released\n");
        downOn = 0;
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
        //printf("Right is pressed\n");
        rightOn = 1;
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE){
        //printf("Right is released\n");
        rightOn = 0;
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
        //printf("Left is pressed\n");
        leftOn = 1;
    }    
    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE){
        //printf("Left is released\n");
        leftOn = 0;
    }    

}


