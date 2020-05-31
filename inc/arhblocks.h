#define CHK_WIDTH 16
#define CHK_LENGTH 16
#define CHK_HEIGHT 16
#define CHK_SZ 16 * 16 * 16

typedef struct Chunk{
    char blocks[CHK_WIDTH * CHK_LENGTH * CHK_HEIGHT];
    int x;
    int y;
    struct Chunk* sides[6];
    struct Meta* meta;
} Chunk;

void UpdateChunkMeta(Chunk* chunk, int x, int y, int z, char checked_neigbours);
void GenerateChunkMeta(Chunk* chunk);
void GenerateChunkGeom(Chunk* chunk);
void RenderChunk(Chunk* chunk);
struct Buffer CubeGeom();

Chunk* FindChunk(int x, int y);
Chunk* LoadChunk(int x, int y);

char GetBlock(Chunk* c, int x, int y, int z);
char GetBlockAny(int x, int y, int z);

void ChunkGen(Chunk* c);
