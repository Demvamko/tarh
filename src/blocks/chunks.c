#include <stdlib.h>
#include <arhgl.h>
#include <arhblocks.h>
#include <arhblock_defines.h>

#define VIEW_DISTANCE 4
#define MAX_CHK 16*16

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

    ChunkGen(c);        

    chunks[chunklen] = c;
    chunkcoords[chunklen] = (arhvec2){ x, y };
    chunklen++;

    return c;
}

char GetBlock(Chunk* c, int x, int y, int z){
    if(!c)
        return 0;

    if(x < 0)
        return GetBlock(c->sides[SIDE_X_N_1], x + CHK_WIDTH, y, z);

    if(x >= CHK_WIDTH)
        return GetBlock(c->sides[SIDE_X_P_1], x - CHK_WIDTH, y, z);

    if(y < 0)
        return GetBlock(c->sides[SIDE_Y_N_1], x, y + CHK_WIDTH, z);

    if(y >= CHK_HEIGHT)
        return GetBlock(c->sides[SIDE_Y_P_1], x, y - CHK_WIDTH, z);

    if(z < 0)
        return GetBlock(c->sides[SIDE_Z_N_1], x, y, z + CHK_WIDTH);

    if(z >= CHK_LENGTH)
        return GetBlock(c->sides[SIDE_Z_P_1], x, y, z - CHK_WIDTH);

    return c->blocks[IDX(x,y,z)];
}

char GetBlockAny(int x, int y, int z){
    int cx = x / 16;
    int cy = z / 16;
    int bx = x % 16;
    int bz = z % 16;

    Chunk* c = FindChunk(cx, cy);
    return GetBlock(c, bx, y, bz);
}

void LoadNearPlayer(int x, int y){
    int sx = x / CHK_WIDTH - VIEW_DISTANCE;
    int ex = sx + VIEW_DISTANCE * 2;
    int sy = y / CHK_LENGTH - VIEW_DISTANCE;
    int ey = sy + VIEW_DISTANCE * 2;

    for(int cx = sx; cx < ex; cx++)
    for(int cy = sy; cy < ey; cy++){
        LoadChunk(cx, cy);
    }
}