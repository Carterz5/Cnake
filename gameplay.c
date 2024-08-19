#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <GLFW/glfw3.h>
#include "snake.h"


int UpState = 0;
int DownState = 0;
int LeftState = 0;
int RightState = 0;
static Keypress last = NONE;
int score = 0;

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
    default:
        break;
    }

    

}

void process_movement(player* p1){

    switch (last){
    case UP:
        if(p1->yPos + p1->size + p1->speed < 1.0f/RES_RATIO){
            p1->yPos += p1->speed;
        }
        break;

    case DOWN:
        if(p1->yPos - p1->size - p1->speed > -1.0f/RES_RATIO){
            p1->yPos -= p1->speed;
        }
        break;

    case LEFT:
        if(p1->xPos - p1->size - p1->speed > -1){
            p1->xPos -= p1->speed;
        }
        break;

    case RIGHT:
        if(p1->xPos + p1->size + p1->speed < 1){
            p1->xPos += p1->speed;
        }
        break;

    default:
        break;
    }
}

void process_inputs(player* p1){

    if(UpState > GLFW_RELEASE){
        last = UP;
    }
    if(DownState > GLFW_RELEASE){
        last = DOWN;
    }
    if(LeftState > GLFW_RELEASE){
        last = LEFT;
    }
    if(RightState > GLFW_RELEASE){
        last = RIGHT;
    }


}

void create_coin(player* p1, box* coin){
    static int framecount = 0;

    if(check_collision(*p1, *coin) == true){
        //printf("I have collided!\n");
        framecount += 1;
        if (framecount == 1){
            score +=1;
            printf("Score: %d\n", score);
        }

        srand(time(0));

        double random_y = ((double)rand() / RAND_MAX) * ((0.75 - coin->size) * 2) - (0.75 - coin->size);
        double random_x = ((double)rand() / RAND_MAX) * ((1 - coin->size) * 2) - (1 - coin->size);

        coin->xPos = random_x;
        coin->yPos = random_y;

    } else {
        framecount = 0;
    }




}




bool check_collision(player player, box box){
    

    // Check if one quad is to the left of the other
    if (box.xPos + box.size < player.xPos - player.size || player.xPos + player.size < box.xPos - box.size) {
        return false;
    }
    
    // Check if one quad is above the other
    if (box.yPos + box.size < player.yPos - player.size || player.yPos + player.size < box.yPos - box.size) {
        return false;
    }

    return true;

}