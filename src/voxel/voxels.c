#include <gl/glew.h>
#include <math.h>
#include <stdlib.h>
#include <arh/gl.h>
#include <voxel/voxels.h>
#include <voxel/def.h>
#include <voxel/tables.h>
#include <voxel/types.h>
#include <ext/pack_nar.h>
#include <arh/std.h>

#define CHUNK_UBO_BIND 1
#define CHUNK_TEX_BIND 1

typedef struct Node{
    float g;
    float h;
    float f;
    
    char x;
    char y;
    char z;
    char px;
    char py;
    char pz;

    char closed;
    char walkable;
} Node;

struct Chunk{
    char blocks[CHK_SZ];
    Node nodes[CHK_SZ];
    int x;
    int y;
    struct Chunk* sides[6];
    char dirty;
    Buffer buffer;
};

typedef struct Vert{
    uchar pos[3];
    uchar light;
    uchar uv;
} Vert;

static Attribute VERT_ATTRIBS[] = {
    { 3, GL_UBYTE, 0, sizeof(Vert), 0, 1},
    { 1, GL_UBYTE, 0, sizeof(Vert), 3, 1},
    { 1, GL_UBYTE, 0, sizeof(Vert), 4, 1},
    { 0 }
};

static Chunk* chunks[MAX_CHK] = { 0 };
static int keysx[MAX_CHK] = { 0 };
static int keysy[MAX_CHK] = { 0 };
static int chunklen = 0;

static char null = 0;

static uint shader;
static uint format;

void InitVoxels(){
    Arh_TextureArray_Create(NAR_VOXEL_ARR, CHUNK_TEX_BIND);

    format = Arh_Attrib_Create(VERT_ATTRIBS);
    shader = Arh_Shader_Create(NAR_VOXEL_VERT, NAR_VOXEL_FRAG);
    Arh_Uniform_Create((int[]){ 0, 0 }, sizeof(int) * 2, CHUNK_UBO_BIND);
    
    for(int y = 0; y < 4; y++)
    for(int x = 0; x < 4; x++){
        Chunk_Load(x, y);
    }
}

void RenderVoxels(){
    Arh_Shader_Bind(shader);

    for(int i = 0; i < chunklen; i++){
        Chunk* chunk = chunks[i];
    
        if(chunk->dirty)
            Chunk_RefreshMesh(chunk);

        Arh_Uniform_Rewrite(CHUNK_UBO_BIND, 0, sizeof(int) * 2, &(chunk->x));
        Arh_Buffer_Render(&chunk->buffer);
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
    int cx = x / CHK_DIM;
    int cy = z / CHK_DIM;
    int bx = x % CHK_DIM;
    int bz = z % CHK_DIM;

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
    int cx = x / CHK_DIM;
    int cy = z / CHK_DIM;
    int bx = x % CHK_DIM;
    int bz = z % CHK_DIM;

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

        int cx = x + c->x * (float)CHK_DIM;
        int cz = z + c->y * (float)CHK_DIM;

        float simplex = snoise2(cx / 64.0f, cz / 64.0f);
        float simplex2 = snoise2(cz / 64.0f, cx / 64.0f);

        c->blocks[id] = BLOCK_AIR;

        // if(y < 5 + simplex * 4)
        //     c->blocks[id] = y < (5 + simplex2 * 4) ? BLOCK_DIRT : BLOCK_GRASS;

    
        if(y < 5 + simplex * 4)
            c->blocks[id] = BLOCK_STONE;
    }
}

void Chunk_RefreshMesh(Chunk* chunk){
    Vert* verts = arralloc(1024, sizeof(Vert));

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

                Vert* vert = (Vert*)arrnext(verts);

                const int* rect = cube_rects[s][v];
                const int* uv = uv_table[s][v];

                FOR(3) vert->pos[iter] = rect[iter] + coord[iter];
                vert->uv = (voxels[block].image << 2) + (uv[1] << 1) + uv[0];

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

                vert->light = light;

                arrlen(verts) += sizeof(Vert);
                verts = arrgrow(verts);
            }
        }
    }

    if(chunk->buffer.vao)
        Arh_Buffer_Delete(&chunk->buffer);

    Arh_Buffer_Create(&chunk->buffer, GL_TRIANGLES, verts, format);
    chunk->dirty = 0;

    free(arrraw(verts));
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
