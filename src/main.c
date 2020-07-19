#include <lib/glew.h>
#include <glfw3.h>
#include <arh/cam.h>
#include <arh/win.h>
#include <voxel/voxels.h>
#include <game/controls.h>
#include <arh/ui.h>
#include <arh/std.h>

int main() {
    InitWindow();
    InitControls(window);
    InitCamera(1280, 720);
    
    Arh_InitResources();
    InitVoxels();

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderVoxels();

        ControlsUpdate();

        glfwSwapBuffers(window);        
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
