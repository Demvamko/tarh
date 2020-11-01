// #include <ft2build.h>
// #include FT_FREETYPE_H
// #include <glew.h>
// #include <arhgl.h>
// #include <arhcam.h>
// #include <ui.h>


// #define TEX_W 512
// #define TEX_H 512
// #define TEX_FONT_SIZE 12.0f

// typedef struct TextVert{
//     short x;
//     short y;

//     unsigned char c;
//     unsigned char u :1;
//     unsigned char v :1;
//     unsigned char z :6;
// } TextVert;

// typedef TextVert TextVerts[6];

// static Attributes TEXT_VERT_ATTRIBS[] = { 
//     { 2, GL_SHORT, 0, sizeof(TextVert), 0, 1 }, 
//     { 1, GL_UBYTE, 0, sizeof(TextVert), 4, 1 }, 
//     { 1, GL_UBYTE, 0, sizeof(TextVert), 5, 1 },
//     { 0 }
// };

// typedef struct Font{
//     Texture texture;
//     Uniform uniform;

//     int line_height;
//     float uvs[127][4];
//     int rects[127][4];
//     int advances[127];
// } Font;

// typedef struct Text{
//     Buffer buffer;
//     TextVerts* verts;
// } Text;

// static Texture texture;
// static Font font;
// static uint font_shader;

// static Text** texts;
// static int texts_len = 0;
// static int texts_max = 16;

// void UI_Text_Init(){
// }

// void UI_Text_CreateText(Tag* tag){
//     if(texts_len + 1 > texts_max){
//         texts_max *= 2;
//         texts = realloc(texts, sizeof(Text*));
//     }

//     texts[texts_len] = calloc(1, sizeof(Text));
//     tag->text = texts[texts_len];

//     tag->text->verts = calloc(tag->text_len, sizeof(TextVerts));

//     texts_len++;   
// }

// void UI_Text_RefreshVerts(Tag* tag){
//     int offset_x = tag->client[0];
//     int offset_y = tag->client[3] - font.line_height;

//     for(int i = 0; tag->text_str[i]; i++){
//         char c = tag->text_str[i];

//         if(c == '\n'){
//             offset_x = 0;
//             offset_y -= font.line_height;
//             continue;
//         }

//         int* font_rect = font.rects[c];

//         for(int vert = 0; vert < 6; vert++){
//             tag->text->verts[i][vert].x = offset_x + font_rect[UI_RECT_TABLE[vert][0]] * (tag->font_size / TEX_FONT_SIZE);
//             tag->text->verts[i][vert].y = offset_y + font_rect[UI_RECT_TABLE[vert][1]] * (tag->font_size / TEX_FONT_SIZE);

//             tag->text->verts[i][vert].c = c;

//             tag->text->verts[i][vert].u = UI_UV_TABLE[vert][0];
//             tag->text->verts[i][vert].v = UI_UV_TABLE[vert][1];
//             tag->text->verts[i][vert].z = tag->depth + 1;
//         }

//         offset_x += font.advances[c] * (tag->font_size / TEX_FONT_SIZE);
//     }

//     tag->text->buffer = CreateBuffer(sizeof(TextVert), tag->text_len * 6, GL_TRIANGLES, tag->text->verts, TEXT_VERT_ATTRIBS);
// }