
typedef struct {
    uint vao;
    uint id;
    uint type;
    uint count;
    uint stride;
    void* data;
} Buffer;

typedef struct {
    uint size;
    uint type;
    uint normalize;
    uint stride;
    uint offset;
    uint as_int;
} Attribute;

uint Arh_Attrib_Create(Attribute* attribs);
void Arh_Buffer_Create(Buffer* buffer, uint type, void* data, uint vao);
void Arh_Buffer_Render(Buffer* buffer);
void Arh_Buffer_Update(Buffer* buffer, uint offset, uint size);
void Arh_Buffer_Delete(Buffer* buffer);

uint Arh_Shader_Create(char* rvert, char* rfrag);

void Arh_TextureArray_Create(char* mem, uint id);
void Arh_Texture_Create(char* mem, uint bind);
void Arh_Texture_Empty(int w, int h, uint id);

void Arh_Frame_Create(uint w, uint h, uint id);
void Arh_Frame_Bind(uint id);
void Arh_Frame_Unbind();

void Arh_Uniform_Create(void* data, uint size, uint id);
void Arh_Uniform_Create_Arr(void* data, uint id);
void Arh_Uniform_Update(uint id, uint offset, uint size);
void Arh_Uniform_Rewrite(uint id, uint offset, uint size, void* data);