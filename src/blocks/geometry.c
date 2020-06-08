#include <stdlib.h>
#include <glew.h>
#include <arhgl.h>
#include <arhblocks.h>
#include <arhblock_defines.h>

#define CHUNK_META_U_BIND 1

typedef struct Meta{
    char sides[CHK_DIM * CHK_DIM * CHK_DIM];
    int vert_count;

    Uniform uniform;
    Buffer buff;
} Meta;

typedef struct Vert{
    fvec3 pos;
    fvec2 uv;
    fvec2 ao;
} Vert;

static Attributes ATTR_VERT[] = { 
    { 3, GL_FLOAT, 0, 7 * sizeof(int), 0 }, 
    { 2, GL_FLOAT, 0, 7 * sizeof(int), 3 * sizeof(float) }, 
    { 2, GL_FLOAT, 0, 7 * sizeof(int), 5 * sizeof(float) }, 
    { 0 }
};

void PrepareTextures(){

}

void GenerateChunkMeta(Chunk* chunk){
    Meta* meta = calloc(sizeof(Meta), 1);

    ITER {
        int idx = IDX(x, y, z);
        char block = *GetBlockRel(chunk, x, y, z);

        if(block & TRAIT_TRANSPARENT)
            continue;

        if(block == TYPE_AIR){
            int a = 1;
        }
            

        SIDES {
            block = *GetBlockRel(chunk, NX, NY, NZ);

            if(block & TRAIT_TRANSPARENT){
                meta->sides[idx] = meta->sides[idx] | (1 << s);
                meta->vert_count += 6;
            }
        }
    }

    meta->uniform = CreateUniform(2 * sizeof(int), (int[]){ chunk->x, chunk->y}, CHUNK_META_U_BIND);

    chunk->meta = meta;
}

void GenerateChunkGeom(Chunk* chunk){
    Meta* meta = chunk->meta;
    Vert* verts = malloc(sizeof(Vert) * meta->vert_count);

    int len = 0;

    ITER {
        int idx = IDX(x, y, z);

        SIDES {
            if(meta->sides[idx] & (1 << s)){
                
                VERTS { 
                    fvec3 vert = cube_rects[s][v];

                    verts[len] = (Vert) {
                        (fvec3) { vert.x + x, vert.y + y, vert.z + z },
                        (fvec2) { vert.x, vert.y },
                        (fvec2) { 0.0f, 0.0f }
                    };

                    len++;
                }
            }
        }
    }

    if(!meta->buff.vao)
        meta->buff = CreateBuffer(sizeof(Vert), meta->vert_count, GL_TRIANGLES, verts, ATTR_VERT);
    else
        UpdateBuffer(&(meta->buff), 0, 0);
}

void RenderChunk(Chunk* chunk){
    if(!(chunk->meta))
        GenerateChunkMeta(chunk);

    if(!(chunk->meta->buff.vao))
        GenerateChunkGeom(chunk);

    BindUniform(&(chunk->meta->uniform));
    RenderBuffer(&(chunk->meta->buff));
}

Buffer CubeGeom(){
    Vert* verts = calloc(36, sizeof(Vert));

    int len = 0;

    SIDES VERTS { 
        fvec3 vert = cube_rects[s][v];

        verts[len] = (Vert) {
            (fvec3) { vert.x * 1.1f, vert.y * 1.1f, vert.z * 1.1f },
            (fvec2) { vert.x, vert.y },
            (fvec2) { 0.0f, 0.0f }
        };

        len++;
    }

    return CreateBuffer(sizeof(Vert), 36, GL_TRIANGLES, verts, ATTR_VERT);
}