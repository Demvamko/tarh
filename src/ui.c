#include <glew.h>
#include <arhgl.h>
#include <arhcam.h>
#include <ui.h>

typedef struct Vert{
    short x;
    short y;

    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

    unsigned char u: 1;
    unsigned char v: 1;
    unsigned char z: 5;
} Vert;

typedef Vert Verts[6];

static Attributes VERT_ATTRIBS[4] = { 
    { 2, GL_SHORT, 0, sizeof(Vert), 0, 1 }, 
    { 4, GL_UBYTE, 1, sizeof(Vert), 4, 0 },
    { 1, GL_UBYTE, 0, sizeof(Vert), 8, 1 }, 
    { 0 }
};

static Tag tags[2] = {
    {
        .anchors = { 0.0f, 0.0f, 1.0f, 1.0f },
        .position = POSITION_ANCHORED
    },
        {
            .anchors = { 0.0f, 0.0f, 1.0f, 0.05f },
            .position = POSITION_ANCHORED,
            .color = { 25, 25, 25, 255 },
            .depth = 1,
            .index = 1,
            .visible = 1
        }
};

static Verts verts[lenof(tags)];
static Buffer buffer;
static uint shader;

void InitUI(){
    shader = CreateShader("./res/glsl/ui.glsl");

    tags[0].client[2] = camera_resolution[0];
    tags[0].client[3] = camera_resolution[1];

    UI_RefreshParents();
    UI_RefreshChildClients(tags);

    buffer = CreateBuffer(sizeof(Vert), lenof(tags) * 6, GL_TRIANGLES, verts, VERT_ATTRIBS);
}

void RenderUI(){
    glUseProgram(shader);
    RenderBuffer(&buffer);
}

void UI_RefreshParents(){
    Tag* last = 0;

    for(int i = 1; i < lenof(tags); i++){
        Tag* tag = tags + i;

        while(last && last->depth >= tag->depth){
            if(last->depth == tag->depth)
                last->sibling = tag;

            last = last->parent;
        }

        tag->parent = last;
        last = tag;
    }
}

void UI_RefreshChildClients(Tag* parent){
    if(parent[1].depth <= parent->depth)
        return;

    int dim = parent->type == TYPE_COL;
    int space = parent->client[2 + dim];

    for(Tag* child = parent + 1; child; child = child->sibling)
        if(child->position == POSITION_DEFAULT)
            space -= child->rect[dim];

    int offset[2];
    offset[0] = parent->client[0];
    offset[1] = parent->client[1];

    if(parent->alignment)
        offset[dim] += space / parent->alignment;

    for(Tag* child = parent + 1; child; child = child->sibling){
        if(child->position == POSITION_DEFAULT){
            child->client[0] = offset[0];
            child->client[1] = offset[1];
            child->client[2] = child->rect[2];
            child->client[3] = child->rect[3];

            offset[dim] += child->client[2 + dim];
        }

        else if(child->position == POSITION_ABSOLUTE){
            child->client[0] = parent->client[0] + child->rect[0];
            child->client[1] = parent->client[1] + child->rect[1];
            child->client[2] = child->rect[2];
            child->client[3] = child->rect[3];
        }

        else if(child->position == POSITION_ANCHORED){
            #define LERP(a, b, amount) (a + amount * b)

            child->client[0] = LERP(parent->client[0], parent->client[2], child->anchors[0]);
            child->client[1] = LERP(parent->client[1], parent->client[3], child->anchors[1]);
            child->client[2] = LERP(parent->client[0], parent->client[2], child->anchors[2]);
            child->client[3] = LERP(parent->client[1], parent->client[3], child->anchors[3]);

            #undef LERP

            child->client[2] -= child->client[0];
            child->client[3] -= child->client[1];
        }
    
        UI_RefreshVerts(child);
        UI_RefreshChildClients(child);
    }
}

void UI_RefreshVerts(Tag* tag){
    if(tag->visible){
        verts[tag->index][0].x = tag->client[0];
        verts[tag->index][0].y = tag->client[1];
        verts[tag->index][0].u = 0;
        verts[tag->index][0].v = 0;

        verts[tag->index][1].x = tag->client[0];
        verts[tag->index][1].y = tag->client[1] + tag->client[3];
        verts[tag->index][1].u = 0;
        verts[tag->index][1].v = 1;

        verts[tag->index][2].x = tag->client[0] + tag->client[2];
        verts[tag->index][2].y = tag->client[1] + tag->client[3];
        verts[tag->index][2].u = 1;
        verts[tag->index][2].v = 1;

        verts[tag->index][3].x = tag->client[0];
        verts[tag->index][3].y = tag->client[1];
        verts[tag->index][3].u = 0;
        verts[tag->index][3].v = 0;

        verts[tag->index][4].x = tag->client[0] + tag->client[2];
        verts[tag->index][4].y = tag->client[1] + tag->client[3];
        verts[tag->index][4].u = 1;
        verts[tag->index][4].v = 1;

        verts[tag->index][5].x = tag->client[0] + tag->client[2];
        verts[tag->index][5].y = tag->client[1];
        verts[tag->index][5].u = 1;
        verts[tag->index][5].v = 0;

        for(int i = 0; i < 6; i++){
            verts[tag->index][i].r = tag->color[0];
            verts[tag->index][i].g = tag->color[1];
            verts[tag->index][i].b = tag->color[2];
            verts[tag->index][i].a = tag->color[3];
            verts[tag->index][i].z = tag->index;
        }
    }
    else {
        for(int i = 0; i < 6; i++)
            verts[tag->index][i] = (Vert) { 0 };
    }

}