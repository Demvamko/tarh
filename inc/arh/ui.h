//pack --type=shader --name=UI --path=./res/arhsl/ui.arhsl

#define UI_UBO_BIND 2
#define UI_FRAME_BIND 4

typedef struct {
    char* string;
    short len;
    char size;
    char font;
} Text;

struct Vert;

typedef struct Tag{
    float client[4];
    float rect[4];
    uchar color[4];
    uchar image;
    uchar depth;
} Tag;

void UI_Init(Tag* tags, int count);
void UI_Render();
