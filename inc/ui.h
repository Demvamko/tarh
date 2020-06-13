
#define TYPE_ROW 0
#define TYPE_COL 1
#define TYPE_TABLE 2

#define ALIGN_START 0
#define ALIGN_CENTER 1
#define ALIGN_END 2

#define DISPLAY_OVERFLOW 0
#define DISPLAY_FIT 1

#define POSITION_DEFAULT 0
#define POSITION_ANCHORED 1
#define POSITION_ABSOLUTE 2

typedef struct Tag{
    float anchors[4];
    float rect[4];

    char depth;
    char* text;

    unsigned char type: 2;
    unsigned char alignment: 2;
    unsigned char display: 1;
    unsigned char position: 2;

    char custom_tag[16];

    struct Tag* parent;
    struct Tag* sibling;
} Tag;

Tag* UI_LoadFile(char* path, int* out_count);
Tag* UI_ParseFile(char* file, int size, int* out_count);
void UI_ParseTag(Tag* tag, char* line, Tag* last);