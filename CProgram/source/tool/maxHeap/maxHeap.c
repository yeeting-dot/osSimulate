#include "maxHeap.h"
#include <stdio.h>
#include <malloc.h>
PageHeap* newPageHeap(int maxHeap) {
	PageHeap* pageHeap = (PageHeap*)malloc(sizeof(PageHeap));
	if (pageHeap == NULL) {
		printf("pageHeap分配失败\n");
		return NULL;
	}
	PageCount* pageCount = (PageCount*)malloc(maxHeap * sizeof(PageCount));
	if (pageCount == NULL) {
		printf("PageCount分配失败\n");
		return NULL;
	}
	pageHeap->pageCount = pageCount;
	pageHeap->current = 0;
	pageHeap->maxHeap = maxHeap;
	return pageHeap;
}

int preb(PageCount* pageCountA,PageCount* pageCountB,int isMax) {
	return isMax ? pageCountA->count > pageCountB->count :
		pageCountA->count < pageCountB->count;
}

int up(PageHeap* pageHeap,int nodeTag,int choice) {
	if (nodeTag == 0)return 0;
	if (preb(&(pageHeap->pageCount[nodeTag]), &(pageHeap->pageCount[(nodeTag - 1) / 2]), choice))
	{
		PageCount tempCount;
		tempCount.count = pageHeap->pageCount[nodeTag].count;
		tempCount.pageNum = pageHeap->pageCount[nodeTag].pageNum;
		pageHeap->pageCount[nodeTag].count = pageHeap->pageCount[(nodeTag - 1) / 2].count;
		pageHeap->pageCount[nodeTag].pageNum = pageHeap->pageCount[(nodeTag - 1) / 2].pageNum;
		pageHeap->pageCount[(nodeTag - 1) / 2].count = tempCount.count;
		pageHeap->pageCount[(nodeTag - 1) / 2].pageNum = tempCount.pageNum;
		return up(pageHeap, (nodeTag - 1) / 2, choice);
		
	}
	else {
		return nodeTag;
	}
}

int down(PageHeap* pageHeap,int nodeTag,int choice) {
	if ((nodeTag * 2) + 1 >= pageHeap->current)return nodeTag;
	int theMaxChild;
	if ((nodeTag * 2) + 2 >= pageHeap->current) {
		theMaxChild = (nodeTag * 2) + 1;
	}
	else {
		theMaxChild = (pageHeap->pageCount[(nodeTag * 2) + 1].count > 
			pageHeap->pageCount[(nodeTag * 2) + 2].count) ? 
			(nodeTag * 2) + 1 : (nodeTag * 2) + 2;
	}
	if (preb(&(pageHeap->pageCount[theMaxChild]), &(pageHeap->pageCount[nodeTag]), choice))
	{
		PageCount tempCount;
		tempCount.count = pageHeap->pageCount[nodeTag].count;
		tempCount.pageNum = pageHeap->pageCount[nodeTag].pageNum;
		pageHeap->pageCount[nodeTag].count = pageHeap->pageCount[theMaxChild].count;
		pageHeap->pageCount[nodeTag].pageNum = pageHeap->pageCount[theMaxChild].pageNum;
		pageHeap->pageCount[theMaxChild].count = tempCount.count;
		pageHeap->pageCount[theMaxChild].pageNum = tempCount.pageNum;
		return down(pageHeap, theMaxChild, choice);
	}
	else {
		return nodeTag;
	}
}

int pushPageHeap(PageHeap* pageHeap,PageCount* pageCount) {
	if (pageHeap->current == pageHeap->maxHeap) {
		printf("达到最大分配值，无法继续分配\n");
		return -1;
	}
	pageHeap->pageCount[pageHeap->current].pageNum = pageCount->pageNum;
	pageHeap->pageCount[pageHeap->current].count = pageCount->count;
	return up(pageHeap, pageHeap->current++, 1);
}

int getTheFirstElement(PageHeap* pageHeap,PageCount* pageCount) {
	if (pageHeap->current == 0) {
		printf("元素已全部取出\n");
		return -1;
	}
	PageCount* result = &(pageHeap->pageCount[0]);
	pageCount->count = result->count;
	pageCount->pageNum = result->pageNum;
	pageHeap->pageCount[0] = pageHeap->pageCount[pageHeap->current - 1];
	--pageHeap->current;
	return down(pageHeap, 0, 1);
}

int heapForEach(PageHeap* pageHeap, int(*func)(PageCount* pageCount, int size, void* ctx),void* ctx) {
	int result = 1;
	for (int i = 0; i < pageHeap->current; i++) {
		result = result && func(&(pageHeap->pageCount[i]), pageHeap->current, ctx);
	}
	return result;
}

int freePageHeap(PageHeap* pageHeap) {
	free(pageHeap->pageCount);
	free(pageHeap);
	return 1;
}