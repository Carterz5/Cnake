#include "gameplay.h"
#include "audio.h"


int UpState = 0;
int DownState = 0;
int LeftState = 0;
int RightState = 0;
int SpaceState = 0;
int debug = 0;
static Keypress last = NOTHING;
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
    case GLFW_KEY_SPACE:
        SpaceState = action;
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
            p1->position.direction = last;
            move_snake(p1);
            p1->position.yPos += p1->speed;
            
        } else{
            reset_game(p1);
        }
        break;

    case DOWN:
        if(p1->position.yPos - p1->position.size - p1->speed >= -1.01f/RES_RATIO){
            p1->position.direction = last;
            move_snake(p1);
            p1->position.yPos -= p1->speed;
            
        } else{
            reset_game(p1);
        }
        break;

    case LEFT:
        if(p1->position.xPos - p1->position.size - p1->speed >= -1.001f){
            p1->position.direction = last;
            move_snake(p1);
            p1->position.xPos -= p1->speed;
            
        } else{
            reset_game(p1);
        }
        break;

    case RIGHT:
        if(p1->position.xPos + p1->position.size + p1->speed <= 1.001f){
            p1->position.direction = last;
            move_snake(p1);
            p1->position.xPos += p1->speed;
            
        } else{
            reset_game(p1);
        }
        break;

    default:
        break;
    }
}

void process_inputs(player* head){
    static int inputcooldown = 0;
    if(UpState > GLFW_RELEASE && last != DOWN && inputcooldown == 0){
        last = UP;
        inputcooldown = 2;
    }
    if(DownState > GLFW_RELEASE && last != UP && inputcooldown == 0){
        last = DOWN;
        inputcooldown = 2;
    }
    if(LeftState > GLFW_RELEASE && last != RIGHT && inputcooldown == 0){
        last = LEFT;
        inputcooldown = 2;
    }
    if(RightState > GLFW_RELEASE && last != LEFT && inputcooldown == 0){
        last = RIGHT;
        inputcooldown = 2;
    }
    if(SpaceState > GLFW_RELEASE && debug == 1 && inputcooldown == 0){
        score += 1;
        grow_snake(head);
        inputcooldown = 10;
    }
    if(inputcooldown > 0){
        inputcooldown--;
    }

}

int create_coin(player* p1, box* coin, unsigned int* difficulty){
    static int scoreframe = 0;

    if(check_collision(*p1, *coin) == true){
        scoreframe += 1;
        if (scoreframe == 1){
            score +=1;
            play_sound("../assets/eating2.wav");
            if (score == 50){
                play_sound("../assets/greatjob.wav");
            } else if (score == 100){
                play_sound("../assets/100.wav");
            }
        
            grow_snake(p1);
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


    while(check_coin_spawn(p1, *coin) == true){

        double random_y = generate_random_in_range(-0.75 + coin->size, 0.75 - coin->size);
        double random_x = generate_random_in_range(-1.0 + coin->size, 1.0 - coin->size);

        coin->xPos = random_x;
        coin->yPos = random_y;

    }




    return score;

}

void init_objects(box* coin){


    coin->xPos = -0.2;
    coin->yPos = -0.2;
    coin->size = 0.02;
    coin->color[0] = 1.0;
    coin->color[1] = 0.0;
    coin->color[2] = 0.0;
    coin->direction = UP;


}


bool check_collision(player player, box box){
    

    // Check if one quad is to the left of the other
    if (box.xPos + box.size - 0.002 <= player.position.xPos - player.position.size + 0.002 || player.position.xPos + player.position.size - 0.002 <= box.xPos - box.size + 0.002) {
        return false;
    }
    
    // Check if one quad is above the other
    if (box.yPos + box.size - 0.002 <= player.position.yPos - player.position.size + 0.002 || player.position.yPos + player.position.size - 0.002 <= box.yPos - box.size + 0.002) {
        return false;
    }

    return true;

}


player* create_snake_node(){
    player* result = malloc(sizeof(player));
    result->position.xPos = P1_XSTART;
    result->position.yPos = P1_YSTART;
    result->position.size = 0.025;
    result->position.color[0] = 0.0;
    result->position.color[1] = 1.0;
    result->position.color[2] = 0.0;
    result->position.direction = last;
    result->texture = 0;
    result->speed = SPEED_DEFAULT;
    result->next = NULL;
    return result;
}

void grow_snake(player* head){
    float random_number = generate_random_in_range(0.0, 1.0);
    player* part = create_snake_node();
    player* temp = head;

    while(temp->next != NULL){
        temp = temp->next;
    }

    part->position.xPos = temp->position.xPos;
    part->position.yPos = temp->position.yPos;
    if (random_number > 0.5) {
        part->texture = 1;
    } else {
        part->texture = 2;
    }
    
    temp->next = part;

}

void move_snake(player* head){
    player* temp = head;
    float x = temp->position.xPos;
    float y = temp->position.yPos;
    Keypress direction = temp->position.direction;
    float oldx;
    float oldy;
    Keypress old_direction;

    while(temp->next != NULL){
        temp = temp->next;
        oldx = temp->position.xPos;
        oldy = temp->position.yPos;
        old_direction = temp->position.direction;
        temp->position.xPos = x;
        temp->position.yPos = y;
        temp->position.direction = direction;
        x = oldx;
        y = oldy;
        direction = old_direction;

    }

}
void destroy_snake(player* head){
    play_sound("../assets/ouch.wav");
    if (head->next != NULL){
        player* temp = head->next;
        player* prev;
        player* next;

        while(temp->next != NULL){
            next = temp->next;
            prev = temp;
            free(prev);
            temp = next;
        }
        free(temp);
        head->next = NULL;
    }



}

bool check_self_collide(player* head){
    unsigned int loops = 0;
    if (head->next != NULL){
        player* temp = head;


        while(temp->next != NULL){
            temp = temp->next;
            if (check_collision(*head, temp->position) == true && loops > 0){
                printf("head x %f, head y %f\n",head->position.xPos, head->position.yPos);
                printf("temp x %f, temp y %f\n",temp->position.xPos, temp->position.yPos);
                return true;

            }
            loops++;
        }
    }

    return false;

}

void reset_game(player* p1){

    score = 0;
    destroy_snake(p1);
    p1->position.xPos = P1_XSTART;
    p1->position.yPos = P1_YSTART;
    last = NOTHING;


}

bool check_coin_spawn(player* p1, box coin){
    player* temp = p1;

    while(temp->next != NULL){
        if (check_collision(*temp, coin) == true){
            return true;
        }
        temp = temp->next;


    }

    if (check_collision(*temp, coin) == true){
        return true;
    } else {
        return false;
    }



}