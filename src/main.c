#include <lib/glew.h>
#include <glfw3.h>
#include <arh/arhcam.h>
#include <arh/arhwin.h>
#include <voxel/voxels.h>
#include <game/controls.h>
#include <arh/arhui.h>
#include <arh/arhres.h>

int main() {
    InitWindow();
    InitControls(window);
    InitCamera(1280, 720);
    
    InitResources();
    InitVoxels();

    InitUI();

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderVoxels();
        RenderUI();
        
        ControlsUpdate();

        glfwSwapBuffers(window);        
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
