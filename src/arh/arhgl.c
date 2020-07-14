#include <lib/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <arh/arhgl.h>
#include <arh/arhstd.h>

#define STB_IMAGE_IMPLEMENTATION
#include <lib/stb_image.h>

//ARH OPENGL FRAMEWORK

float RECT_VERTS[8] = {
    0.0f, 0.0f, //BL
    0.0f, 1.0f, //TL
    1.0f, 0.0f, //BR
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

    CreateBuffer(&RECT_BUFFER, GL_TRIANGLE_STRIP, 8 * sizeof(float), RECT_VERTS, VERT_ATTRIBS);
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
    
    // stbi_set_flip_vertically_on_load(1);
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
void CreateBuffer(Buffer* buffer, uint type, uint size, void* data, Attributes* attribs){
    buffer->type = type;
    buffer->size = size;
    buffer->data = data;
    InitBuffer(buffer, attribs);
}

void InitBuffer(Buffer* buffer, Attributes* attribs){
    glGenVertexArrays(1, &(buffer->vao));
    glBindVertexArray(buffer->vao);

    glGenBuffers(1, &(buffer->vbo));
    glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
    glBufferData(GL_ARRAY_BUFFER, buffer->size, buffer->data, GL_STATIC_DRAW);

    SetAttributes(attribs);

    glCheckError();
}

void UpdateBuffer(Buffer* buffer, uint offset, uint size){
    if(buffer->size <= size){
        glNamedBufferSubData(buffer->vbo, offset, size, buffer->data);
    }
    else {
        buffer->size = size;
        glNamedBufferData(buffer->vbo, size, buffer->data, GL_STATIC_DRAW);
    }

    glCheckError();
}

void DeleteBuffer(Buffer* buffer){
    glDeleteBuffers(1, &buffer->vbo);
    glDeleteVertexArrays(1, &buffer->vao);

    glCheckError();
}

void RenderBuffer(Buffer* buffer){
    glBindVertexArray(buffer->vao);
    glDrawArrays(buffer->type, 0, buffer->size);

    glCheckError();
}

void SetAttributes(Attributes* attribs){
    for(int i = 0; attribs->type; i++, attribs++){
        glEnableVertexAttribArray(i);

        if(attribs->as_int)
            glVertexAttribIPointer(i, attribs->size, attribs->type, attribs->stride, (void*)attribs->offset);
        else
            glVertexAttribPointer(i, attribs->size, attribs->type, attribs->normalize, attribs->stride, (void*)attribs->offset);
    }
    glCheckError();
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
