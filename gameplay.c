#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GLFW/glfw3.h>
#include "snake.h"


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