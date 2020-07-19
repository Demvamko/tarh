#include <arh/gl.h>
#include <cglm.h>
#include <lib/glew.h>
#include <arh/cam.h>
#include <stdio.h>

vec3 camera_pos = { 4, 10, 3 };
vec3 camera_rot = { -GLM_PI_2f, 0, 0 };

vec3 camera_up = { 0, 1, 0 };
vec3 camera_right = { 1, 0, 0 };
vec3 camera_front = { 0, 0, 1 };

int camera_resolution[2];

static mat4 matrices[2];

static Uniform uniform;
static Uniform resolution;

#define pos   camera_pos
#define rot   camera_rot
#define up    camera_up
#define right camera_right
#define front camera_front
#define res   camera_resolution

void InitCamera(int w, int h){
    glm_perspective(0.25f * (float)GLM_PI, (float)w / (float)h, 0.1f, 1000.0f, matrices[0]);
    glm_lookat(pos, (vec3) { 0, 0, 0 }, up, matrices[1]);

    res[0] = w;
    res[1] = h;

    uniform = CreateUniform(sizeof(mat4) * 2, matrices, 0);
    resolution = CreateUniform(sizeof(int) * 2, res, 1);

    RotateCamera(0,0);
    ArhCamUpdateView();
}

void ArhCamChangeSize(int w, int h){
    glm_perspective(0.25f * (float)GLM_PI, (float)w / (float)h, 0.1f, 1000.0f, matrices[0]);
    glViewport(0, 0, w, h);

    res[0] = w;
    res[1] = h;

    UpdateUniform(&uniform);
    UpdateUniform(&resolution);
}

void ArhCamUpdateView(){
    vec3 look;
    glm_vec3_add(pos, front, look);
    glm_lookat(pos, look, up, matrices[1]);

    UpdateUniform(&uniform);
}

void ArhCamBindUniform(){
    BindUniform(&uniform);
}

void MoveCamera(float* v){
    vec3 movement;

    if(v[0]){
        glm_vec3_mul(front, (vec3){ v[0], 0, v[0] }, movement);
        glm_vec3_add(pos, movement, pos);
    }
    if(v[1]){
        glm_vec3_mul(right, (vec3){ v[1], 0, v[1] }, movement);
        glm_vec3_add(pos, movement, pos);
    }
    if(v[2]){
        glm_vec3_mul(up, (vec3){ 0, v[2], 0 }, movement);
        glm_vec3_add(pos, movement, pos);
    }

    ArhCamUpdateView();
}

void RotateCamera(float x, float y){

    rot[0] += x;
    rot[1] += y;

    if(rot[1] < -GLM_PI_2f)
        rot[1] = -GLM_PI_2f;
    else if(rot[1] > GLM_PI_2f)
        rot[1] = GLM_PI_2f;


    glm_vec3_copy((vec3){
        cos(rot[1]) * sin(rot[0]),
        sin(rot[1]),
        cos(rot[1]) * cos(rot[0])
    }, front);

    glm_vec3_copy((vec3){
        sin(rot[0] - GLM_PI_2f),
        0,
        cos(rot[0] - GLM_PI_2f)
    }, right);

    glm_vec3_cross(right, front, up); 
}