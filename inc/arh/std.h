void* Arh_LoadFile(const char* path, int* outsize);

int Arh_BinarySearch(int* arr, int size, int target);
void Arh_SortedInsert(int* arr, int top, int value);

void Arh_InitResources();
void* Arh_GetResource(int offset);