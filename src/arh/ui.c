#include <stdlib.h>
#include <lib/glew.h>
#include <arh/gl.h>
#include <arh/cam.h>
#include <arh/ui.h>
#include <arh/std.h>
#include <ext/pack_nar.h>

static int ui_table[6][2] = {
    { 0, 0 },
    { 1, 0 },
    { 1, 1 },
    { 1, 1 },
    { 0, 1 },
    { 0, 0 }
};

typedef struct Node {
    Tag* tag;
    Node* child;
    Node* sibling;
    Node* parent;
} Node;

typedef struct Vert {
    uchar pos[2];
    uchar color[4];
    uchar image;
} Vert;

static Attribute VERT_ATTRIBS[] = {
    { 2, GL_UBYTE, 1, sizeof(Vert), 0, 0 },
    { 4, GL_UBYTE, 1, sizeof(Vert), 2, 0 },
    { 1, GL_UBYTE, 0, sizeof(Vert), 6, 1 },
    { 0 }
};

static uint shader;
static uint vpassf;
static uint format;

static Node* tree;
static Vert* verts;
static Buffer buffer;

void UI_Rect_Anchor(float* a, float* b, float* result){
    float w = a[2] - a[0];
    float h = a[3] - a[1];
 
    result[0] = w * b[0] + a[0];
    result[1] = h * b[1] + a[2];
    result[2] = w * b[2] + a[0];
    result[3] = h * b[3] + a[2];
}

void UI_Tag_UpdateSubtree(Vert* verts, Tag* tags);

void UI_Init(Tag* tags, int count){
    Arh_Frame_Create(camera.res[0], camera.res[1], UI_FRAME_BIND);
    Arh_Frame_Unbind();

    Arh_Uniform_Create(0, sizeof(Tag), UI_UBO_BIND);
    shader = Arh_Shader_Create(NAR_UI_VERT, NAR_UI_FRAG);
    vpassf = Arh_Shader_Create(NAR_VPASS_VERT, NAR_VPASS_FRAG);
    format = Arh_Attrib_Create(VERT_ATTRIBS);

    verts = arralloc(count * 6, sizeof(Vert));
    
    UI_Tag_UpdateSubtree(verts, tags);
    Arh_Buffer_Create(&buffer, GL_TRIANGLES, verts, format);
}

void UI_Tag_UpdateSubtree(Vert* verts, Tag* tags){
    #define FOR(x) for(int p = 0; p < x; p++)
    
    Tag* parents[8] = { tags, 0 };
    char plen = 0;

    Tag* tag = tags;
    int id = 0;

    do{
        FOR(4) tag->client[p] = parents[plen]->client[p] * tag->rect[p];

        for(int v = 0; v < 6; v++){
            Vert* vert = verts + v + id * 6;

            FOR(2) vert->pos[p] = tag->client[p + ui_table[v][p] * 2] * 255;
            FOR(4) vert->color[p] = tag->color[p];
            vert->image = (tag->image << 2) + (ui_table[v][1] << 1) + ui_table[v][0];
    
            arrlen(verts) += sizeof(Vert);
        }

        if(tag[1].depth > tag->depth){
            parents[plen + 1] = tag;
            plen += 1;
        }
        else if(tag[1].depth < tag->depth)
            plen --;

        id++;
        tag++;

    } while(tag->depth > tags->depth);

    #undef FOR
}

void UI_Tag_BuildTree(Tag* tags, int count){
    tree = calloc(count, sizeof(Node));

    Tag* parents[8] = { tags, 0 };
    char plen;

    Node* node = tree;
    Tag* tag = tags;

    do{
        node->tag = tag;



    } while(tag->depth > tags->depth);
}

// Tag* UI_Tag_GetHovered(Tag* tag, float x, float y){
//     if( tag->client[0] < x &&  tag->client[2] > x && tag->client[1] < y && tag->client[3] > y ){
//         if(tag->child){
//             Tag* t = UI_Tag_GetHovered(tag->child, x, y);
//             return t ? t : tag;
//         }

//         return tag;
//     }

//     if(tag->sibling)
//         return UI_Tag_GetHovered(tag->sibling, x, y);

//     return 0;
// }

void UI_Render(){
    Arh_Shader_Bind(shader);
    Arh_Frame_Bind(UI_FRAME_BIND);
    glClear(GL_COLOR_BUFFER_BIT);
    Arh_Buffer_Render(&buffer);
    Arh_Frame_Unbind();
    Arh_Shader_Bind(vpassf);
    Arh_RectPass();
}