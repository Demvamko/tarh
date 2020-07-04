
#define TYPE_ROW 0
#define TYPE_COL 1
#define TYPE_TABLE 2

#define ALIGN_START 0
#define ALIGN_END 1
#define ALIGN_CENTER 2

#define POSITION_DEFAULT 0
#define POSITION_ANCHORED 1
#define POSITION_ABSOLUTE 2

#define RES_TYPE_TEX 0

typedef struct Tag{
    int client[4];

    float anchors[4];
    int rect[4];

    char depth;

    char* text_str;
    short text_len;
    char font_size;

    struct Text* text;

    float uv[4];
    unsigned char color[4];

    unsigned char type: 2;
    unsigned char alignment: 2;
    unsigned char position: 2;
    unsigned char visible: 1;

    char custom_tag[16];

    int index;

    struct Tag* parent;
    struct Tag* child;
    struct Tag* sibling;
} Tag;

extern int UI_RECT_TABLE[6][2];
extern int UI_UV_TABLE[6][2];

void InitUI();
void RenderUI();

void UI_RefreshParents();
void UI_RefreshChildClients(Tag* parent);
void UI_RefreshVerts(Tag* tag);

void UI_Text_CreateFont(const char* path);
void UI_Text_RefreshVerts(Tag* tag);
void UI_Text_CreateText(Tag* tag);