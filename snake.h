#include <stdbool.h>
//#include <GL/glew.h>
#include <GLFW/glfw3.h>



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





//Gameplay

typedef struct {
    float xMin;
    float xMax;
    float yMin;
    float yMax;
    float xPos;
    float yPos;
    float size;
    float speed;
} player;

typedef struct {
    float xMin;
    float xMax;
    float yMin;
    float yMax;
    float xPos;
    float yPos;
    float size;
} box;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
bool check_collision(player player, box box);



extern int UpState;
extern int DownState;
extern int LeftState;
extern int RightState;
extern int SpaceState;
