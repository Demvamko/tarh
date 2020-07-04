#include <ft2build.h>
#include FT_FREETYPE_H
#include <glew.h>
#include <arhgl.h>
#include <arhcam.h>
#include <ui.h>

int UI_RECT_TABLE[6][2] = {
    { 0, 1 },
    { 0, 3 },
    { 2, 3 },
    { 2, 3 },
    { 2, 1 },
    { 0, 1 }
};

int UI_UV_TABLE[6][2] = {
    { 0, 0 },
    { 0, 1 },
    { 1, 1 },
    { 1, 1 },
    { 1, 0 },
    { 0, 0 },
};

typedef struct Vert{
    short x;
    short y;

    unsigned char color[4];
    unsigned char z;

    float u;
    float v;
} Vert;

typedef Vert Verts[6];

static Attributes VERT_ATTRIBS[] = { 
    { 2, GL_SHORT, 0, sizeof(Vert), 0, 1 }, 
    { 4, GL_UBYTE, 1, sizeof(Vert), 4, 0 },
    { 1, GL_UBYTE, 0, sizeof(Vert), 8, 1 },
    { 2, GL_FLOAT, 0, sizeof(Vert), 12, 0 },
    { 0 }
};

#define TEX_W 512
#define TEX_H 512
#define TEX_FONT_SIZE 12.0f

typedef struct TextVert{
    short x;
    short y;

    unsigned char c;
    unsigned char u :1;
    unsigned char v :1;
    unsigned char z :6;
} TextVert;

typedef TextVert TextVerts[6];

static Attributes TEXT_VERT_ATTRIBS[] = { 
    { 2, GL_SHORT, 0, sizeof(TextVert), 0, 1 }, 
    { 1, GL_UBYTE, 0, sizeof(TextVert), 4, 1 }, 
    { 1, GL_UBYTE, 0, sizeof(TextVert), 5, 1 },
    { 0 }
};

typedef struct Font{
    Texture texture;
    Uniform uniform;

    int line_height;
    float uvs[127][4];
    int rects[127][4];
    int advances[127];
} Font;

typedef struct Text{
    Buffer buffer;
    TextVerts* verts;
} Text;

static char example[] = "Example string. \n Newline example as well. \n Some punctuation !?.{}<>";

#include <ui_layout.h>

static Verts verts[lenof(tags)];
static Buffer buffer;
static uint shader;

static Texture texture;
static Font font;
static uint font_shader;

static Text** texts;
static int texts_len = 0;
static int texts_max = 16;

void InitUI(){
    texts = calloc(texts_max, sizeof(Text*));

    texture = CreateImgTexture("./res/img/granite.png", 0);

    shader = CreateShader("./res/glsl/ui.glsl");
    font_shader = CreateShader("./res/glsl/text.glsl");

    UI_Text_CreateFont("./res/font/roboto-mono.ttf");

    tags[0].client[2] = camera_resolution[0];
    tags[0].client[3] = camera_resolution[1];

    UI_RefreshParents();
    UI_RefreshChildClients(tags);

    buffer = CreateBuffer(sizeof(Vert), lenof(tags) * 6, GL_TRIANGLES, verts, VERT_ATTRIBS);
}

void RenderUI(){
    glUseProgram(shader);
    BindTexture(&texture);
    RenderBuffer(&buffer);

    glUseProgram(font_shader);
    BindUniform(&font.uniform);
    BindTexture(&font.texture);

    for(int i = 0; i < lenof(tags); i++)
        if(tags[i].text)
            RenderBuffer(&tags[i].text->buffer);
}

void UI_RefreshParents(){
    Tag* last = tags;

    for(int i = 1; i < lenof(tags); i++){
        Tag* tag = tags + i;

        while(last && last->depth >= tag->depth){
            if(last->depth == tag->depth)
                last->sibling = tag;

            last = last->parent;
        }

        tag->parent = last;
        
        if(last && !last->child)
            last->child = tag;

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

    for(Tag* child = parent->child; child; child = child->sibling){
        if(child->position == POSITION_DEFAULT){
            child->client[0] = offset[0];
            child->client[1] = offset[1];
            child->client[2] = offset[0] + child->rect[2];
            child->client[3] = offset[1] + child->rect[3];

            offset[dim] += child->rect[2 + dim];
        }

        else if(child->position == POSITION_ABSOLUTE){
            child->client[0] = parent->client[0] + child->rect[0];
            child->client[1] = parent->client[1] + child->rect[1];
            child->client[2] = child->client[0] + child->rect[2];
            child->client[3] = child->client[1] + child->rect[3];
        }

        else if(child->position == POSITION_ANCHORED){
            #define LERP(a, b, amount) (a + amount * (b - a))

            child->client[0] = LERP(parent->client[0], parent->client[2], child->anchors[0]);
            child->client[1] = LERP(parent->client[1], parent->client[3], child->anchors[1]);
            child->client[2] = LERP(parent->client[0], parent->client[2], child->anchors[2]);
            child->client[3] = LERP(parent->client[1], parent->client[3], child->anchors[3]);

            #undef LERP
        }
    
        UI_RefreshVerts(child);
        UI_RefreshChildClients(child);

        if(child->text_str){
            if(!child->text)
                UI_Text_CreateText(child);

            UI_Text_RefreshVerts(child);
        }
    }
}

void UI_RefreshVerts(Tag* tag){
    if(!tag->visible){
        for(int i = 0; i < 6; i++)
            verts[tag->index][i] = (Vert) { 0 };
    }
    else {
        for(int i = 0; i < 6; i++){
            verts[tag->index][i].x = tag->client[UI_RECT_TABLE[i][0]];
            verts[tag->index][i].y = tag->client[UI_RECT_TABLE[i][1]];

            for(int c = 0; c < 4; c++)
                verts[tag->index][i].color[c] = tag->color[c];

            verts[tag->index][i].z = tag->index;

            verts[tag->index][i].u = tag->uv[1 + 2 * UI_UV_TABLE[i][0]];
            verts[tag->index][i].v = tag->uv[1 + 2 * UI_UV_TABLE[i][1]];
        }
    }
}

void UI_Text_CreateFont(const char* path){
    FT_Library lib;
    FT_Face face;

    FT_Init_FreeType(&lib);
    FT_New_Face(lib, path, 0, &face);
    FT_Set_Pixel_Sizes(face, 0, TEX_FONT_SIZE);

    char* tex_buffer = calloc(TEX_W * TEX_H, 1);

    int x_offset = 5;
    int y_offset = 5;
    int y_max = 0;
    int line_height = 0;

    for(char i = 0; i < 127; i++){
        FT_Load_Char(face, i, FT_LOAD_RENDER);

        int w = face->glyph->bitmap.width;
        int h = face->glyph->bitmap.rows;

        if(x_offset + face->glyph->bitmap.width > TEX_W){
            x_offset = 0;
            y_offset += y_max + 5;
            y_max = 0;
        }

        for(int y = 0; y < h; y++)
        for(int x = 0; x < w; x++){
            int coord = (x + x_offset) + (y + y_offset) * TEX_W;
            tex_buffer[coord] = face->glyph->bitmap.buffer[x + y * w];
        }

        font.uvs[i][0] = x_offset / (float)TEX_W; 
        font.uvs[i][3] = y_offset / (float)TEX_H;
        font.uvs[i][2] = (x_offset + w) / (float)TEX_W;
        font.uvs[i][1] = (y_offset + h) / (float)TEX_H;

        int xpos = face->glyph->bitmap_left;
        int ypos = 0 - (face->glyph->bitmap.rows - face->glyph->bitmap_top);

        font.rects[i][0] = xpos;
        font.rects[i][1] = ypos;
        font.rects[i][2] = xpos + w;
        font.rects[i][3] = ypos + h;

        font.advances[i] = face->glyph->advance.x >> 6;

        x_offset += w + 5;

        if(h > y_max)
            y_max = h;

        if(h > line_height)
            line_height = h;
    }

    font.line_height = line_height;

    FT_Done_Face(face);
    FT_Done_FreeType(lib);

    glGenTextures(1, &font.texture.id);
    glBindTexture(GL_TEXTURE_2D, font.texture.id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, TEX_W, TEX_H, 0, GL_RED, GL_UBYTE, tex_buffer);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    font.uniform = CreateUniform(sizeof(font.uvs), font.uvs, 3);
}


void UI_Text_CreateText(Tag* tag){
    if(texts_len + 1 > texts_max){
        texts_max *= 2;
        texts = realloc(texts, sizeof(Text*));
    }

    texts[texts_len] = calloc(1, sizeof(Text));
    tag->text = texts[texts_len];

    tag->text->verts = calloc(tag->text_len, sizeof(TextVerts));

    texts_len++;   
}

void UI_Text_RefreshVerts(Tag* tag){
    int offset_x = tag->client[0];
    int offset_y = tag->client[3] - font.line_height;

    for(int i = 0; tag->text_str[i]; i++){
        char c = tag->text_str[i];

        if(c == '\n'){
            offset_x = 0;
            offset_y -= font.line_height;
            continue;
        }

        int* font_rect = font.rects[c];

        for(int vert = 0; vert < 6; vert++){
            tag->text->verts[i][vert].x = offset_x + font_rect[UI_RECT_TABLE[vert][0]] * (tag->font_size / TEX_FONT_SIZE);
            tag->text->verts[i][vert].y = offset_y + font_rect[UI_RECT_TABLE[vert][1]] * (tag->font_size / TEX_FONT_SIZE);

            tag->text->verts[i][vert].c = c;

            tag->text->verts[i][vert].u = UI_UV_TABLE[vert][0];
            tag->text->verts[i][vert].v = UI_UV_TABLE[vert][1];
            tag->text->verts[i][vert].z = tag->depth + 1;
        }

        offset_x += font.advances[c] * (tag->font_size / TEX_FONT_SIZE);
    }

    tag->text->buffer = CreateBuffer(sizeof(TextVert), tag->text_len * 6, GL_TRIANGLES, tag->text->verts, TEXT_VERT_ATTRIBS);
}