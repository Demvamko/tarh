#include <arh/arhstd.h>
#include <arh/arhres.h>

static char* bin;

void InitResources(){
    bin = ArhLoadFile("./res/bin/pack.bin", 0);
}

void* Res_Get(int offset){
    return bin + offset;
}