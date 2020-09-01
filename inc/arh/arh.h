#define _CRT_SECURE_NO_WARNINGS

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;

#define lenof(arr) (sizeof(arr) / sizeof(arr[0]))
#define GL_UBYTE GL_UNSIGNED_BYTE
#define GL_USHORT GL_UNSIGNED_SHORT

void* arralloc(uint size, uint elemsize);
void* arrgrow(void* arr);

#define arrraw(arr) ((uint*) (void*) (arr) - 3)
#define arrlen(arr)  arrraw(arr)[2]
#define arrmax(arr)  arrraw(arr)[1]
#define arrelem(arr) arrraw(arr)[0]
#define arrnext(arr) ((((char*)arr) + arrlen(arr)))
#define arrcount(arr) (arrlen(arr) / arrelem(arr))

//pack --type=shader --name=MODEL --path=./res/arhsl/model.arhsl