#define CGLTF_IMPLEMENTATION
#include <lib/cgltf.h>
#include <stdio.h>
#include <lib/glew.h>
#include <arh/gl.h>
#include <ext/pack_nar.h>
#include <arh/std.h>

typedef struct Vert {
    float pos[3];
    float uv[2];
    char skin;
} Vert;

typedef struct Keyframe {
    float pos[3];
    float rot[3];
    float scl[3];
} Keyframe;

static Vert* verts;
static Buffer buffer;
static uint shader;

void ExperimentRender(){
    Arh_Shader_Bind(shader);
    Arh_Buffer_Render(&buffer);
}

static Attribute VERT_ATTR[] = {
    { 3, GL_FLOAT, 0, sizeof(Vert), 0, 0 },
    { 2, GL_FLOAT, 0, sizeof(Vert), 12, 0 },
    { 3, GL_UBYTE, 0, sizeof(Vert), 20, 0 },
    { 0 }
};

void* CGL_GetBuffer(cgltf_accessor* acc){
    cgltf_buffer_view* bview = acc->buffer_view;
    return (char*)bview->buffer->data + bview->offset + acc->offset;
}

void Experimenting(){
    
    cgltf_options options = { 0 };
    cgltf_data* data = 0;

    cgltf_result result = cgltf_parse_file(&options, "./res/obj/ugh.glb", &data);

    cgltf_load_buffers(&options, data, "./res/obj/ugh.glb");

    for(int m = 0; m < data->meshes_count; m++){
        cgltf_mesh* mesh = data->meshes + m;
        cgltf_primitive* prim = mesh->primitives;

        ushort* indices = CGL_GetBuffer(prim->indices);

        verts = arralloc((uint)prim->indices->count, sizeof(Vert));

        float* pos;
        float* uv;
        ushort* skin;

        int s_pos;
        int s_uv;
        int s_skin;

        for(int i = 0; i < prim->attributes_count; i++){
            cgltf_attribute* attr = prim->attributes + i;

            if(attr->type == cgltf_attribute_type_position){
                pos = CGL_GetBuffer(attr->data);
                s_pos = (int)attr->data->stride;
            }
            else if(attr->type == cgltf_attribute_type_texcoord){
                uv = CGL_GetBuffer(attr->data);
                s_uv = (int)attr->data->stride;
            }
            else if(attr->type == cgltf_attribute_type_joints){
                skin = CGL_GetBuffer(attr->data);
                s_skin = (int)attr->data->stride;
            }
        }

        for(int i = 0; i < prim->indices->count; i++){
            Vert* vert = verts + i;
            int index = indices[i];

            float* mpos = (float*)((char*)pos + index * s_pos);
            float* muv = (float*)((char*)uv + index * s_uv);
            ushort* mskin = (ushort*)((char*)skin + index * s_skin);

            #define FOR(x) for(int p = 0; p < x; p++)

            FOR(3) vert->pos[p] = mpos[p];
            FOR(2) vert->uv[p] = muv[p];
            vert->skin = mskin[0];

            // printf("VERT %5d : (%8.3f,%8.3f,%8.3f) (%8.3f,%8.3f) (%u)\n", i, vert->pos[0],vert->pos[1],vert->pos[2], vert->uv[0], vert->uv[1], vert->skin);
        }

    }

    arrlen(verts) = arrmax(verts);

    int format = Arh_Attrib_Create(VERT_ATTR);
    Arh_Buffer_Create(&buffer, GL_TRIANGLES, verts, format);
    shader = Arh_Shader_Create(NAR_MODEL_VERT, NAR_MODEL_FRAG);

        
}
