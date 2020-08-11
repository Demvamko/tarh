#define VIEW_DISTANCE 4
#define CHK_DIM 16
#define MAX_CHK CHK_DIM * CHK_DIM
#define CHK_SZ CHK_DIM * CHK_DIM * CHK_DIM

//pack --type=atlas --name=VOXEL --path=./res/img/block/*.png
//pack --type=shader --name=VPASS --path=./res/arhsl/pass.arhsl
//pack --type=shader --name=VOXEL --path=./res/arhsl/blocks.arhsl
//pack --type=raw --name=VOXEL_ARR --path=./res/img/block/*.png
//pack --type=icon --process=voxel --size=64 --name=ICE --path=./res/img/block/dirt.png

typedef struct Chunk Chunk;

void InitVoxels();
void RenderVoxels();

Chunk* Chunk_Find(int x, int y);
Chunk* Chunk_Load(int x, int y);

void Chunk_Generate(Chunk* c);
void Chunk_RefreshMesh(Chunk* chunk);

char* Voxel_Get(int x, int y, int z);
void Voxel_Set(int x, int y, int z, char val);
char* Chunk_GetVoxel(Chunk* c, int x, int y, int z);

void Voxel_RayCast_Line(float* start, float* end, int maxdist, int* out, int* outprev);
void Voxel_RayCast_Vector(float* start, float* dir, int maxdist, int* out, int* outprev);