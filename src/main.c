#include <glew.h>
#include <glfw3.h>
#include <arhcam.h>
#include <window.h>
#include <arhblocks.h>
#include <controls.h>
#include <misc.h>
#include <ui.h>

int main() {
    InitWindow();
    InitControls(window);
    InitCamera(1280, 720);
    
    InitChunks();
    InitCrosshair();

    InitUI();

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderChunks();
        RenderCrosshair();
        RenderUI();
        
        ControlsUpdate();

        glfwSwapBuffers(window);        
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
