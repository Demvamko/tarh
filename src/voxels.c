#include <voxels.h>
#include <glew.h>
#include <math.h>
#include <stdlib.h>
#include <arhgl.h>
#include <arhblock_defines.h>
#include <arhblock_tables.h>
#include <arhblock_types.h>

#define CHUNK_META_U_BIND 3

struct Chunk{
    char blocks[CHK_SZ];
    int x;
    int y;
    struct Chunk* sides[6];
    char dirty;
    Buffer buffer;
    Uniform uniform;
};

typedef struct Vert{
    unsigned short x : 5;
    unsigned short y : 5;
    unsigned short z : 5;
    unsigned char u : 1;
    unsigned char v : 1;
    unsigned char s : 3;
    unsigned char type;
    unsigned char light;
} Vert;

static Attributes VERT_ATTRIBS[] = { 
    { 1, GL_SHORT, 0, sizeof(Vert), 0, 1 }, 
    { 1, GL_BYTE , 0, sizeof(Vert), 2, 1 }, 
    { 1, GL_BYTE , 0, sizeof(Vert), 3, 1 }, 
    { 1, GL_BYTE , 0, sizeof(Vert), 4, 1 }, 
    { 0 }
};

Chunk* chunks[MAX_CHK] = { 0 };
int keysx[MAX_CHK] = { 0 };
int keysy[MAX_CHK] = { 0 };
int chunklen = 0;

static char null = 0;
static uint shader;
static Texture texture;

void InitVoxels(){
    shader = CreateShader("./res/glsl/blocks.glsl");
    texture = CreateImgTexture("./res/img/tileset.png", 0);

    for(int y = 0; y < 4; y++)
    for(int x = 0; x < 4; x++){
        Chunk_Load(x, y);
    }
}

void RenderVoxels(){
    glUseProgram(shader);
    BindTexture(&texture);   

    for(int i = 0; i < chunklen; i++){
        Chunk* chunk = chunks[i];
    
        if(chunk->dirty)
            Chunk_RefreshMesh(chunk);

        BindUniform(&chunk->uniform);
        RenderBuffer(&chunk->buffer);
    }
}


Chunk* Chunk_Find(int x, int y){
    for(int i = 0; i < MAX_CHK; i++)
        if(keysx[i] == x && keysy[i] == y)
            return chunks[i];

    return 0;
}

Chunk* Chunk_Load(int x, int y){
    Chunk* c = calloc(sizeof(Chunk), 1);

    int z = y;

    for(int s = 0; s < 6; s++){
        c->sides[s] = Chunk_Find(NX, NZ);

        if(c->sides[s])
            c->sides[s]->sides[s + (s & 1 ? -1 : 1)] = c;
    }

    c->x = x;
    c->y = y;

    c->dirty = 1;

    Chunk_Generate(c);        

    chunks[chunklen] = c;
    keysx[chunklen] = x;
    keysy[chunklen] = y;
    chunklen++;

    return c;
}

char* Chunk_GetVoxel(Chunk* c, int x, int y, int z){
    if(!c)
        return &null;

    if(x < 0)
        return Chunk_GetVoxel(c->sides[SIDE_X_N_1], x + CHK_DIM, y, z);

    if(x >= CHK_DIM)
        return Chunk_GetVoxel(c->sides[SIDE_X_P_1], x - CHK_DIM, y, z);

    if(y < 0)
        return Chunk_GetVoxel(c->sides[SIDE_Y_N_1], x, y + CHK_DIM, z);

    if(y >= CHK_DIM)
        return Chunk_GetVoxel(c->sides[SIDE_Y_P_1], x, y - CHK_DIM, z);

    if(z < 0)
        return Chunk_GetVoxel(c->sides[SIDE_Z_N_1], x, y, z + CHK_DIM);

    if(z >= CHK_DIM)
        return Chunk_GetVoxel(c->sides[SIDE_Z_P_1], x, y, z - CHK_DIM);

    return &(c->blocks[IDX(x,y,z)]);
}

char* Voxel_Get(int x, int y, int z){
    int cx = x / 16;
    int cy = z / 16;
    int bx = x % 16;
    int bz = z % 16;

    Chunk* c = Chunk_Find(cx, cy);

    if(!c)
        return &null;

    return Chunk_GetVoxel(c, bx, y, bz);
}

void Voxel_Set(int x, int y, int z, char val){
    int cx = x / 16;
    int cy = z / 16;
    int bx = x % 16;
    int bz = z % 16;

    Chunk* c = Chunk_Find(cx, cy);

    if(!c)
        return;

    *Chunk_GetVoxel(c, bx, y, bz) = val;

    c->dirty = 1;    
}

float snoise2( float x, float y );

void Chunk_Generate(Chunk* c){
    ITER {
        int id = IDX(x, y, z);

        int cx = x + c->x * 16.0f;
        int cz = z + c->y * 16.0f;

        float simplex = snoise2(cx / 64.0f, cz / 64.0f);

        c->blocks[id] = TYPE_AIR;

        if(y < 5 + simplex * 4)
            c->blocks[id] = TYPE_STONE;
    }

    c->uniform = CreateUniform(2 * sizeof(int), (int[]){ c->x, c->y }, CHUNK_META_U_BIND);
}

void Chunk_RefreshMesh(Chunk* chunk){
    int max = 1024;
    int len = 0;
    Vert* verts = calloc(max, sizeof(Vert));

    ITER {
        char block = *Chunk_GetVoxel(chunk, x, y, z);

        if(block & TRAIT_TRANSPARENT)
            continue;

        SIDES {
            char nblock = *Chunk_GetVoxel(chunk, NX, NY, NZ);

            if(!(nblock & TRAIT_TRANSPARENT))
                continue;

            VERTS {
                arhvec3 vert = cube_rects[s][v];

                verts[len].x = vert.x + x;
                verts[len].y = vert.y + y;
                verts[len].z = vert.z + z;

                arhvec2 uv = uv_table[s][v];

                verts[len].u = uv.x;
                verts[len].v = uv.y;

                verts[len].s = s;
                verts[len].type = block;

                //CALCULATE AMBIENT OCCLUSION AT VERTEX
                char light = 16;

                for(int i = 0; i < 3; i++){
                    arhvec3 ncoord = occlusion_table[s][v][i];

                    char iblock = *Chunk_GetVoxel(
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

    if(chunk->buffer.vao)
        DeleteBuffer(&chunk->buffer);

    chunk->buffer = CreateBuffer(sizeof(Vert), len, GL_TRIANGLES, verts, VERT_ATTRIBS);
    chunk->dirty = 0;

    free(verts);
}

#define sign(x) ((x) > 0) - ((x) < 0)

void Voxel_RayCast_Line(float* start, float* end, int maxdist, int* out, int* outprev){
    int curr[3];
    int step[3];
    float next[3];
    float delta[3];

    char last = -1;

    for(int i = 0; i < 3; i++){
        curr[i] = floor(start[i]);
        step[i] = sign(end[i] - start[i]);
        delta[i] = 1.0f / fabs(end[i] - start[i]);

        float min = floor(start[i]);
        float max = min + 1.0f;

        next[i] = ((start[i] > end[i]) ? (start[i] - min) : (max - start[i])) * delta[i];
    }

    for(int i = 0; i < maxdist; i++){
        if(next[0] < next[1] && next[0] < next[2]){
            next[0] += delta[0];
            curr[0] += step[0];
            last = 0;
        }
        else if(next[1] < next[2]){
            next[1] += delta[1];
            curr[1] += step[1];
            last = 1;
        }
        else{
            next[2] += delta[2];
            curr[2] += step[2];
            last = 2;
        }

        if(*Voxel_Get(curr[0], curr[1], curr[2]) != TRAIT_TRANSPARENT)
            break;
    }

    for(int i = 0; i < 3; i++){
        out[i] = curr[i];

        if(last == i)
            curr[i] -= step[last];

        outprev[i] = curr[i];
    }
}

void Voxel_RayCast_Vector(float* start, float* dir, int maxdist, int* out, int* outprev){
    float end[3];

    for(int i = 0; i < 3; i++)
        end[i] = start[i] + dir[i] * maxdist;

    Voxel_RayCast_Line(start, end, maxdist, out, outprev);
}
