

extern float camera_pos[3];
extern float camera_rot[3];
extern float camera_up[3];
extern float camera_right[3];
extern float camera_front[3];
extern int camera_resolution[2];

void InitCamera(int w, int h);
void ArhCamChangeSize(int w, int h);
void ArhCamUpdateView();
void ArhCamBindUniform();
void MoveCamera(float* v);
void RotateCamera(float x, float y);