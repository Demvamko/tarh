#include <arh/gl.h>
#include <cglm.h>
#include <lib/glew.h>
#include <arh/cam.h>
#include <stdio.h>

#define CAM_UBO_ID 0

Camera camera = {
    .pos = {4, 10, 3},
    .rot = { -GLM_PI_2f, 0, 0 },

    .up = { 0, 1, 0 },
    .right = { 1, 0, 0 },
    .front = { 0, 0, 1 }
};

void Arh_Camera_Init(int w, int h){
    glm_perspective(0.25f * (float)GLM_PI, (float)w / (float)h, 0.1f, 1000.0f, camera.proj);
    glm_lookat(camera.pos, (vec3) { 0, 0, 0 }, camera.up, camera.view);

    camera.res[0] = w;
    camera.res[1] = h;

    Arh_Uniform_Create(&camera, sizeof(Camera), CAM_UBO_ID);

    Arh_Camera_Rotate(0,0);
    Arh_Camera_Update();
}

void Arh_Camera_Resize(int w, int h){
    glm_perspective(0.25f * (float)GLM_PI, (float)w / (float)h, 0.1f, 1000.0f, camera.proj);
    glViewport(0, 0, w, h);

    camera.res[0] = w;
    camera.res[1] = h;

    Arh_Uniform_Update(CAM_UBO_ID, 0, sizeof(Camera));
}

void Arh_Camera_Update(){
    vec3 look;
    glm_vec3_add(camera.pos, camera.front, look);
    glm_lookat(camera.pos, look, camera.up, camera.view);

    Arh_Uniform_Update(CAM_UBO_ID, 0, sizeof(Camera));
}

void Arh_Camera_Move(float* v){
    vec3 movement;

    if(v[0]){
        glm_vec3_mul(camera.front, (vec3){ v[0], 0, v[0] }, movement);
        glm_vec3_add(camera.pos, movement, camera.pos);
    }
    if(v[1]){
        glm_vec3_mul(camera.right, (vec3){ v[1], 0, v[1] }, movement);
        glm_vec3_add(camera.pos, movement, camera.pos);
    }
    if(v[2]){
        glm_vec3_mul(camera.up, (vec3){ 0, v[2], 0 }, movement);
        glm_vec3_add(camera.pos, movement, camera.pos);
    }

    Arh_Camera_Update();
}

void Arh_Camera_Rotate(float x, float y){
    camera.rot[0] += x;
    camera.rot[1] += y;

    if(camera.rot[1] < -GLM_PI_2f)
        camera.rot[1] = -GLM_PI_2f;
    else if(camera.rot[1] > GLM_PI_2f)
        camera.rot[1] = GLM_PI_2f;


    glm_vec3_copy((vec3){
        cos(camera.rot[1]) * sin(camera.rot[0]),
        sin(camera.rot[1]),
        cos(camera.rot[1]) * cos(camera.rot[0])
    }, camera.front);

    glm_vec3_copy((vec3){
        sin(camera.rot[0] - GLM_PI_2f),
        0,
        cos(camera.rot[0] - GLM_PI_2f)
    }, camera.right);

    glm_vec3_cross(camera.right, camera.front, camera.up); 
}