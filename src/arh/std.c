#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arh/std.h>

void* Arh_LoadFile(const char* path, int* outsize){
    FILE* file = fopen(path, "rb");

    fseek(file, 0, SEEK_END);
    uint size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* mem = malloc(size + 1);
    fread(mem, 1, size, file);
    mem[size] = 0;

    fclose(file);

    if(outsize)
        *outsize = size;
    
    return mem;
}

int Arh_BinarySearch(int* arr, int size, int target) 
{ 
    int low = 0;
    int high = size;
    int mid = size / 2;

    while(low != mid && high != mid){
        if(arr[mid] == target)
            return mid;

        if(arr[mid] > target)
            mid = (mid + high) / 2;
        else
            mid = (low + mid) / 2;
    }

    return -1;
}

void Arh_SortedInsert(int* arr, int top, int value){
    int cursor = top - 1;

    while(arr[cursor] > value){
        arr[cursor + 1] = arr[cursor];
        cursor--;
    }

    arr[cursor + 1] = arr[cursor];
    arr[cursor] = value;
}

static char* binary;

void Arh_InitResources(){
    binary = Arh_LoadFile("./res/bin/pack.bin", 0);
}

void* Arh_GetResource(int offset){
    return binary + offset;
}

void* arralloc(uint size){
    uint* mem = malloc(size + 2 * sizeof(uint));
    mem[0] = size;
    mem[1] = 0;

    return mem + 2;
}