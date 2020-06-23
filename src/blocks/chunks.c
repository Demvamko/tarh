#include <glew.h>
#include <stdlib.h>
#include <arhgl.h>
#include <arhblocks.h>
#include <arhblock_defines.h>

#define VIEW_DISTANCE 4
#define MAX_CHK 16*16

static char null = 0;

Chunk* chunks[MAX_CHK] = { 0 };
arhvec2 chunkcoords[MAX_CHK] = { 0 };

int chunklen = 0;

Chunk* FindChunk(int x, int y){
    for(int i = 0; i < MAX_CHK; i++){
        arhvec2 coord = chunkcoords[i];

        if(coord.x == x && coord.y == y)
            return chunks[i];
    }

    return 0;
}

Chunk* LoadChunk(int x, int y){
    Chunk* c = calloc(sizeof(Chunk), 1); //TODO: Memory leaking

    int z = y;

    for(int s = 0; s < 6; s++){
        c->sides[s] = FindChunk(NX, NZ);

        if(c->sides[s])
            c->sides[s]->sides[s + (s & 1 ? -1 : 1)] = c;
    }

    c->x = x;
    c->y = y;

    c->dirty = 1;

    ChunkGen(c);        

    chunks[chunklen] = c;
    chunkcoords[chunklen] = (arhvec2){ x, y };
    chunklen++;

    return c;
}

char* GetBlockRel(Chunk* c, int x, int y, int z){
    if(!c)
        return &null;

    if(x < 0)
        return GetBlockRel(c->sides[SIDE_X_N_1], x + CHK_DIM, y, z);

    if(x >= CHK_DIM)
        return GetBlockRel(c->sides[SIDE_X_P_1], x - CHK_DIM, y, z);

    if(y < 0)
        return GetBlockRel(c->sides[SIDE_Y_N_1], x, y + CHK_DIM, z);

    if(y >= CHK_DIM)
        return GetBlockRel(c->sides[SIDE_Y_P_1], x, y - CHK_DIM, z);

    if(z < 0)
        return GetBlockRel(c->sides[SIDE_Z_N_1], x, y, z + CHK_DIM);

    if(z >= CHK_DIM)
        return GetBlockRel(c->sides[SIDE_Z_P_1], x, y, z - CHK_DIM);

    return &(c->blocks[IDX(x,y,z)]);
}

char* GetBlockAbs(int x, int y, int z){
    int cx = x / 16;
    int cy = z / 16;
    int bx = x % 16;
    int bz = z % 16;

    Chunk* c = FindChunk(cx, cy);
    return GetBlockRel(c, bx, y, bz);
}

Chunk* GetChunkAbs(int x, int y){
    int cx = x / 16;
    int cy = y / 16;

    return FindChunk(cx, cy);
}

void LoadNearPlayer(int x, int y){
    int sx = x / CHK_DIM - VIEW_DISTANCE;
    int ex = sx + VIEW_DISTANCE * 2;
    int sy = y / CHK_DIM - VIEW_DISTANCE;
    int ey = sy + VIEW_DISTANCE * 2;

    for(int cx = sx; cx < ex; cx++)
    for(int cy = sy; cy < ey; cy++){
        LoadChunk(cx, cy);
    }
}

static uint shader;
static Texture texture;

void InitChunks(){
    shader = CreateShader("./res/glsl/blocks.glsl");
    texture = CreateImgTexture("./res/img/tileset.png", 0);

    for(int y = 0; y < 4; y++)
    for(int x = 0; x < 4; x++){
        LoadChunk(x, y);
    }
}

void RenderChunks(){
    glUseProgram(shader);
    BindTexture(&texture);   

    for(int i = 0; i < chunklen; i++)
        RenderChunk(chunks[i]);
}