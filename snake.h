#include <stdbool.h>
#include <GLFW/glfw3.h>

#define RES_X 1024.0f
#define RES_Y 768.0f
#define RES_RATIO (RES_X / RES_Y)
#define P1_XSTART 0.025f
#define P1_YSTART 0.025f

typedef struct {
    float xPos;
    float yPos;
    float color[3];
    float size;
} box;

typedef struct {
    box position;
    float speed;
} player;




//Graphics

typedef enum {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
} ShaderType;


typedef struct {
    char* VertexSource;
    char* FragmentSource;
} ShaderProgramSource;


ShaderProgramSource ParseShader(const char* filepath);
unsigned int CompileShader(unsigned int type, const char* source);
unsigned int CreateShader(const char* vertexShader, const char* fragmentShader);
void GLDrawBox(box object);
unsigned int SetShader();





//Gameplay
extern int UpState;
extern int DownState;
extern int LeftState;
extern int RightState;

typedef enum {
   NOTHING = 0, UP = 1, DOWN = 2, LEFT = 3, RIGHT = 4
} Keypress;

#define SPEED_DEFAULT  0.05
#define TARGET_FPS 60




void process_inputs(player* p1);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
bool check_collision(player player, box box);
void create_coin(player* p1, box* coin, int* difficulty);
void process_movement(player* p1);
