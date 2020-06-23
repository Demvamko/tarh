#define _CRT_SECURE_NO_WARNINGS

typedef unsigned int uint;

#define lenof(arr) (sizeof(arr) / sizeof(arr[0]))
#define GL_UBYTE GL_UNSIGNED_BYTE

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
