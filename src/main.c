#include <glew.h>
#include <glfw3.h>
#include <arhgl.h>
#include <cglm.h>
#include <arhcam.h>
#include <window.h>
#include <arhblocks.h>
#include <arhblock_defines.h>
#include <arhblock_types.h>

static void RecalcCamera();
static void OnKey(GLFWwindow* window, int key, int code, int action, int mods);
static void OnMouseMove(GLFWwindow* window, double x, double y);
static void UpdateMovement();
static void OnLButton(GLFWwindow* window, int button, int action, int mods);

void RayCast(vec3 start, vec3 dir, int maxdist);

int main() {
    InitWindow();

    glfwSetKeyCallback(window, OnKey);
    glfwSetCursorPosCallback(window, OnMouseMove);
    glfwSetMouseButtonCallback(window, OnLButton);

    ArhCamInit(1280, 720);
    ArhCamBindUniform();

    for(int y = 0; y < 4; y++)
    for(int x = 0; x < 4; x++){
        Chunk* c = LoadChunk(x, y);
    }

    uint blocks_shader = CreateShader("./res/glsl/blocks.glsl");
    uint pointer_shader = CreateShader("./res/glsl/pointer.glsl");
    uint basic_shader = CreateShader("./res/glsl/basic.glsl");

    CreateImgTexture("./res/img/tileset.png", 0);

    Attributes attr[2] = {{ 2, GL_FLOAT }, { 0 }};
    Buffer crosshair = CreateBuffer(sizeof(float) * 2, 1, GL_POINTS, (float[]) { 0.0f, 0.0f }, attr);

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(blocks_shader);

        for(int y = 0; y < 4; y++)
        for(int x = 0; x < 4; x++){
            RenderChunk(FindChunk(x, y));
        }

        RayCast(*ArhCamGetPos(), *ArhCamGetFront(), 50);

        glUseProgram(basic_shader);
        RenderBuffer(&crosshair);

        glfwSwapBuffers(window);
        
        glfwPollEvents();
        UpdateMovement();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}

#define MOVESPEED 0.05f

static void UpdateMovement(){
    vec3 movement = { 0 };
    float keys[4] = { 0 };

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movement[0] += MOVESPEED;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movement[0] -= MOVESPEED;

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movement[1] += MOVESPEED;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movement[1] -= MOVESPEED;

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        movement[2] += MOVESPEED;
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        movement[2] -= MOVESPEED;

    ArhCamMove(movement);

    ArhCamUpdateView();
}

static void OnKey(GLFWwindow* window, int key, int code, int action, int mods){
    if(action != GLFW_PRESS)
        return;

    switch(key){
    case GLFW_KEY_ESCAPE:
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        break;
    }
}

static float lastx = -1;
static float lasty = -1;

static void OnMouseMove(GLFWwindow* window, double x, double y){
    if(lastx == -1){
        lastx = x;
        lasty = y;
        return;
    }

    float rx = (lastx - x) * 0.001f;
    float ry = (lasty - y) * 0.001f;

    lastx = x;
    lasty = y;

    ArhCamRotate(rx, ry);
}

static void OnLButton(GLFWwindow* window, int button, int action, int mods){
    if(action != GLFW_PRESS)
        return;

    if(button == GLFW_MOUSE_BUTTON_LEFT){
        *GetBlockAbs(RX, RY, RZ) = TYPE_AIR;

        Chunk* c = GetChunkAbs(RX, RZ);
        c->dirty = 1;
    }

    if(button == GLFW_MOUSE_BUTTON_RIGHT){
        *GetBlockAbs(RPX, RPY, RPZ) = TYPE_DIRT;

        Chunk* c = GetChunkAbs(RPX, RPZ);
        c->dirty = 1;
    }

}