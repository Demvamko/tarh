#include <stdlib.h>
#include <glew.h>
#include <arhgl.h>
#include <arhblocks.h>
#include <arhblock_defines.h>
#include <arhblock_tables.h>

#define CHUNK_META_U_BIND 1

typedef struct Vert{
    unsigned short x : 5;
    unsigned short y : 5;
    unsigned short z : 5;
    unsigned char u : 1;
    unsigned char v : 1;
    unsigned char type;
    unsigned char light;
} Vert;

static Attributes ATTR_VERT[] = { 
    { 1, GL_SHORT, 0, sizeof(Vert), 0, 1 }, 
    { 1, GL_BYTE , 0, sizeof(Vert), 2, 1 }, 
    { 1, GL_BYTE , 0, sizeof(Vert), 3, 1 }, 
    { 1, GL_BYTE , 0, sizeof(Vert), 4, 1 }, 
    { 0 }
};

typedef struct Render{
    Buffer buff;
    Uniform uniform;
} Render;

static Render renders[255];
static int renders_len = 0;

Vert* GenerateChunkMesh(Chunk* chunk, int* out_len){
    int max = 1024;
    int len = 0;
    Vert* verts = calloc(max, sizeof(Vert));

    ITER {
        char block = *GetBlockRel(chunk, x, y, z);

        if(block & TRAIT_TRANSPARENT)
            continue;

        SIDES {
            char nblock = *GetBlockRel(chunk, NX, NY, NZ);

            if(!(nblock & TRAIT_TRANSPARENT))
                continue;

            VERTS {
                arhvec3 vert = cube_rects[s][v];

                verts[len].x = vert.x + x;
                verts[len].y = vert.y + y;
                verts[len].z = vert.z + z;

                verts[len].type = block;

                arhvec2 uv = uv_table[v];

                verts[len].u = uv.x;
                verts[len].v = uv.y;

                short pos = *(short*)(verts + len);

                //CALCULATE AMBIENT OCCLUSION AT VERTEX
                char light = 16;

                for(int i = 0; i < 3; i++){
                    arhvec3 ncoord = occlusion_table[s][v][i];

                    char iblock = *GetBlockRel(
                        chunk,
                        ncoord.x + x,
                        ncoord.y + y,
                        ncoord.z + z
                    );

                    if(!(iblock & TRAIT_TRANSPARENT))
                        light -= 5;
                }

                verts[len].light = light;

                len++;

                if(len >= max){
                    max = max * 2;
                    verts = realloc(verts, max * sizeof(Vert));
                }
            }
        }
    }

    *out_len = len;
    return verts;
}

void GenerateChunkBuff(Chunk* chunk){
    int len = 0;
    Vert* verts = GenerateChunkMesh(chunk, &len);

    if(chunk->render){
        DeleteBuffer(&chunk->render->buff);
        chunk->render->buff = CreateBuffer(sizeof(Vert), len, GL_TRIANGLES, verts, ATTR_VERT);
    }
    else{
        chunk->render = &(renders[renders_len]);
        chunk->render->buff = CreateBuffer(sizeof(Vert), len, GL_TRIANGLES, verts, ATTR_VERT);
        chunk->render->uniform = CreateUniform(2 * sizeof(int), (int[]){ chunk->x, chunk->y }, CHUNK_META_U_BIND);
    
        renders_len++;
    }

    free(verts);
}

void RenderChunk(Chunk* chunk){
    if(chunk->dirty){
        GenerateChunkBuff(chunk);
        chunk->dirty = 0;
    }

    BindUniform(&chunk->render->uniform);
    RenderBuffer(&chunk->render->buff);
}
