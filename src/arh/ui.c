#include <lib/glew.h>
#include <arh/gl.h>
#include <arh/cam.h>
#include <arh/ui.h>
#include <arh/std.h>
#include <ext/pack_nar.h>

static uint shader;
static uint vpassf;
static uint format;

typedef struct {
    char pos[2];
    char color[4];
    char image;
} Vert;

static Attribute VERT_ATTRIBS[] = {
    { 2, GL_UBYTE, 1, sizeof(Vert), 0, 0 },
    { 4, GL_UBYTE, 1, sizeof(Vert), 2, 0 },
    { 1, GL_UBYTE, 0, sizeof(Vert), 6, 1 },
    { 0 }
};

void UI_Init(){
    Arh_Frame_Create(camera.res[0], camera.res[1], UI_FRAME_BIND);
    Arh_Frame_Unbind();

    Arh_Uniform_Create(0, sizeof(Tag), UI_UBO_BIND);
    shader = Arh_Shader_Create(NAR_UI_VERT, NAR_UI_FRAG);
    vpassf = Arh_Shader_Create(NAR_VPASS_VERT, NAR_VPASS_FRAG);
    format = Arh_Attrib_Create(VERT_ATTRIBS);
}

void UI_Render(Buffer* buffer){
    Arh_Shader_Bind(shader);
    Arh_Frame_Bind(UI_FRAME_BIND);
    glClear(GL_COLOR_BUFFER_BIT);
    Arh_Buffer_Render(buffer);
    Arh_Frame_Unbind();
    Arh_Shader_Bind(vpassf);
    Arh_RectPass();
}

void UI_Tag_AddChild(Tag* parent, Tag* child){
    child->parent = parent;
    child->sibling = parent->child;
    parent->child = child;
}

static int ui_table[6][2] = {
    { 0, 0 },
    { 1, 0 },
    { 1, 1 },
    { 1, 1 },
    { 0, 1 },
    { 0, 0 }
};

void UI_Tag_Populate(Vert* verts, Tag* tag, float* frame){
    tag->client[0] = frame[0] + frame[2] * tag->pos[0];
    tag->client[1] = frame[1] + frame[3] * tag->pos[1];
    tag->client[2] = frame[2] * tag->size[0];
    tag->client[3] = frame[3] * tag->size[1];

    tag->vertices = (Vert*)arrnext(verts);

    for(int i = 0; i < 6; i++){
        Vert* vert = (Vert*)arrnext(verts);
    
        #define FOR(x) for(int p = 0; p < x; p++)

        FOR(2) vert->pos[p] = (tag->client[p] + ui_table[i][p] * tag->client[p + 2]) * 255;
        FOR(4) vert->color[p] = tag->color[p] * 255;
        vert->image = (tag->image << 2) + (ui_table[i][1] << 1) + ui_table[i][0];
    
        #undef FOR
    
        arrlen(verts) += sizeof(Vert);
    }
    
    if(tag->child)
        UI_Tag_Populate(verts, tag->child, tag->client);
    if(tag->sibling)
        UI_Tag_Populate(verts, tag->sibling, frame);
}

void UI_Tag_CreateBuffer(Buffer* buffer, Tag* root, uint count){
    Vert* verts = arralloc(count * 6, sizeof(Vert));
    UI_Tag_Populate(verts, root, (float[4]){ 0, 0, 1, 1 });
    Arh_Buffer_Create(buffer, GL_TRIANGLES, verts, format);
}

Tag* UI_Tag_GetHovered(Tag* tag, float x, float y){
    if(
        tag->client[0] < x && 
        tag->client[2] + tag->client[0] > x &&
        tag->client[1] < y &&
        tag->client[3] + tag->client[1] > y)
    {
        if(tag->child){
            Tag* t = UI_Tag_GetHovered(tag, x, y);
            return t ? t : tag;
        }

        return tag;
    }

    if(tag->sibling)
        return UI_Tag_GetHovered(tag, x, y);
}