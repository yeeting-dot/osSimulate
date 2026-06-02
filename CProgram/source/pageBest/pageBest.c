#include "pageBest.h"
#include "hashTable.h"
#include "maxHeap.h"
#include <stdio.h>
#include <malloc.h>

int pageBest_updateDynamicWindow(PageBest_DynamicWindow* pageBest_DynamicWindow
								,PageBest_GlobalStructs* pageBest_GlobalStructs
								,PageBest_Evaluation* pageBest_Evaluation) {
	PageCount pageCount;
	int needInsertPageNum = pageBest_GlobalStructs->dynamicPagesArray[pageBest_GlobalStructs->dynamicPagesArrayIndex++];
	HashNode* nodeInGlobal = getHash(pageBest_GlobalStructs->startPagesHashTable, needInsertPageNum);
	if (nodeInGlobal == NULL) {
		printf("Hash表中不存在该节点\n");
		return 0;
	}
	pageCount.pageNum = nodeInGlobal->key;
	pageCount.count = --nodeInGlobal->value;
	if (getHash(pageBest_DynamicWindow->dynamicHashTable, needInsertPageNum)) {
		printf("contains the pageNum\n");
		return 1;
	}
	if (pageBest_DynamicWindow->dynamicHeap->current < pageBest_DynamicWindow->dynamicHeap->maxHeap) {
		pushPageHeap(pageBest_DynamicWindow->dynamicHeap, &pageCount);
		insertHash(pageBest_DynamicWindow->dynamicHashTable, nodeInGlobal->key, nodeInGlobal->value);
		pageBest_Evaluation->lostPageCount += 1;
		return 1;
	}
	//取Heap的第一个元素，并push pageCount
	PageCount temp;
	getTheFirstElement(pageBest_DynamicWindow->dynamicHeap, &temp);
	deleteHash(pageBest_DynamicWindow->dynamicHashTable, temp.pageNum);
	pushPageHeap(pageBest_DynamicWindow->dynamicHeap, &pageCount);
	insertHash(pageBest_DynamicWindow->dynamicHashTable, nodeInGlobal->key, nodeInGlobal->value);
	pageBest_Evaluation->lostPageCount += 1;
	pageBest_Evaluation->insteadCount += 1;
	return 1;
}

int pageBest_showEvaluation(PageBest_Evaluation* pageBest_Evaluation) {
	printf("lost page nums: %8d instead nums: %8d lost page frequency: %10lf\n"
		, pageBest_Evaluation->lostPageCount
		, pageBest_Evaluation->insteadCount
		, (double)pageBest_Evaluation->lostPageCount / pageBest_Evaluation->totalPages);
}

int pageBest_preHanlePagesData(PageBest_GlobalStructs* pageBest_GlobalStructs) {
	int times = pageBest_GlobalStructs->startPagesHashTable->size;
	for (int i = 0; i < times; i++) {
		scanf("%d", &pageBest_GlobalStructs->dynamicPagesArray[i]);
		HashNode* hashNode = getHash(pageBest_GlobalStructs->startPagesHashTable,
			pageBest_GlobalStructs->dynamicPagesArray[i]);
		if (hashNode != NULL) {
			hashNode->value += 1;
			continue;
		}
		insertHash(pageBest_GlobalStructs->startPagesHashTable,pageBest_GlobalStructs->dynamicPagesArray[i],1);
	}
	return 1;
}

int pageBest_showDynamic(HashNode* hashNode, int size, void* ctx) {
	static int count = 0;
	printf("count: %8d pageNum: %8d leftCount: %8d\n", (count++) % size, hashNode->key, hashNode->value);
	return 1;
}

int pageBest_initAllStructs(PageBest_GlobalStructs** pageBest_GlobalStructs
	, PageBest_DynamicWindow** pageBest_DynamicWindow,PageBest_Evaluation** pageBest_Evaluation){
	int pages;
	int windowSize;
	printf("How much pages you want to allocate: ");
	scanf("%d", &pages);
	printf("How spaces you want to allocate for memory: ");
	scanf("%d", &windowSize);
	
	*pageBest_GlobalStructs = (PageBest_GlobalStructs*)malloc(sizeof(PageBest_GlobalStructs));
	if (*pageBest_GlobalStructs == NULL) {
		printf("can not allocate enough space for PageBest_GlobalStructs\n");
		return 0;
	}
	*pageBest_DynamicWindow = (PageBest_DynamicWindow*)malloc(sizeof(PageBest_DynamicWindow));
	if (*pageBest_DynamicWindow == NULL) {
		printf("can not allocate enough space for PageBest_DynamicWindow\n");
		return 0;
	}
	*pageBest_Evaluation = (PageBest_Evaluation*)malloc(sizeof(PageBest_Evaluation));
	if (*pageBest_Evaluation == NULL) {
		printf("can not allocate enough space for PageBest_Evaluation\n");
		return 0;
	}
	(*pageBest_GlobalStructs)->startPagesHashTable = newHashTable(pages);
	(*pageBest_GlobalStructs)->dynamicPagesArray = (int*)malloc(pages * sizeof(int));
	(*pageBest_GlobalStructs)->dynamicPagesArrayIndex = 0;
	(*pageBest_DynamicWindow)->dynamicHashTable = newHashTable(windowSize);
	(*pageBest_DynamicWindow)->dynamicHeap = newPageHeap(windowSize);
	(*pageBest_Evaluation)->totalPages = pages;
	(*pageBest_Evaluation)->insteadCount = 0;
	(*pageBest_Evaluation)->lostPageCount = 0;
	if ((*pageBest_GlobalStructs)->dynamicPagesArray == NULL) {
		printf("can not allocate enough space for PageBest_GlobalStructs->dynamicPagesArray\n");
		return 0;
	}
	if ((*pageBest_DynamicWindow)->dynamicHashTable == NULL
		|| (*pageBest_DynamicWindow)->dynamicHeap == NULL
		|| (*pageBest_GlobalStructs)->startPagesHashTable == NULL) {
		return 0;
	}
	return 1;
};


int pageBest_dynamicUpdateWindow() {
	PageBest_GlobalStructs* pageBest_GlobalStructs = NULL;
	PageBest_DynamicWindow* pageBest_DynamicWindow = NULL;
	PageBest_Evaluation* pageBest_Evaluation = NULL;
	pageBest_initAllStructs(&pageBest_GlobalStructs, &pageBest_DynamicWindow,&pageBest_Evaluation);
	pageBest_preHanlePagesData(pageBest_GlobalStructs);
	for (int i = 0; i < pageBest_GlobalStructs->startPagesHashTable->size; i++) {
		pageBest_updateDynamicWindow(pageBest_DynamicWindow, pageBest_GlobalStructs, pageBest_Evaluation);
		hashForEach(pageBest_DynamicWindow->dynamicHashTable, pageBest_showDynamic, NULL);
	}
	pageBest_showEvaluation(pageBest_Evaluation);
	pageBest_freeAllStructs(pageBest_GlobalStructs, pageBest_DynamicWindow, pageBest_Evaluation);
};

int pageBest_freeAllStructs(PageBest_GlobalStructs* pageBest_GlobalStructs
	, PageBest_DynamicWindow* pageBest_DynamicWindow,PageBest_Evaluation* pageBest_Evaluation) {
	freePageHeap(pageBest_DynamicWindow->dynamicHeap);
	freeHash(pageBest_DynamicWindow->dynamicHashTable);
	freeHash(pageBest_GlobalStructs->startPagesHashTable);
	free(pageBest_GlobalStructs->dynamicPagesArray);
	free(pageBest_Evaluation);
	free(pageBest_GlobalStructs);
	free(pageBest_DynamicWindow);
	return 1;
};