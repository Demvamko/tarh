
typedef struct { 
    uint res[4];

    float pos[4];
    float rot[4];

    float up[4];
    float right[4];
    float front[4];

    float view[4][4];
    float proj[4][4];
} Camera;

extern Camera camera;

void Arh_Camera_Init(int w, int h);
void Arh_Camera_Resize(int w, int h);
void Arh_Camera_Update();
void Arh_Camera_Move(float* v);
void Arh_Camera_Rotate(float x, float y);