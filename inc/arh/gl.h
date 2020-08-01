
typedef struct {
    uint vao;
    uint vbo;
    uint type;
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

uint Arh_Attr_Create(Attribute* attribs);
void Arh_Buff_Create(Buffer* buffer, uint type, void* data, uint vao);
void Arh_Buff_Render(Buffer* buffer);
void Arh_Buff_Update(Buffer* buffer, uint offset, uint size);
void Arh_Buff_Delete(Buffer* buffer);

uint Arh_Shader_Create(int* rvert, int* rfrag);
void Arh_Texture_Create(int* mem, int len, uint bind);
void Arh_Uniform_Create(void* data, uint id);
void Arh_Uniform_Update(uint id, uint offset, uint size);