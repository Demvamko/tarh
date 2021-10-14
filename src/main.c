#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <arh/cam.h>
#include <arh/win.h>
#include <arh/gl.h>
#include <ext/pack_nar.h>
#include <voxel/voxels.h>
#include <game/controls.h>
#include <game/game_ui.h>
#include <arh/std.h>

void PCCube_Init();
void PCCube_Render();

int main() {
    Arh_Window_Init();
    Arh_Controls_Init(window);
    Arh_Camera_Init(1280, 720);
    
    Arh_InitResources();

    Arh_UI_Game_Init();
    InitVoxels();
    
    int crosshair = Arh_Shader_Create(NAR_CROSSHAIR_VERT, NAR_CROSSHAIR_FRAG);

    PCCube_Init();

    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        RenderVoxels();
        PCCube_Render();

        Arh_Shader_Bind(crosshair);
        glDrawArrays(GL_POINTS, 0, 1);

        Arh_UI_Game_Render();

        Arh_Controls_Update();

        glfwSwapBuffers(window);        
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
