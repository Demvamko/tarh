#include <stdio.h>
#include <stdlib.h>
#include <arhblocks.h>
#include <perlin.h>

//Y     IS VERTICAL NORMAL
//X-Z   IS HORIZONTAL PLANE

#define IDX(x, y, z) (x + z * CHK_LENGTH + y * CHK_WIDTH * CHK_LENGTH)

#define ITER3D \
    for(float y = 0; y < CHK_WIDTH; y++) \
    for(float z = 0; z < CHK_LENGTH; z++) \
    for(float x = 0; x < CHK_HEIGHT; x++)
    
#define ITER3DRANGE(from, to) \
    for(float y = from; y < CHK_WIDTH + to; y++) \
    for(float z = from; z < CHK_LENGTH + to; z++) \
    for(float x = from; x < CHK_HEIGHT + to; x++)

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

BlockVertices* ChunkToGeometry(Chunk* c, int* len){
    uint max_verts = 36 * 32;
    BlockVertices* verts = malloc(sizeof(BlockVertices) * max_verts);
    uint verts_len = 0;

    ITER3D{
        int id = IDX(x, y, z);

        for(int i = 0; i < 6; i++){     //ITER OVER NEIGHBOURS            
            int neighbour = id + neighbour_lookup[i];
            
            if(neighbour >= CHK_SZ || neighbour < 0)
                continue;

            if(c->blocks[id] == 0)
                continue;

            if(c->blocks[neighbour] == 1)
                continue;

            for(int v = 0; v < 6; v++){     //ITER OVER VERTICES

                fvec3 vert = cube_rects[i][v];

                verts[verts_len] = (BlockVertices) {
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

                verts_len++;
            }
        }

        if(verts_len + 36 > max_verts){
            max_verts *= 2;
            verts = realloc(verts, sizeof(BlockVertices) * max_verts);
        }
    }

    *len = verts_len;
    return verts;
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