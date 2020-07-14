typedef struct Voxel{
    uint image;
    uint flags;
} Voxel;

#define BLOCK_AIR 0
#define BLOCK_STONE 1 

extern Voxel voxels[];