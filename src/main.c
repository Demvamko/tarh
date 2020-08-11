#include <lib/glew.h>
#include <glfw3.h>
#include <arh/cam.h>
#include <arh/win.h>
#include <voxel/voxels.h>
#include <game/controls.h>
#include <game/game_ui.h>
#include <arh/std.h>
void UI_Init();

int main() {
    Arh_Window_Init();
    Arh_Controls_Init(window);
    Arh_Camera_Init(1280, 720);
    
    Arh_InitResources();

    UI_Init();
    Arh_UI_Game_Init();
    InitVoxels();

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderVoxels();
        Arh_UI_Game_Render();
        Arh_Controls_Update();

        glfwSwapBuffers(window);        
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
