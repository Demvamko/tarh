#include <arhblocks.h>
#include <arhblock_defines.h>
#include <stdlib.h>

float snoise1( float x );
float snoise2( float x, float y );
float snoise3( float x, float y, float z );
float snoise4( float x, float y, float z, float w );

void ChunkGen(Chunk* c){
    ITER {
        int id = IDX(x, y, z);

        int cx = x + c->x * 16.0f;
        int cz = z + c->y * 16.0f;

        float simplex = snoise2(cx / 64.0f, cz / 64.0f);

        c->blocks[id] = TYPE_AIR;

        if(y < 5 + simplex * 4)
            c->blocks[id] = TYPE_STONE;
    }
}
