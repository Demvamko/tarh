#include <glew.h>
#include <glfw3.h>
#include <arhgl.h>
#include <arhblocks.h>
#include <cglm.h>
#include <arhcam.h>

static void RecalcCamera();
static void OnKey(GLFWwindow* window, int key, int code, int action, int mods);
static void OnMouseMove(GLFWwindow* window, double x, double y);

int main() {
    glfwInit();

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

    GLFWwindow* window = glfwCreateWindow(1280, 720, "tArh", 0, 0);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, OnKey);
    glfwSetCursorPosCallback(window, OnMouseMove);

    glfwMakeContextCurrent(window); 
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // BlockVertices* verts = CubeGen();

    Chunk c;
    ChunkGen(&c);
    int size = 0;
    BlockVertices* verts = ChunkToGeometry(&c, &size);

    Attributes attribs[] = { 
        { 3, GL_FLOAT, 0, 7 * sizeof(int), 0 }, 
        { 2, GL_FLOAT, 0, 7 * sizeof(int), 3 * sizeof(float) }, 
        { 2, GL_FLOAT, 0, 7 * sizeof(int), 5 * sizeof(float) }, 
        { 0 }
    };

    Buffer buff = CreateBuffer(sizeof(BlockVertices), size, GL_TRIANGLES, verts, attribs);

    ArhCamInit(1280, 720);

    uint blocks_shader = CreateShader("./res/glsl/blocks.glsl");
    glUseProgram(blocks_shader);
    ArhCamBindUniform();

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderBuffer(&buff);

        glfwSwapBuffers(window);
        
        glfwPollEvents();

        vec3 movement = { 0 };
        float keys[4] = { 0 };

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            movement[0] += 0.005f;
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            movement[0] -= 0.005f;

        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            movement[1] += 0.005f;
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            movement[1] -= 0.005f;

        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            movement[2] += 0.005f;
        if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            movement[2] -= 0.005f;

        ArhCamMove(movement);

        ArhCamUpdateView();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
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
