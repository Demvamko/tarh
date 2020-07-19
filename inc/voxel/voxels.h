#define VIEW_DISTANCE 4
#define MAX_CHK 16*16

#define CHK_DIM 16
#define CHK_SZ 16 * 16 * 16

//pack --type="shader" --id="VOXEL_SHADER" --path="./res/arhsl/blocks.arhsl"

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