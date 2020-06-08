#define CHK_DIM 16
#define CHK_SZ 16 * 16 * 16

typedef struct Chunk{
    char blocks[CHK_SZ];
    int x;
    int y;
    struct Chunk* sides[6];
    struct Meta* meta;
} Chunk;

void GenerateChunkMeta(Chunk* chunk);
void GenerateChunkGeom(Chunk* chunk);
void RenderChunk(Chunk* chunk);
struct Buffer CubeGeom();

Chunk* FindChunk(int x, int y);
Chunk* LoadChunk(int x, int y);
Chunk* GetChunkAbs(int x, int y);

char* GetBlockRel(Chunk* c, int x, int y, int z);
char* GetBlockAbs(int x, int y, int z);

void ChunkGen(Chunk* c);

extern float raycast_hit[3];
extern float raycast_hit_prev[3];