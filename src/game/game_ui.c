#define UI_USAGE_FUNC

#include <arh/gl.h>
#include <arh/ui.h>
#include <game/game_ui.h>

static Tag tags[5] = { 
    {
        { 0, 0 },
        { 1, 0.1f },
        { 0, 0, 0, 0.8f }
    },
    {
        { 0.05f, 0.1f },
        { 0.05f, 0.8f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        0
    },
    {
        { 0.15f, 0.1f },
        { 0.05f, 0.8f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        2
    }
};

static Buffer buffer;

void Arh_UI_Game_Init(){
    UI_Tag_AddChild(tags, tags + 1);
    UI_Tag_AddChild(tags, tags + 2);

    UI_Tag_CreateBuffer(&buffer, tags, 3);
}

void Arh_UI_Game_Render(){
    UI_Render(&buffer);
}