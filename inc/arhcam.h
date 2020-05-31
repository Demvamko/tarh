
void ArhCamInit(int w, int h);
void ArhCamChangeSize(int w, int h);
void ArhCamUpdateView();
void ArhCamBindUniform();
void ArhCamMove(vec3 v);
void ArhCamRotate(float x, float y);

vec3* ArhCamGetPos();
vec3* ArhCamGetFront();