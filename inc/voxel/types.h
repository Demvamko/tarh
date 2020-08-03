
typedef struct Voxel{
    uint image;
    uint flags;
} Voxel;

#define BLOCK_AIR 0
#define BLOCK_STONE 1
#define BLOCK_GRASS 2
#define BLOCK_DIRT 3

extern Voxel voxels[];