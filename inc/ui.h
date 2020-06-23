
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
    char* text;

    unsigned char image;
    unsigned char color[4];

    unsigned char type: 2;
    unsigned char alignment: 2;
    unsigned char position: 2;
    unsigned char visible: 1;

    char custom_tag[16];

    int index;

    struct Tag* parent;
    struct Tag* sibling;
} Tag;


void InitUI();
void RenderUI();
void UI_RefreshParents();
void UI_RefreshChildClients(Tag* parent);
void UI_RefreshVerts(Tag* tag);