#ifndef SNAKE_H
#define SNAKE_H
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <GLFW/glfw3.h>

#define RES_X 1024.0f
#define RES_Y 768.0f
#define RES_RATIO (RES_X / RES_Y)


typedef struct {
    float xPos;
    float yPos;
    float color[3];
    float size;
} box;

typedef struct player {
    box position;
    float speed;
    struct player* next;
} player;




//Graphics

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);





//Gameplay


#endif