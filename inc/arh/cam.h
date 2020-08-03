
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

void InitCamera(int w, int h);
void ArhCamChangeSize(int w, int h);
void ArhCamUpdateView();
void ArhCamBindUniform();
void MoveCamera(float* v);
void RotateCamera(float x, float y);