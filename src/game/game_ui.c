#define UI_USAGE_FUNC

#include <arh/gl.h>
#include <arh/ui.h>
#include <game/game_ui.h>

static Tag tags[5] = { 
    {
        { 0, 0, 1, 0.1f },
        { 0, 0, 0, 200 },
        0,
        0,
        { 0, 0, 1, 1 },
    },
    {
        { 0.05f, 0.1f, 0.1f, 0.9f },
        { 255, 255, 255, 255 },
        0,
        1,
    },
    {
        { 0.15f, 0.1f, 0.2f, 0.9f },
        { 255, 255, 255, 255 },
        2,
        1,
    },
    {0}
};

void Arh_UI_Game_Init(){
    UI_Init(tags, 3);
}

void Arh_UI_Game_Render(){
    UI_Render();
}