#define CHK_WIDTH 16
#define CHK_LENGTH 16
#define CHK_HEIGHT 16
#define CHK_SZ 16 * 16 * 16

typedef struct Chunk{
    char blocks[CHK_WIDTH * CHK_LENGTH * CHK_HEIGHT];
    short x;
    short y;
} Chunk;

typedef struct BlockVertices{
    fvec3 pos;
    fvec2 uv;
    fvec2 ao;
} BlockVertices;

void ChunkGen(Chunk* c);
BlockVertices* ChunkToGeometry(Chunk* c, int* verts_len);
BlockVertices* CubeGen();

Chunk* GetChunk(int x, int y);
void SetChunk(int x, int y, Chunk* c);
char GetBlock(int x, int y, int z);