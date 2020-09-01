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

char* resources;

void Arh_InitResources(){
    resources = Arh_LoadFile("./res/bin/pack_nar.bin", 0);
}

void* arralloc(uint count, uint size){
    uint* arr = malloc(count * size + 3 * sizeof(uint));
    arr += 3;

    arrlen(arr) = 0;
    arrmax(arr) = count * size;
    arrelem(arr) = size; 

    return arr;
}

void* arrgrow(void* arr){
    if(arrlen(arr) + arrelem(arr) * 3 < arrmax(arr))
        return arr;

    arrmax(arr) *= 2;
    uint* narr = realloc(arrraw(arr), arrmax(arr));
    narr += 3;

    return narr;
}