#ifndef MAX_HEAP
#define MAX_HEAP
typedef struct PageCount {
	int pageNum;
	int count;
}PageCount;

typedef struct PageHeap {
	PageCount* pageCount;
	int current;
	int maxHeap;
}PageHeap;

int preb(PageCount* pageCountA,PageCount* pageCountB,int isMax);
int up(PageHeap* pageHeap, int nodeTag, int choice); 
int down(PageHeap* pageHeap, int nodeTag, int choice);
int pushPageHeap(PageHeap* pageHeap, PageCount* pageCount);
int getTheFirstElement(PageHeap* pageHeap, PageCount* pageCount);
int freePageHeap(PageHeap* pageHeap);
int heapForEach(PageHeap* pageHeap,int(*func)(PageCount* pageCount,int size,void* ctx),void* ctx);
PageHeap* newPageHeap(int maxHeap);

#endif