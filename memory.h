
void* malloc(int numBytes);
void free(void* ptr, int numBytes);
unsigned int mmu_section ( unsigned int add, unsigned int flags );
unsigned int cacheInit(void);
void memInit(void);
