#define STB_IMAGE_IMPLEMENTATION
#include <lib/stb_image.h>
#include <lib/glew.h>
#include <arh/gl.h>

//UNIFORMS
static void* arrs[16] = { 0 };
static uint ubos[16] = { 0 };

void Arh_Uniform_Create(void* data, uint size, uint id){
    uint ubo = 0;

    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, id, ubo);    

    arrs[id] = data;
    ubos[id] = ubo;
}

void Arh_Uniform_Create_Arr(void* data, uint id){
    uint size = arrlen(data);
    Arh_Uniform_Create(data, size, id);
}

void Arh_Uniform_Update(uint id, uint offset, uint size){
    glNamedBufferSubData(ubos[id], offset, size, arrs[id]);
}

void Arh_Uniform_Rewrite(uint id, uint offset, uint size, void* data){
    glNamedBufferSubData(ubos[id], offset, size, data);
}

//TEXTURES
static uint textures[16];

void Arh_Texture_Create(char* mem, uint id){
    uint w, h, n;
    char* data = stbi_load_from_memory(mem, arrlen(mem), &w, &h, &n, STBI_rgb_alpha);

    uint tex;
 
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0 + id);

    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    stbi_image_free(data);

    textures[id] = tex;
}

void Arh_Texture_Empty(int w, int h, uint id){
    uint tex = 0;

    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0 + id);

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    textures[id] = tex;
}

//FRAMEBUFFERS
static uint frames[16];

void Arh_Frame_Create(uint w, uint h, uint id){
    Arh_Texture_Empty(w, h, id);

    uint frame = 0;

    glGenFramebuffers(1, &frame);
    glBindFramebuffer(GL_FRAMEBUFFER, frame);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[id], 0);

    frames[id] = frame;
}

void Arh_Frame_Bind(uint id){
    glBindFramebuffer(GL_FRAMEBUFFER, frames[id]);
}

void Arh_Frame_Unbind(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}