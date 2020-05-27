#include <glew.h>
#include <stdlib.h>
#include <stdio.h>
#include "arhgl.h"
#include "arhstd.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//ARH OPENGL FRAMEWORK

// rect viewport;
Uniform viewport_ubo;

int time = 0;
Uniform time_ubo;

// void UpdateViewport(rect r){
//     viewport.x = r.x ? r.x : viewport.x;
//     viewport.y = r.y ? r.y : viewport.y;
//     viewport.w = r.w ? r.w : viewport.w;
//     viewport.h = r.h ? r.h : viewport.h;

//     UpdateUniform(&viewport_ubo);
//     glViewport(0, 0, viewport.w, viewport.h);
// }

void UpdateTime(int t){
    time = t;
    UpdateUniform(&time_ubo);
}

float RECT_VERTS[8] = {
    0.0f, 0.0f, //BL
    0.0f, 1.0f, //TL
    1.0f, 0.0f,  //BR
    1.0f, 1.0f, //TR
};

Attributes VERT_ATTRIBS[2] = { { 2, GL_FLOAT }, { 0 }};
Buffer RECT_BUFFER;

void ArhGLInit(){    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(1, 1, 1, 1);

    glPointSize(4);
    glLineWidth(1);
    // viewport_ubo = CreateUniform(sizeof(int) * 4, &viewport, 0);
    time_ubo = CreateUniform(sizeof(int), &time, 1);

    RECT_BUFFER = CreateBuffer(2 * sizeof(float), 4, GL_TRIANGLE_STRIP, RECT_VERTS, VERT_ATTRIBS);
}

//ERROR CHECKING

uint glCheckError_(const char *file, int line){
    uint errorCode;
    while (errorCode = glGetError())
    {
        char* error;
        switch (errorCode)
        {
            case 0x0500: error = "INVALID_ENUM"; break;
            case 0x0501: error = "INVALID_VALUE"; break;
            case 0x0502: error = "INVALID_OPERATION"; break;
            case 0x0503: error = "STACK_OVERFLOW"; break;
            case 0x0504: error = "STACK_UNDERFLOW"; break;
            case 0x0505: error = "OUT_OF_MEMORY"; break;
            case 0x0506: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        printf("%s:%d : %s\n", file, line, error);
    }
    return errorCode;
}

//RECTS

// void RectToLines(rect r, fvec2* mem){
//     float left = (float)r.x;
//     float bottom = (float)r.y;
//     float right = (float)(r.w + r.x);
//     float top = (float)(r.h + r.y);

//     mem[0] = (fvec2){ left, bottom };
//     mem[1] = (fvec2){ left, top };

//     mem[2] = (fvec2){ left, top };
//     mem[3] = (fvec2){ right, top };

//     mem[4] = (fvec2){ right, bottom };
//     mem[5] = (fvec2){ left, bottom };

//     mem[6] = (fvec2){ right, top };
//     mem[7] = (fvec2){ right, bottom };
// }

// int InRect(int x, int y, rect* r){
//     int inx = x > r->x && x < (r->x + r->w);  
//     int iny = y > r->y && y < (r->y + r->h);

//     return inx && iny;
// }

// rect NewRect(int x, int y, int w, int h){
//     rect r = (rect) { 
//         x, y, w, h
//     };

//     if(r.w < 0){
//         r.w = -r.w;
//         r.x -= r.w;
//     }

//     if(r.h < 0){
//         r.h = -r.h;
//         r.y -= r.h;
//     }

//     return r;
// }

//UNIFORMS

Uniform CreateUniform(int size, void* data, uint bind){
    Uniform uniform;
    uniform.size = size;
    uniform.data = data;
    uniform.bind = bind;

    glGenBuffers(1, &(uniform.ubo));
    glBindBuffer(GL_UNIFORM_BUFFER, uniform.ubo);
    glBufferData(GL_UNIFORM_BUFFER, uniform.size, uniform.data, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, uniform.bind, uniform.ubo);    
    glCheckError();

    return uniform;
}

void UpdateUniform(Uniform* uniform){
    glBindBuffer(GL_UNIFORM_BUFFER, uniform->ubo);
    glBufferData(GL_UNIFORM_BUFFER, uniform->size, uniform->data, GL_STATIC_DRAW);  
    glBindBufferBase(GL_UNIFORM_BUFFER, uniform->bind, uniform->ubo);      
    glCheckError();
}

void BindUniform(Uniform* uniform){
    glBindBufferBase(GL_UNIFORM_BUFFER, uniform->bind, uniform->ubo);  
    glCheckError();
}

void SetUniform(Uniform* uniform, void* data){
    void* map = glMapNamedBufferRange(uniform->ubo, 0, uniform->size, GL_MAP_WRITE_BIT);
    memcpy(map, data, uniform->size);
    glUnmapNamedBuffer(uniform->ubo);
}

//TEXTURES
Texture CreateImgTexture(const char* path, uint bind){   
    uint w, h, n;
    
    stbi_set_flip_vertically_on_load(1);
    char* data = stbi_load(path, &w, &h, &n, STBI_rgb_alpha);

    Texture tex;
    tex.bind = bind;

    glGenTextures(1, &(tex.id));
    glActiveTexture(GL_TEXTURE0 + bind);
    
    glBindTexture(GL_TEXTURE_2D, tex.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    stbi_image_free(data);

    glCheckError();
    return tex;
}

Texture CreateIntTexture(const char* data, uint w, uint h, uint bind){
    Texture tex;
    tex.bind = bind;

    glGenTextures(1, &(tex.id));
    glActiveTexture(GL_TEXTURE0 + bind);

    glBindTexture(GL_TEXTURE_2D, tex.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, w, h, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, data );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glCheckError();
    return tex;
}

Texture CreateTxtTexture(const char* text, int maxw, int maxh, uint bind){
    char* mem = calloc(maxw * maxh, 1);

    int row = maxh - 1;
	int col = 0;

    for(int i = 0; text[i]; i++){
        if(text[i] == '\n'){
            row--;
            col = 0;
            continue;
        }
        if(text[i] == '\t'){
            col += 4 - (col % 4);
            continue;
        }

        mem[ row * maxw + col ] = text[i];
        col++;

        if(col >= maxw || row < 0)
            break;
    }
    
    Texture tex = CreateIntTexture(mem, maxw, maxh, bind);
    free(mem);
    return tex;
}

void BindTexture(Texture* texture){
    glActiveTexture(GL_TEXTURE0 + texture->bind);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glCheckError();
}

//BUFFERS
Attributes RECT_ATTRIBS[2] = { { 4, GL_FLOAT }, { 0 }};
static void SetAttributes(Attributes* attribs);

Buffer CreateBuffer(uint elemsize, uint count, uint render_type, void* data, Attributes* attribs){
    Buffer buffer;

    buffer.count = count;
    buffer.elemsize = elemsize;
    buffer.size = elemsize * count;
    buffer.data = data;
    buffer.render_type = render_type;
    buffer.render_per_elem = 1;

    glGenVertexArrays(1, &(buffer.vao));
    glBindVertexArray(buffer.vao);

    glGenBuffers(1, &(buffer.vbo));
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
    glBufferData(GL_ARRAY_BUFFER, buffer.size, buffer.data, GL_STATIC_DRAW);

    glCheckError();
    SetAttributes(attribs ? attribs : RECT_ATTRIBS);

    return buffer;
}

void UpdateBuffer(Buffer* buffer, int offset, int size){
    glBindVertexArray(buffer->vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
    
    if(!size)
        size = buffer->size;

    glBufferSubData(GL_ARRAY_BUFFER, offset, size, buffer->data);
    glCheckError();
}

void AppendBuffer(Buffer* buffer, void* data, int count){
    void* map = glMapNamedBufferRange(buffer->vbo, buffer->count * buffer->elemsize, buffer->elemsize * count, GL_MAP_WRITE_BIT);
    memcpy(map, data, buffer->elemsize * count);
    buffer->count += count;
    glUnmapNamedBuffer(buffer->vbo);
}

void PushBuffer(Buffer* buffer, void* data){
    AppendBuffer(buffer, data, 1);
}

void SetBuffer(Buffer* buffer, void* data, int count, int idx){
    void* map = glMapNamedBufferRange(buffer->vbo, idx * buffer->elemsize, buffer->elemsize * count, GL_MAP_WRITE_BIT);
    memcpy(map, data, buffer->elemsize * count);
    glUnmapNamedBuffer(buffer->vbo);
}

static void SetAttributes(Attributes* attribs){
    for(int i = 0; attribs->type; i++, attribs++){
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, attribs->size, attribs->type, attribs->normalize, attribs->stride, (void*)attribs->offset);
    }
    glCheckError();
}

void RenderBuffer(Buffer* buffer){
    glBindVertexArray(buffer->vao);
    
    glDrawArrays(buffer->render_type, 0, buffer->count * buffer->render_per_elem);
}

//SHADERS

typedef struct ARH_DEF_TYPE_PAIR{
    char* str;
    uint type;
} ARH_DEF_TYPE_PAIR;

ARH_DEF_TYPE_PAIR ARH_DEF_TYPE_PAIRS[3] = {
    { "#define VERT\n", GL_VERTEX_SHADER },
    { "#define GEOM\n", GL_GEOMETRY_SHADER },
    { "#define FRAG\n", GL_FRAGMENT_SHADER }
};

const char* version = "#version 440\n";

static int CompileFailed(uint shade);

uint CreateShader(char* path){
    uint size;
    char* mem = ArhLoadFile(path, &size);

    uint prog = glCreateProgram();

    for(int i = 0; i < 3; i++){
        ARH_DEF_TYPE_PAIR pair = ARH_DEF_TYPE_PAIRS[i];
        
        const char* sources[3] = { version, pair.str, mem };

        uint shade = glCreateShader(pair.type);
        glShaderSource(shade, 3, sources, NULL);
        glCompileShader(shade);

        if(CompileFailed(shade))
            continue;

        glAttachShader(prog, shade);
    }

    glLinkProgram(prog);
    free(mem);

    glCheckError();
    return prog;
}

static int CompileFailed(uint shade){
    GLint compiled = 0;
    glGetShaderiv(shade, GL_COMPILE_STATUS, &compiled);

    if(compiled)
        return 0;

    char err[2048];
    glGetShaderInfoLog(shade, 2048, NULL, err);
    printf("%s\n", err);

    glCheckError();
    return 1;
}

void ShaderPass(uint shader){
    glUseProgram(shader);
    RenderBuffer(&RECT_BUFFER);
}

//PRESETS

void RenderPreset(Preset* preset){

}