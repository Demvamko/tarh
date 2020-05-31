
#define IDX(x, y, z) (x + z * CHK_LENGTH + y * CHK_WIDTH * CHK_LENGTH)

#define ITER3D \
    for(int y = 0; y < CHK_HEIGHT; y++) \
    for(int z = 0; z < CHK_LENGTH; z++) \
    for(int x = 0; x < CHK_WIDTH; x++)
    
#define ITER3DRANGE(from, to) \
    for(int y = from; y < CHK_HEIGHT + to; y++) \
    for(int z = from; z < CHK_LENGTH + to; z++) \
    for(int x = from; x < CHK_WIDTH + to; x++)

#define ITERSIDES \
    for(int s = 0; s < 6; s++)

#define SIDE_X_P_1 0
#define SIDE_X_N_1 1
#define SIDE_Y_P_1 2
#define SIDE_Y_N_1 3
#define SIDE_Z_P_1 4
#define SIDE_Z_N_1 5

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


static int idx_offsets[] = {
      1,                        //X + 1
    - 1,                        //X - 1
      CHK_WIDTH * CHK_LENGTH,   //Y + 1
    - CHK_WIDTH * CHK_LENGTH,   //Y - 1
      CHK_LENGTH,               //Z + 1
    - CHK_LENGTH                //Z - 1
};

static int coord_offsets[] = {
    1, 0, 0,
    -1, 0, 0,
    0, 1, 0,
    0, -1, 0,
    0, 0, 1,
    0, 0, -1,
};

#define NX x + coord_offsets[s * 3]
#define NY y + coord_offsets[s * 3 + 1]
#define NZ z + coord_offsets[s * 3 + 2]

#define TRAIT_TRANSPARENT (char)0b10000000

#define TYPE_AIR (0 | TRAIT_TRANSPARENT)
#define TYPE_GLASS (1 | TRAIT_TRANSPARENT)

#define TYPE_STONE 1
#define TYPE_DIRT 2
#define TYPE_BRICK 3
