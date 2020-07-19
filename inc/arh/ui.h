
#define ALIGN_START 0
#define ALIGN_END 1
#define ALIGN_CENTER 2

#define LAYOUT_HORIZONTAL 0
#define LAYOUT_VERTICAL 1

typedef struct Text{
    char* string;
    short len;
    char size;
    char font;
} Text;

typedef struct Tag{
    short client[4];
    float size[2];

    short image;

    uchar color[4];
    uchar depth;

    uchar align[2];
    uchar layout : 1;

    struct Tag* parent;
    struct Tag* child;
    struct Tag* sibling;
    struct Vert* vertices;
} Tag;

extern int UI_RECT_TABLE[6][2];
extern int UI_UV_TABLE[6][2];

void InitUI();
void RenderUI();

void UI_RefreshChildClients(Tag* parent);
void UI_RefreshVerts(Tag* tag);

// void UI_Text_CreateFont(const char* path);
// void UI_Text_RefreshVerts(Tag* tag);
// void UI_Text_CreateText(Tag* tag);