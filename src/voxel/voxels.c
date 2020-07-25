#include <lib/glew.h>
#include <math.h>
#include <stdlib.h>
#include <arh/gl.h>
#include <voxel/voxels.h>
#include <voxel/def.h>
#include <voxel/tables.h>
#include <voxel/types.h>
#include <ext/pack.h>
#include <arh/std.h>

#define CHUNK_META_U_BIND 1

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
    uchar pos[3];
    uchar light;
    ushort uv[2];
} Vert;

static Attributes VERT_ATTRIBS[] = {
    { 3, GL_UBYTE , 0, sizeof(Vert), 0, 1},
    { 1, GL_UBYTE , 0, sizeof(Vert), 3, 1},
    { 2, GL_USHORT, 1, sizeof(Vert), 4, 0},
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
    shader = CreateShaderRes(VOXEL_SHADER_VERTEX_RANGE ,VOXEL_SHADER_FRAGMENT_RANGE);
    texture = CreateImgTextureRes(ATLAS_VOXEL_RANGE, 0);

    for(int y = 0; y < 4; y++)
    for(int x = 0; x < 4; x++){
        Chunk_Load(x, y);
    }
}

void RenderVoxels(){
    glUseProgram(shader);
    glCheckError();
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

static int neigbours[] = {
     1,  0,  0,
    -1,  0,  0,
     0,  1,  0,
     0, -1,  0,
     0,  0,  1,
     0,  0, -1,
};

char* Chunk_GetVoxel(Chunk* c, int x, int y, int z){
    if(!c)
        return &null;

    int cnx = (x >= CHK_DIM) - (x < 0);
    int cny = (x >= CHK_DIM) - (y < 0);
    int cnz = (z >= CHK_DIM) - (z < 0);

    if(cnx || cny || cnz)
        return &null;

    return &(c->blocks[IDX(x,y,z)]);
}

char* Chunk_GetNeigbour(Chunk* c, int x, int y, int z, int side){
    int nx = x + neigbours[side * 3];
    int ny = y + neigbours[side * 3 + 1];
    int nz = z + neigbours[side * 3 + 2];

    return Chunk_GetVoxel(c, nx, ny, nz);
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

char* Voxel_GetNeighbour(int x, int y, int z, int side){
    int nx = x + neigbours[side * 3];
    int ny = y + neigbours[side * 3 + 1];
    int nz = z + neigbours[side * 3 + 2];

    return Voxel_Get(nx, ny, nz);
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

        c->blocks[id] = BLOCK_AIR;

        if(y < 5 + simplex * 4)
            c->blocks[id] = BLOCK_STONE;
    }

    c->uniform = CreateUniform(2 * sizeof(int), (int[]){ c->x, c->y }, CHUNK_META_U_BIND);
}

void Chunk_RefreshMesh(Chunk* chunk){
    int max = 1024;
    int len = 0;
    Vert* verts = calloc(max, sizeof(Vert));

    int coord[3];

    int idx = 0;

    for(coord[0] = 0; coord[0] < CHK_DIM; coord[0]++)
    for(coord[1] = 0; coord[1] < CHK_DIM; coord[1]++)
    for(coord[2] = 0; coord[2] < CHK_DIM; coord[2]++, idx++)
    {
        char block = *Chunk_GetVoxel(chunk, coord[0], coord[1], coord[2]);

        if(block == BLOCK_AIR)
            continue;

        SIDES {
            char nblock = *Chunk_GetNeigbour(chunk, coord[0], coord[1], coord[2], s);

            if(nblock != BLOCK_AIR)
                continue;

            VERTS {
                #define FOR(x) for(int iter = 0; iter < x; iter++)

                const int* vert = cube_rects[s][v];
                const int* uv = uv_table[s][v];

                ushort* uvs = Arh_GetResource(voxels[block].image);

                FOR(3) verts[len].pos[iter] = vert[iter] + coord[iter];
                FOR(2) verts[len].uv[iter] = uvs[uv[iter] * 2 + iter];
                // FOR(2) verts[len].uv[iter] = uv[iter] * 0xFFFF;

                //CALCULATE AMBIENT OCCLUSION AT VERTEX
                char light = 16;

                for(int i = 0; i < 3; i++){
                    const int* ncoord = occlusion_table[s][v][i];

                    char iblock = *Chunk_GetVoxel(
                        chunk,
                        ncoord[0] + coord[0],
                        ncoord[1] + coord[1],
                        ncoord[2] + coord[2]
                    );

                    if(iblock != BLOCK_AIR)
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

    CreateBuffer(&chunk->buffer, GL_TRIANGLES, len * sizeof(Vert), verts, VERT_ATTRIBS);
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

        if(*Voxel_Get(curr[0], curr[1], curr[2]) != BLOCK_AIR)
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
