#include "maxHeap.h"
#include <stdio.h>
int test() {
	PageHeap* pageHeap = NULL;
	pageHeap = newPageHeap(10);
	PageCount pageCount;
	pageCount.pageNum = 6;
	pageCount.count = 5;
	pushPageHeap(pageHeap, &pageCount);
	pageCount.pageNum = 10;
	pageCount.count = 9;
	pushPageHeap(pageHeap, &pageCount);
	pageCount.pageNum = 6;
	pageCount.count = 3;
	pushPageHeap(pageHeap, &pageCount);
	getTheFirstElement(pageHeap, &pageCount);
	printf("%d\n", pageCount.pageNum);
	getTheFirstElement(pageHeap, &pageCount);
	printf("%d\n", pageCount.pageNum);
	getTheFirstElement(pageHeap, &pageCount);
	printf("%d\n", pageCount.pageNum);
	freePageHeap(pageHeap);
	return 1;
}