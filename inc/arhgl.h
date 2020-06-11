//COMMON
void ArhGLInit();

uint glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)

//UNIFORMS
typedef struct Uniform {
    uint ubo;
    uint size;
    uint bind;
    void* data;
} Uniform;

Uniform CreateUniform(int size, void* data, uint bind);
void UpdateUniform(Uniform* Uniform);
void BindUniform(Uniform* uniform);
void SetUniform(Uniform* uniform, void* data);

//TEXTURES
typedef struct Texture{
    uint id;
    uint bind;
} Texture;

Texture CreateIntTexture(const char* data, uint w, uint h, uint bind);
Texture CreateImgTexture(const char* path, uint bind);
Texture CreateTxtTexture(const char* text, int maxw, int maxh, uint bind);
void BindTexture(Texture* texture);

//BUFFERS
typedef struct Buffer{
    uint vao;
    uint vbo;
    uint size;
    uint count;
    uint elemsize;
    uint render_per_elem;
    uint render_type;
    void* data;
} Buffer;

typedef struct Attributes{
    uint size;
    uint type;
    uint normalize;
    uint stride;
    uint offset;
    uint as_int;
} Attributes;

Buffer CreateBuffer(uint elemsize, uint count, uint render_type, void* data, Attributes* attribs);
void UpdateBuffer(Buffer* buffer, int offset, int size);
void AppendBuffer(Buffer* buffer, void* data, int count);
void PushBuffer(Buffer* buffer, void* data);
void RenderBuffer(Buffer* buffer);
void DeleteBuffer(Buffer* buffer);

//SHADERS
uint CreateShader(char* path);
void ShaderPass(uint shader);

//PRESETS
typedef struct Preset{
    Buffer* buff;
    Uniform* uniforms;
    uint shader;
} Preset;

void RenderPreset(Preset* preset);

//VIEWPORT

// extern rect viewport;
extern Uniform viewport_ubo;

// vec2 WinToVec2(int pos);
// vec2 WinToWorld(int pos);
// vec2 V2ToWorld(vec2 pos);
// void UpdateViewport(rect r);
void UpdateTime(int t);