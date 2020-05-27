#include <stdio.h>
#include <stdlib.h>
#include <arhblocks.h>
#include <perlin.h>

//Y     IS VERTICAL NORMAL
//X-Z   IS HORIZONTAL PLANE

#define IDX(x, y, z) (x + z * CHK_LENGTH + y * CHK_WIDTH * CHK_LENGTH)

#define ITER3D \
    for(int y = 0; y < CHK_HEIGHT; y++) \
    for(int z = 0; z < CHK_LENGTH; z++) \
    for(int x = 0; x < CHK_WIDTH; x++)
    
#define ITER3DRANGE(from, to) \
    for(int y = from; y < CHK_HEIGHT + to; y++) \
    for(int z = from; z < CHK_LENGTH + to; z++) \
    for(int x = from; x < CHK_WIDTH + to; x++)

static fvec3 cube_rects[6][6] = {
    {
        //X + 1
        { 1.0f, 1.0f, 0.0f }, 
        { 1.0f, 1.0f, 1.0f }, 
        { 1.0f, 0.0f, 1.0f }, 
        { 1.0f, 0.0f, 1.0f }, 
        { 1.0f, 0.0f, 0.0f }, 
        { 1.0f, 1.0f, 0.0f }  
    },{
        //X - 1
        { 0.0f, 1.0f, 0.0f }, 
        { 0.0f, 1.0f, 1.0f }, 
        { 0.0f, 0.0f, 1.0f }, 
        { 0.0f, 0.0f, 1.0f }, 
        { 0.0f, 0.0f, 0.0f }, 
        { 0.0f, 1.0f, 0.0f }  
    },{
        //Y + 1
        { 0.0f, 1.0f, 0.0f }, 
        { 0.0f, 1.0f, 1.0f }, 
        { 1.0f, 1.0f, 1.0f }, 
        { 1.0f, 1.0f, 1.0f }, 
        { 1.0f, 1.0f, 0.0f }, 
        { 0.0f, 1.0f, 0.0f }  
    },{
        //Y - 1
        { 0.0f, 0.0f, 0.0f }, 
        { 0.0f, 0.0f, 1.0f }, 
        { 1.0f, 0.0f, 1.0f }, 
        { 1.0f, 0.0f, 1.0f }, 
        { 1.0f, 0.0f, 0.0f }, 
        { 0.0f, 0.0f, 0.0f }  
    },{
        //Z + 1
        { 0.0f, 0.0f, 1.0f }, 
        { 0.0f, 1.0f, 1.0f }, 
        { 1.0f, 1.0f, 1.0f }, 
        { 1.0f, 1.0f, 1.0f }, 
        { 1.0f, 0.0f, 1.0f }, 
        { 0.0f, 0.0f, 1.0f }
    },{
        //Z - 1
        { 0.0f, 0.0f, 0.0f }, 
        { 0.0f, 1.0f, 0.0f }, 
        { 1.0f, 1.0f, 0.0f }, 
        { 1.0f, 1.0f, 0.0f }, 
        { 1.0f, 0.0f, 0.0f }, 
        { 0.0f, 0.0f, 0.0f }
    }
};

static int neighbour_lookup[] = {
      1,                        //X + 1
    - 1,                        //X - 1
      CHK_WIDTH * CHK_LENGTH,   //Y + 1
    - CHK_WIDTH * CHK_LENGTH,   //Y - 1
      CHK_LENGTH,               //Z + 1
    - CHK_LENGTH                //Z - 1
};

float* GenVerticesGrid(){
    float* mem = malloc(sizeof(float) * 3 * (CHK_HEIGHT + 1) * (CHK_LENGTH + 1) * (CHK_WIDTH + 1));
    float* cur = mem;

    ITER3DRANGE(0, 1){
        cur[0] = x;
        cur[1] = y;
        cur[2] = z;

        cur += 3;
    }

    return mem;
}

static int nl[] = {
    1, 0, 0,
    -1, 0, 0,
    0, 1, 0,
    0, -1, 0,
    0, 0, 1,
    0, 0, -1,
};

BlockVertices* ChunkToGeometry(Chunk* c, int* len){
    uint max_verts = 36 * 32;
    BlockVertices* verts = malloc(sizeof(BlockVertices) * max_verts);
    uint verts_len = 0;

    ITER3D{ //INNER
        int id = IDX(x, y, z);

        if(c->blocks[id] == 0)
            continue;

        for(int s = 0; s < 6; s++){           
            char neighbour = GetBlock(c, x + nl[s * 3], y + nl[s * 3 + 1], z + nl[s * 3 + 2]);

            if(neighbour)
                continue;

            GenVerts(x, y, z, s, verts, &verts_len);
        }

        if(verts_len + 36 > max_verts){
            max_verts *= 2;
            verts = realloc(verts, sizeof(BlockVertices) * max_verts);
        }
    }

    *len = verts_len;
    return verts;
}

char GetBlock(Chunk* c, int x, int y, int z){
    if(!c)
        return 0;

    if(x < 0)
        return GetBlock(c->sides[SIDE_X_N_1], x + CHK_WIDTH, y, z);

    if(x > CHK_WIDTH)
        return GetBlock(c->sides[SIDE_X_P_1], x - CHK_WIDTH, y, z);

    if(y < 0)
        return GetBlock(c->sides[SIDE_Y_N_1], x, y + CHK_WIDTH, z);

    if(y > CHK_HEIGHT)
        return GetBlock(c->sides[SIDE_Y_P_1], x, y - CHK_WIDTH, z);

    if(z < 0)
        return GetBlock(c->sides[SIDE_Z_N_1], x, y, z + CHK_WIDTH);

    if(z > CHK_LENGTH)
        return GetBlock(c->sides[SIDE_Z_P_1], x, y, z - CHK_WIDTH);

    return c->blocks[IDX(x,y,z)];
}

void GenVerts(int x, int y, int z, int side, BlockVertices* verts, int* verts_len){
    int len = *verts_len;

    for(int v = 0; v < 6; v++){ 
        fvec3 vert = cube_rects[side][v];

        verts[len] = (BlockVertices) {
            (fvec3) {
                vert.x + x,
                vert.y + y,
                vert.z + z
            },
            (fvec2) {
                vert.x,
                vert.y
            },
            (fvec2) { 0.0f, 0.0f }
        };

        len ++;
    }

    *verts_len = len;
}

void ChunkGen(Chunk* c){
    ITER3D{
        int id = IDX(x, y, z);
        
        double p = Perlin(x, z, 0.2, 5) * 6.0f;
        if(y > p)
            c->blocks[id] = 0;
        else
            c->blocks[id] = 1;
    }
}

BlockVertices* CubeGen(){
    BlockVertices* verts = malloc(sizeof(BlockVertices) * 6 * 6);
    int verts_len = 0;

    for(int i = 0; i < 6; i++)
    for(int v = 0; v < 6; v++){
        fvec3 vert = cube_rects[i][v];

        verts[verts_len] = (BlockVertices) {
            vert,
            (fvec2) { vert.x, vert.y },
            (fvec2) { 0.0f, 0.0f }
        };

        verts_len++;
    }

    return verts;
}