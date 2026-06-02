#include "pageBestNew.h"
#include "maxHeap.h"
#include "hashTable.h"
#include <malloc.h>
#include <stdio.h>
const static int MAX_PBN = 10086;

int pbn_getReflectIndex(PBN_GlobalStructs* globalStructs, int key) {
	HashNode* manageIndex = getHash(globalStructs->numReflectManageIndex, key);
	if (manageIndex != NULL)return manageIndex->value;
	else return -1;
}

int pbn_init_Structs(PBN_DynamicStructs** dynamicStructs,
	PBN_GlobalStructs** globalStructs,
	PBN_Evaluation** evaluation) {
	int pages, dynamicWindow;
	printf("Please enter the pages number you want to input: ");
	scanf("%d", &pages);
	printf("Please enter the dynamic Window number yopu want to set: ");
	scanf("%d", &dynamicWindow);
	*dynamicStructs = (PBN_DynamicStructs*)malloc(sizeof(PBN_DynamicStructs));
	if (*dynamicStructs == NULL)return pbn_check_null_allocate("dynamicStructs allocate failure");
	*globalStructs = (PBN_GlobalStructs*)malloc(sizeof(PBN_GlobalStructs));
	if (*globalStructs == NULL)return pbn_check_null_allocate("globalStructs allocate failure");
	*evaluation = (PBN_Evaluation*)malloc(sizeof(PBN_Evaluation));
	if (*evaluation == NULL)return pbn_check_null_allocate("evaluation allocate failure");
	(*globalStructs)->inputArray = (int*)malloc(pages * sizeof(int));
	if ((*globalStructs)->inputArray == NULL)
		return pbn_check_null_allocate("inputArray allocate failure");
	(*globalStructs)->numManages = (PBN_pageNumManage*)malloc(pages * sizeof(PBN_pageNumManage));
	for (int i = 0; i < pages; i++) {
		(*globalStructs)->numManages[i].numListHead = (*globalStructs)->numManages[i].numListTail = NULL;
	}
	if ((*globalStructs)->numManages == NULL)
		return pbn_check_null_allocate("numManages allocate failure");
	(*globalStructs)->numReflectManageIndex = newHashTable(pages);
	(*globalStructs)->currentIndex = -1;
	(*globalStructs)->manageLength = 0;
	(*globalStructs)->totalsInput = pages;
	(*dynamicStructs)->dynamicHeap = newPageHeap(dynamicWindow);
	(*dynamicStructs)->dynamicHashTable = newHashTable(dynamicWindow);
	(*evaluation)->insteadCount = 0;
	(*evaluation)->lostPageCount = 0;
	return 1;
}

int pbn_check_null_allocate(const char* str) {
	printf("%s", str);
	return 0;
}

int pbn_prehandle(PBN_GlobalStructs* globalStructs) {
	printf("please enter the pages you want to insert: ");
	for (int i = 0; i < globalStructs->totalsInput; i++) {
		scanf("%d", &globalStructs->inputArray[i]);
		PBN_pageNumNode* position = (PBN_pageNumNode*)malloc(sizeof(PBN_pageNumNode));
		position->num = i;
		position->next = NULL;
		HashNode* hashNode = getHash(globalStructs->numReflectManageIndex, globalStructs->inputArray[i]);
		if (hashNode == NULL) {
			insertHash(globalStructs->numReflectManageIndex, globalStructs->inputArray[i],
				globalStructs->manageLength);
			globalStructs->numManages[globalStructs->manageLength].manageNum = globalStructs->inputArray[i];
			globalStructs->numManages[globalStructs->manageLength].numListHead =
			globalStructs->numManages[globalStructs->manageLength].numListTail = position;
			globalStructs->manageLength++;
		}
		else {
			int manageIndex = hashNode->value;
			globalStructs->numManages[manageIndex].numListTail->next = position;
			globalStructs->numManages[manageIndex].numListTail = position;
		}
	}
	return 1;
}

int pbn_update(PBN_DynamicStructs* dynamicStructs,
	PBN_GlobalStructs* globalStructs,
	PBN_Evaluation* evaluation) {
	for (int i = 0; i < globalStructs->totalsInput; i++) {
		int pageNum = globalStructs->inputArray[i]; 
		int manageIndex = pbn_getReflectIndex(globalStructs, pageNum);
		if (manageIndex == -1)return 0;
		//需要在当前循环释放并移位nextPosition
		PBN_pageNumNode* nextPosition = globalStructs->numManages[manageIndex].numListHead->next;
		int nextIndex = nextPosition == NULL ? (MAX_PBN) : nextPosition->num;
		//遇到的问题，当存在于动态窗口的时候，怎么更新Heap
		//当存在于动态窗口中时
		if (getHash(dynamicStructs->dynamicHashTable, pageNum)) { 
			int heapIndex = getHash(dynamicStructs->dynamicHashTable, pageNum)->value;
			dynamicStructs->dynamicHeap->pageCount[heapIndex].count = nextIndex;
			insertHash(dynamicStructs->dynamicHashTable, pageNum, up(dynamicStructs->dynamicHeap, heapIndex, 1));
		}
		//当当前动态窗口页面数小于最大页面数时
		else if (dynamicStructs->dynamicHeap->current < dynamicStructs->dynamicHeap->maxHeap) {
			PageCount tempPageCount;
			tempPageCount.pageNum = pageNum;
			tempPageCount.count = nextIndex;
			int heapIndex = pushPageHeap(dynamicStructs->dynamicHeap, &tempPageCount);
			insertHash(dynamicStructs->dynamicHashTable, pageNum,heapIndex);
			evaluation->lostPageCount += 1;
		}
		//当需要替换时
		else {
			PageCount tempPageCount;
			getTheFirstElement(dynamicStructs->dynamicHeap, &tempPageCount);
			deleteHash(dynamicStructs->dynamicHashTable, tempPageCount.pageNum);
			tempPageCount.pageNum = pageNum;
			tempPageCount.count = nextIndex;
			int heapIndex = pushPageHeap(dynamicStructs->dynamicHeap, &tempPageCount);
			insertHash(dynamicStructs->dynamicHashTable, pageNum, heapIndex);
			evaluation->lostPageCount += 1;
			evaluation->insteadCount += 1;
		}
		PBN_pageNumNode* tempNode = globalStructs->numManages[manageIndex].numListHead;
		globalStructs->numManages[manageIndex].numListHead = tempNode->next;
		free(tempNode);
		heapForEach(dynamicStructs->dynamicHeap, pbn_show_foreach, NULL);
		printf("\n");
	}
	return 1;
}

int pbn_show_foreach(PageCount* pageCount,int size,void* ctx) {
	static int count = 0;
	printf("count: %8d pageNum: %8d nextPosition: %8d\n", count++ % size,pageCount->pageNum,pageCount->count);
	return 1;
}

int pbn_show_last(PBN_Evaluation* evaluation,int totals) {
	printf("lost pages count: %8d instead count: %8d lost pages frequency: %8lf\n",
		evaluation->lostPageCount, evaluation->insteadCount, (double)evaluation->lostPageCount / totals);
	return 1;
}

int pbn_main() {
	PBN_DynamicStructs* pbnDynamicStructs = NULL;
	PBN_GlobalStructs* pbnGlobalStructs = NULL;
	PBN_Evaluation* pbnEvaluation = NULL;
	pbn_init_Structs(&pbnDynamicStructs, &pbnGlobalStructs, &pbnEvaluation);
	pbn_prehandle(pbnGlobalStructs);
	pbn_update(pbnDynamicStructs, pbnGlobalStructs, pbnEvaluation);
	pbn_show_last(pbnEvaluation, pbnGlobalStructs->totalsInput);
	pbn_free(pbnDynamicStructs, pbnGlobalStructs, pbnEvaluation);
	return 1;
}

int pbn_free(PBN_DynamicStructs* dynamicStructs,
	PBN_GlobalStructs* globalStructs,
	PBN_Evaluation* evaluation) {
	free(evaluation);
	free(globalStructs->inputArray);
	freeHash(globalStructs->numReflectManageIndex);
	PBN_pageNumManage* manage = globalStructs->numManages;
	int manageLength = globalStructs->manageLength;
	for (int i = 0; i < manageLength; i++) {
		PBN_pageNumNode* start = manage[i].numListHead;
		while (start != NULL) {
			PBN_pageNumNode* next = start->next;
			start->next = NULL;
			free(start);
			start = next;
		}
	}
	free(globalStructs->numManages);
	free(globalStructs);
	freeHash(dynamicStructs->dynamicHashTable);
	freePageHeap(dynamicStructs->dynamicHeap);
	free(dynamicStructs);
	return 1;
}
