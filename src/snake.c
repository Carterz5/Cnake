// test.c
#include <time.h>
#include "graphics.h"
#include "gameplay.h"

#define TARGET_FPS 60
int digits(int n);
void append_score(char* text, int score);



int main(void){
    GLFWwindow* window = init_glfw();
    init_opengl();
    load_fonts();
    load_textures();
    srand(time(0));
   
    player* p1 = create_snake_node();
    box coin;
    init_objects(&coin);

    unsigned int basic_shader = SetShaders("../shaders/Basic.glsl");
    unsigned int text_shader = SetShaders("../shaders/Text.glsl");

    int framecount = 1;
    unsigned int difficulty = 20;
    int score = 0;
    double lasttime = glfwGetTime();
    float textcolor[3] = {1.0f, 1.0f, 1.0f};
    char scoretext[20] = "Score: ";
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        GLDrawSnake(basic_shader, p1);
        GLDrawBox(basic_shader, 3, &coin);

        RenderText(text_shader,scoretext, 25.0f, 720.0f, 0.8f,textcolor);




        score = create_coin(p1, &coin, &difficulty);

        while (glfwGetTime() < lasttime + 1.0/TARGET_FPS) {

        }
        lasttime += 1.0/TARGET_FPS;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        
        process_inputs(p1);
        
        if (framecount % difficulty == 0){
            process_movement(p1);
            if (check_self_collide(p1) == true ){
                reset_game(p1);
            }
        }

        append_score(scoretext, score);

        framecount += 1;
        if(framecount > 120){
            framecount = 1;
        }
    }

    glDeleteProgram(basic_shader);
    glDeleteProgram(text_shader);

    glfwTerminate();
    return 0;
}

void append_score(char* text, int score){
    int scorelen = digits(score);
    char numberstr[20];
    sprintf(numberstr, "%d", score);

    for (int i = 0; i <= scorelen; i++){
        text[7 + i] = numberstr[i];
    }
   

}

int digits(int n){
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    if (n < 1000000) return 6;
    return 0;
}

