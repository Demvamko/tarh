#define UI_USAGE_FUNC

#include <arh/gl.h>
#include <arh/ui.h>
#include <game/game_ui.h>
#include <arh/cam.h>

static float frames[4 * 4] = { 
       0 ,   0 ,   1 , 0.1f,
    0.05f, 0.1f, 0.1f, 0.9f,
    0.15f, 0.1f, 0.2f, 0.9f
};

static View views[4] = {
    { {   0,   0,   0, 200 }, 0 },
    { { 255, 255, 255, 255 }, 0 },
    { { 255, 255, 255, 255 }, 0 }
};

static char tree[4] = { 0, 0, 0 };



void Arh_UI_Game_Init(){
    UI_Init(frames, views, 4, tree);
}

void Arh_UI_Game_Render(){
    UI_Render();
}

void Arh_UI_Game_MouseDown(float x, float y){
    Node* node = UI_Tag_GetHoveredTree(x / camera.res[0], y / camera.res[1]);

    
}

void Arh_UI_Game_MouseMove(float x, float y){
    Node* node = UI_Tag_GetHoveredTree(x / camera.res[0], y / camera.res[1]);


}