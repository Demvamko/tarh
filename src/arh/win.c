#include <lib/glew.h>
#include <glfw3.h>
#include <arh/win.h>
#include <stdio.h>

GLFWwindow* window;

void __stdcall GLErrorCallback(uint stc, uint type, uint id, uint sev, int len, const char* msg, const void* param){
    printf("%s", msg);
}

void InitWindow(){
    glfwInit();

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

    window = glfwCreateWindow(1280, 720, "tArh", 0, 0);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window); 
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLErrorCallback, 0);

    glClearColor(0, 0, 0, 1);

    glLineWidth(1);
    glPointSize(4.0f);
}
