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

static uint count;

static float* frames;
static float* clients;

static View* views;
static Node* nodes;
static Vert* verts;
static Buffer buffer;

static Node null_node = { -1, 0 };

void UI_Tag_CreateTree(char* parents, int count);

void UI_Init(float* mframes, View* mviews, int mcount, char* mparents){
    Arh_Frame_Create(camera.res[0], camera.res[1], UI_FRAME_BIND);
    Arh_Frame_Unbind();

    shader = Arh_Shader_Create(NAR_UI_VERT, NAR_UI_FRAG);
    vpassf = Arh_Shader_Create(NAR_VPASS_VERT, NAR_VPASS_FRAG);
    format = Arh_Attrib_Create(VERT_ATTRIBS);
    
    frames = mframes;
    views = mviews;  

    count = mcount;

    verts = arralloc(mcount * 6, sizeof(Vert));
    arrlen(verts) = mcount * 6 * sizeof(Vert);
    nodes = calloc(mcount, sizeof(Node));
    clients = calloc(mcount + 1, sizeof(float) * 4);

    clients[0] = 0;
    clients[1] = 0;
    clients[2] = 1;
    clients[3] = 1;

    clients += 4;

    UI_Tag_CreateTree(mparents, mcount);
    nodes->parent = &null_node;
    UI_Tag_CalcNodesSize(nodes);
    UI_Tag_UpdateClients(nodes);
    UI_Tag_UpdateVerts(0, mcount);
    Arh_Buffer_Create(&buffer, GL_TRIANGLES, verts, format);
}

void UI_Tag_CreateTree(char* parents, int count){
    for(int i = 1; i < count; i++){
        Node* child = nodes + i;
        Node* parent = nodes + parents[i];

        child->id = i;

        child->parent = parent;
        child->sibling = parent->child;
        parent->child = child;
    }
}

void UI_Tag_UpdateClients(Node* node){
    float* frame = frames + (node->id * 4);
    float* client = clients + (node->id * 4);
    float* parent = clients + (node->parent->id * 4);

    float w = parent[2] - parent[0];
    float h = parent[3] - parent[1];
 
    client[0] = w * frame[0] + parent[0];
    client[1] = h * frame[1] + parent[1];
    client[2] = w * frame[2] + parent[0];
    client[3] = h * frame[3] + parent[1];

    if(node->sibling)
        UI_Tag_UpdateClients(node->sibling);

    if(node->child)
        UI_Tag_UpdateClients(node->child);
}

void UI_Tag_UpdateBuffer(int id, int count){
    UI_Tag_UpdateVerts(id, count);

    Vert* mverts = verts + id * 6;
    int offset = id * sizeof(Vert) * 6;
    int size = count * sizeof(Vert) * 6;

    Arh_Buffer_Update(&buffer, offset, size);
}

void UI_Tag_UpdateVerts(int id, int count){
    #define FOR(x) for(int p = 0; p < x; p++)

    Vert* mverts = verts + id * 6;

    for(int i = 0; i < count; i++){
        float* client = clients + i * 4;
        View* view = views + i;

        for(int v = 0; v < 6; v++){
            Vert* vert = mverts + i * 6 + v;

            FOR(2) vert->pos[p] = client[p + ui_table[v][p] * 2] * 255;
            FOR(4) vert->color[p] = view->color[p];
            vert->image = (view->image << 2) + (ui_table[v][1] << 1) + ui_table[v][0];
        }
    }

    #undef FOR
}

uint UI_Tag_CalcNodesSize(Node* node){
    int siblings = 0;

    if(node->child)
        node->size += UI_Tag_CalcNodesSize(node->child) + 1;

    if(node->sibling)
        siblings = UI_Tag_CalcNodesSize(node->sibling);

    return node->size + siblings;
}

Node* UI_Tag_GetHoveredTree(float x, float y){
    return UI_Tag_GetHovered(nodes, x, y);
}

Node* UI_Tag_GetHovered(Node* node, float x, float y){
    float* client = clients + node->id * 4;

    if(client[0] < x && client[2] > x && client[1] < y && client[3] > y ){
        if(node->child){
            Node* t = UI_Tag_GetHovered(node->child, x, y);
            return t ? t : node;
        }

        return node;
    }

    if(node->sibling)
        return UI_Tag_GetHovered(node->sibling, x, y);

    return 0;
}

void UI_Render(){
    Arh_Shader_Bind(shader);
    Arh_Frame_Bind(UI_FRAME_BIND);
    glClear(GL_COLOR_BUFFER_BIT);
    Arh_Buffer_Render(&buffer);
    Arh_Frame_Unbind();
    Arh_Shader_Bind(vpassf);
    Arh_RectPass();
}