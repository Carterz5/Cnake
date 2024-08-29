#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include "snake.h"





#define P1_XSTART 0.025f
#define P1_YSTART 0.025f
#define SPEED_DEFAULT  0.05



typedef enum {
   NOTHING = 0, UP = 1, DOWN = 2, LEFT = 3, RIGHT = 4
} Keypress;




void init_objects(box* coin);
void process_inputs();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
bool check_collision(player player, box box);
int create_coin(player* p1, box* coin, unsigned int* difficulty);
void process_movement(player* p1);
player* create_snake_node();
void grow_snake(player* head);
void move_snake(player* head);
void destroy_snake(player* head);
bool check_self_collide(player* head);
void reset_game(player* p1);

#endif