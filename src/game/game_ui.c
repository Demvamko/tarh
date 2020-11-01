#define UI_USAGE_FUNC

#include <arh/gl.h>
#include <arh/ui.h>
#include <game/game_ui.h>
#include <arh/cam.h>
#include <voxel/types.h>
#include <ext/pack_nar.h>
#include <lib/glew.h>

static float frames[8 * 4] = { 0, 0, 1, 0.1f, 0 };
static View views[8] = {{{ 0, 0, 0, 200 }, 0 }, { 0 }};
static char tree[8] = { 0, 0, 0 };

Node* hot = 0;
Node* active;

void TestUIEnter(Node* node){
    node->view->color[0] = 200;
    node->view->color[1] = 200;
    node->view->color[2] = 200;
    UI_Tag_UpdateBuffer(node->id, 1);
}

void TestUILeave(Node* node){
    node->view->color[0] = 255;
    node->view->color[1] = 255;
    node->view->color[2] = 255;
    UI_Tag_UpdateBuffer(node->id, 1);
}

extern int sel_block;

void TestUIClick(Node* node){
    node->view->color[0] = 255;
    node->view->color[1] = 255;
    node->view->color[2] = 255;
    UI_Tag_UpdateBuffer(node->id, 1);

    sel_block = node->custom_data[0];
}

static int crosshair = 0;

void Arh_UI_Game_Init(){
    UI_Init(frames, views, 5, tree);

    glGetError();
    for(int i = 0; i < 4; i++){
        Node* node = UI_Tag_Get(i + 1);
        node->frame[0] = 0.02f + i * 0.07f;
        node->frame[1] = 0.1f;
        node->frame[2] = 0.07f + i * 0.07f;
        node->frame[3] = 0.9f;

        node->view->color[0] = 255;
        node->view->color[1] = 255;
        node->view->color[2] = 255;
        node->view->color[3] = 255;

        node->view->image = voxels[i].image;
        node->custom_data[0] = i;

        node->onclick = TestUIClick;
        node->onenter = TestUIEnter;
        node->onleave = TestUILeave;
    }

    UI_Tag_UpdateClients(UI_Tag_Get(0));
    glGetError();
    UI_Tag_UpdateBuffer(0, 5);
    glGetError();

    glGetError();
}

void Arh_UI_Game_Render(){
    UI_Render();
}

void Arh_UI_Game_MouseClick(float x, float y){
    Node* node = UI_Tag_GetHoveredTree(x / camera.res[0], 1 - y / camera.res[1]);

    if(node && node->onclick)
        node->onclick(node);
}

void Arh_UI_Game_MouseMove(float x, float y){
    Node* node = UI_Tag_GetHoveredTree(x / camera.res[0], 1 - y / camera.res[1]);

    if(node == hot)
        return;

    if(node && node->onenter)
        node->onenter(node);

    if(hot && hot->onleave)
        hot->onleave(hot);

    hot = node;
}