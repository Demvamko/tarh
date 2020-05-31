#include <arhblocks.h>
#include <arhblock_defines.h>
#include <stdlib.h>

void ChunkGen(Chunk* c){
    ITER3D{
        int cx = x + c->x * CHK_WIDTH;
        int cz = z + c->y * CHK_LENGTH;

        int id = IDX(x, y, z);

        c->blocks[id] = y > 5 ? TYPE_AIR : TYPE_STONE;
    }
}
