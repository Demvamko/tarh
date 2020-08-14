//pack --type=shader --name=UI --path=./res/arhsl/ui.arhsl

#define UI_UBO_BIND 2
#define UI_FRAME_BIND 4

typedef struct {
    char* string;
    short len;
    char size;
    char font;
} Text;

typedef struct View {
    uchar color[4];
    uchar image;
} View;

typedef struct Node {
    int id;
    int size;
    struct Node* child;
    struct Node* sibling;
    struct Node* parent;
} Node;

void UI_Render();

void UI_Init(float* mframes, View* mviews, int mcount, char* mparents);

void UI_Tag_UpdateClients(Node* node);
void UI_Tag_UpdateVerts(int id, int count);

uint UI_Tag_CalcNodesSize(Node* node);
Node* UI_Tag_GetHovered(Node* tag, float x, float y);
Node* UI_Tag_GetHoveredTree(float x, float y);