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

float generate_random_in_range(float min, float max) {
    return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}



void process_movement(player* p1){


    switch (last){
    case UP:
        if(p1->position.yPos + p1->position.size + p1->speed <= 1.01f/RES_RATIO){
            p1->position.yPos += p1->speed;
        } else{
            score = 0;
            p1->position.xPos = P1_XSTART;
            p1->position.yPos = P1_YSTART;
            last = NONE;
        }
        break;

    case DOWN:
        if(p1->position.yPos - p1->position.size - p1->speed >= -1.01f/RES_RATIO){
            p1->position.yPos -= p1->speed;
        } else{
            score = 0;
            p1->position.xPos = P1_XSTART;
            p1->position.yPos = P1_YSTART;
            last = NONE;
        }
        break;

    case LEFT:
        if(p1->position.xPos - p1->position.size - p1->speed >= -1.001f){
            p1->position.xPos -= p1->speed;
        } else{
            score = 0;
            p1->position.xPos = P1_XSTART;
            p1->position.yPos = P1_YSTART;
            last = NONE;
        }
        break;

    case RIGHT:
        if(p1->position.xPos + p1->position.size + p1->speed <= 1.001f){
            p1->position.xPos += p1->speed;
        } else{
            score = 0;
            p1->position.xPos = P1_XSTART;
            p1->position.yPos = P1_YSTART;
            last = NONE;
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

int create_coin(player* p1, box* coin, unsigned int* difficulty){
    static int scoreframe = 0;

    if(check_collision(*p1, *coin) == true){
        scoreframe += 1;
        if (scoreframe == 1){
            score +=1;
            //printf("Score: %d\n", score);
        }
        if (*difficulty > 5 && score % 2 == 0){
            *difficulty -= 5;
        }

 
    } else {
        scoreframe = 0;
    }

    if (score < 2){
        *difficulty = 20;
    }


    while(check_collision(*p1, *coin) == true){

        double random_y = generate_random_in_range(-0.75 + coin->size, 0.75 - coin->size);
        double random_x = generate_random_in_range(-1.0 + coin->size, 1.0 - coin->size);

        coin->xPos = random_x;
        coin->yPos = random_y;

    }




    return score;

}

void init_objects(player* p1, box* coin){

    p1->position.xPos = P1_XSTART;
    p1->position.yPos = P1_YSTART;
    p1->speed = SPEED_DEFAULT;
    p1->position.size = 0.025;
    p1->position.color[0] = 0.0;
    p1->position.color[1] = 1.0;
    p1->position.color[2] = 0.0;

    coin->xPos = -0.2;
    coin->yPos = -0.2;
    coin->size = 0.02;
    coin->color[0] = 1.0;
    coin->color[1] = 0.0;
    coin->color[2] = 0.0;


}


bool check_collision(player player, box box){
    

    // Check if one quad is to the left of the other
    if (box.xPos + box.size < player.position.xPos - player.position.size || player.position.xPos + player.position.size < box.xPos - box.size) {
        return false;
    }
    
    // Check if one quad is above the other
    if (box.yPos + box.size < player.position.yPos - player.position.size || player.position.yPos + player.position.size < box.yPos - box.size) {
        return false;
    }

    return true;

}