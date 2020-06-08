#include <arhblocks.h>
#include <arhblock_defines.h>
#include <stdlib.h>

void ChunkGen(Chunk* c){
    ITER {
        int id = IDX(x, y, z);

        c->blocks[id] = y > 5 ? TYPE_AIR : TYPE_STONE;
    }
}
