#ifndef PAGEBEST_NEW_H
#define PAGEBEST_NEW_H
#include "hashTable.h"
#include "maxHeap.h"

typedef struct PBN_pageNumNode {
	int num;
	struct PBN_pageNumNode* next;
} PBN_pageNumNode;

typedef struct PBN_pageNumManage {
	int manageNum;
	PBN_pageNumNode* numListHead;
	PBN_pageNumNode* numListTail;
} PBN_pageNumManage;


typedef struct PBN_GlobalStructs {
	int currentIndex;
	int totalsInput;
	int* inputArray;
	PBN_pageNumManage* numManages;
	int manageLength;
	HashTable* numReflectManageIndex;
} PBN_GlobalStructs;

typedef struct PBN_DynamicStructs {
	PageHeap* dynamicHeap;
	HashTable* dynamicHashTable;
} PBN_DynamicStructs;

typedef struct PBN_Evaluation {
	int lostPageCount;
	int insteadCount;
} PBN_Evaluation;

int pbn_check_null_allocate(const char* str);

int pbn_init_Structs(PBN_DynamicStructs** dynamicStructs,
					PBN_GlobalStructs** globalStructs,
					PBN_Evaluation** evaluation);

int pbn_prehandle(PBN_GlobalStructs* globalStructs);

int pbn_update(PBN_DynamicStructs* dynamicStructs,
				PBN_GlobalStructs* globalStructs,
				PBN_Evaluation* evaluation);
int pbn_main();
int pbn_show_foreach(PageCount* pageCount, int size, void* ctx);

int pbn_show_last(PBN_Evaluation* evaluation,int totals);
int pbn_free(PBN_DynamicStructs* dynamicStructs,
	PBN_GlobalStructs* globalStructs,
	PBN_Evaluation* evaluation);

int pbn_getReflectIndex(PBN_GlobalStructs* globalStructs,int key);

#endif