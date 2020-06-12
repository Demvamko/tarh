
#define IDX(x, y, z) (x + z * CHK_DIM + y * CHK_DIM * CHK_DIM)

#define ITER \
    for(int y = 0, idx = 0; y < CHK_DIM; y++) \
    for(int z = 0; z < CHK_DIM; z++) \
    for(int x = 0; x < CHK_DIM; x++, idx++)

#define SIDES \
    for(int s = 0; s < 6; s++)

#define VERTS \
    for(int v = 0; v < 6; v++)

#define SIDE_X_P_1 0
#define SIDE_X_N_1 1
#define SIDE_Y_P_1 2
#define SIDE_Y_N_1 3
#define SIDE_Z_P_1 4
#define SIDE_Z_N_1 5


static int idx_offsets[] = {
      1,                   //X + 1
    - 1,                   //X - 1
      CHK_DIM * CHK_DIM,   //Y + 1
    - CHK_DIM * CHK_DIM,   //Y - 1
      CHK_DIM,             //Z + 1
    - CHK_DIM              //Z - 1
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

#define RX raycast_hit[0]
#define RY raycast_hit[1]
#define RZ raycast_hit[2]

#define RPX raycast_hit_prev[0]
#define RPY raycast_hit_prev[1]
#define RPZ raycast_hit_prev[2]
