#include <glfw3.h>
#include <arh/cam.h>
#include <arh/win.h>
#include <game/controls.h>
#include <voxel/voxels.h>
#include <voxel/types.h>
#include <voxel/def.h>
#include <game/game_ui.h>

static float lastx = -1;
static float lasty = -1;

int sel_block = BLOCK_STONE;

void Arh_Controls_Init(){
    glfwSetKeyCallback(window, OnKey);
    glfwSetCursorPosCallback(window, OnMouseMove);
    glfwSetMouseButtonCallback(window, OnMouseButton);
}

void Arh_Controls_Update(){
    float move[3] = { 0 };

    #define pressed(key) (glfwGetKey(window, key) == GLFW_PRESS)

    if(pressed(GLFW_KEY_W))
        move[0] += MOVESPEED;
    if(pressed(GLFW_KEY_S))
        move[0] -= MOVESPEED;

    if(pressed(GLFW_KEY_D))
        move[1] += MOVESPEED;
    if(pressed(GLFW_KEY_A))
        move[1] -= MOVESPEED;

    if(pressed(GLFW_KEY_SPACE))
        move[2] += MOVESPEED;
    if(pressed(GLFW_KEY_LEFT_SHIFT))
        move[2] -= MOVESPEED;

    #undef pressed

    Arh_Camera_Move(move);
}


void OnKey(GLFWwindow* window, int key, int code, int action, int mods){
    if(action != GLFW_PRESS)
        return;

    switch(key){
    case GLFW_KEY_ESCAPE:
    case GLFW_KEY_TAB:
        int inp_mode = glfwGetInputMode(window, GLFW_CURSOR);

        if(inp_mode == GLFW_CURSOR_NORMAL)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        lastx = -1;
        lasty = -1;

        break;
    }
}

void OnMouseMove(GLFWwindow* window, double x, double y){
    int inp_mode = glfwGetInputMode(window, GLFW_CURSOR);

    if(inp_mode == GLFW_CURSOR_NORMAL){
        Arh_UI_Game_MouseMove(x, y);
        return;
    }

    if(lastx == -1){
        lastx = x;
        lasty = y;
        return;
    }

    float rx = (lastx - x) * MOUSE_SENSITIVITY;
    float ry = (lasty - y) * MOUSE_SENSITIVITY;

    lastx = x;
    lasty = y;

    Arh_Camera_Rotate(rx, ry);
}

void OnMouseButton(GLFWwindow* window, int button, int action, int mods){
    int inp_mode = glfwGetInputMode(window, GLFW_CURSOR);

    if(inp_mode == GLFW_CURSOR_NORMAL){
        double x = 0;
        double y = 0;
        glfwGetCursorPos(window, &x, &y);

        Arh_UI_Game_MouseClick(x, y);
        return;
    }

    if(action != GLFW_PRESS)
        return;

    #define ARGS(vec) vec[0], vec[1], vec[2]

    int rayhit[3] = { 0 };
    int rayprev[3] = { 0 };

    Voxel_RayCast_Vector(camera.pos, camera.front, 50, rayhit, rayprev);

    if(button == GLFW_MOUSE_BUTTON_LEFT)
        Voxel_Set(ARGS(rayhit), BLOCK_AIR);

    if(button == GLFW_MOUSE_BUTTON_RIGHT)
        Voxel_Set(ARGS(rayprev), sel_block);

    #undef ARGS
}