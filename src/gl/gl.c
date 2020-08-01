#define STB_IMAGE_IMPLEMENTATION
#include <lib/stb_image.h>
#include <lib/glew.h>
#include <arh/gl.h>

//UNIFORMS
static void* arrs[16] = { 0 };
static uint ubos[16] = { 0 };

void Arh_Uniform_Create(void* data, uint id){
    uint ubo = 0;

    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, arrsize(data), data, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, id, ubo);    

    arrs[id] = data;
    ubos[id] = ubo;
}

void Arh_Uniform_Update(uint id, uint offset, uint size){
    glNamedBufferSubData(ubos[id], offset, size, arrs[id]);
}

//TEXTURES
static uint textures[16];

void Arh_Texture_Create(int* mem, int len, uint id){
    uint w, h, n;
    char* data = stbi_load_from_memory(mem, len, &w, &h, &n, STBI_rgb_alpha);

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

