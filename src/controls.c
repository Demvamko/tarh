#include <glfw3.h>
#include <arhcam.h>
#include <window.h>
#include <controls.h>
#include <voxels.h>
#include <voxel_types.h>
#include <voxel_defines.h>

static float lastx = -1;
static float lasty = -1;

void InitControls(){
    glfwSetKeyCallback(window, OnKey);
    glfwSetCursorPosCallback(window, OnMouseMove);
    glfwSetMouseButtonCallback(window, OnMouseButton);
}

void ControlsUpdate(){
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

    MoveCamera(move);
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

    if(inp_mode == GLFW_CURSOR_NORMAL)
        return;

    if(lastx == -1){
        lastx = x;
        lasty = y;
        return;
    }

    float rx = (lastx - x) * MOUSE_SENSITIVITY;
    float ry = (lasty - y) * MOUSE_SENSITIVITY;

    lastx = x;
    lasty = y;

    RotateCamera(rx, ry);
}

void OnMouseButton(GLFWwindow* window, int button, int action, int mods){
    if(action != GLFW_PRESS)
        return;

    #define ARGS(vec) vec[0], vec[1], vec[2]

    int rayhit[3] = { 0 };
    int rayprev[3] = { 0 };

    Voxel_RayCast_Vector(camera_pos, camera_front, 50, rayhit, rayprev);

    if(button == GLFW_MOUSE_BUTTON_LEFT)
        Voxel_Set(ARGS(rayhit), TYPE_AIR);

    if(button == GLFW_MOUSE_BUTTON_RIGHT)
        Voxel_Set(ARGS(rayprev), TYPE_DIRT);

    #undef ARGS
}