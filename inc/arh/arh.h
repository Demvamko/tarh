#define _CRT_SECURE_NO_WARNINGS

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;

#define lenof(arr) (sizeof(arr) / sizeof(arr[0]))
#define GL_UBYTE GL_UNSIGNED_BYTE
#define GL_USHORT GL_UNSIGNED_SHORT

#define EXPAND(ptr, len, max, size) \
    if(size >= max){ \
        max *= 2; \
        ptr = realloc(ptr, size); \
    } \

typedef struct fvec4{
    float x;
    float y;
    float z;
    float w;
} fvec4;


typedef struct fvec3{
    float x;
    float y;
    float z;
} fvec3;

typedef struct fvec2{
    float x;
    float y;
} fvec2;

typedef struct frect {
    float x;
    float y;
    float w;
    float h;
} frect;

typedef struct arhvec2{
    int x;
    int y;
} arhvec2;

typedef struct arhvec3{
    int x;
    int y;
    int z;
} arhvec3;

void* arralloc(uint size);

#define arrlen(arr) (((uint*)arr)[-1])
#define arrmax(arr) (((uint*)arr)[-2])
#define arrelem(arr) (((uint*)arr)[-3])

#define arrcount(arr) (arrlen(arr) / arrelem(arr))

#define arrexp(arr)                                                 \
    arr = (uint*)realloc(((uint*)arr - 3), arrmax(arr) * 2)) + 3;   \
    arrmax(arr) *= 2;

#define arrpush(arr, elem)          \
    arr[arrcount(arr)] = elem;      \
    arrlen(arr) += 1;

#define arrpushc(arr, elem)         \
    if(arrlen(arr) >= arrmax(arr)){ \
        arrexp(arr)                 \
    }                               \
    arrpush(arr, elem)              \