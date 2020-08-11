//pack --type=shader --name=UI --path=./res/arhsl/ui.arhsl

#define UI_UBO_BIND 2
#define UI_FRAME_BIND 4

typedef struct {
    char* string;
    short len;
    char size;
    char font;
} Text;

typedef struct Tag{
    float pos[2];
    float size[2];
    float color[4];
    uint image;

    float client[4];

    struct Tag* parent;
    struct Tag* child;
    struct Tag* sibling;
    struct Vert* vertices;
} Tag;

void UI_Init();

void UI_Tag_AddChild(Tag* parent, Tag* child);

#ifdef UI_USAGE_FUNC

void UI_Render(Buffer* buffer);
void UI_Tag_CreateBuffer(Buffer* buffer, Tag* root, uint count);

#endif