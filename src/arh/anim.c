// #include <arh/gl.h>
// #include <cglm.h>
// #include <lib/glew.h>

// #define ANIM_UBO 3
// #define ANIM_UBO_SIZE sizeof(float) * 16 * 8

// float matrices[16][8] = { 0 };

// typedef struct Track {
//     int* times;
//     float* quats;
//     int len;
//     int uniform;
// } Track;

// typedef struct Anim {
//     float duration;
//     float start;
//     float end;

//     Track* tracks;
//     uint count;
// } Anim;

// static Anim* anims;

// void Arh_Anim_Init(){
//     Arh_Uniform_Create(matrices, ANIM_UBO_SIZE, ANIM_UBO);
//     anims = arralloc(32, sizeof(Anim));
// }

// uint Arh_Anim_Update(uint id, float t){
//     Anim anim = anims[id];

//     if(anim.end < t)
//         return 0;

//     float dt = t - anim.start;

//     for(int i = 0; i < anim.count; i++){
//         Track* track = anim.tracks + i;
//         glm_quat_mat4(track->quats + (int)dt, matrices[track->uniform]);
//     }

//     Arh_Uniform_Update(ANIM_UBO, 0, ANIM_UBO_SIZE);

//     return 1;
// }

// uint Arh_Anim_Create(uint id, float t){
//     arrlen(anims) = (arrlen(anims) + 1) % arrmax(anims);
//     return arrlen(anims);
// }