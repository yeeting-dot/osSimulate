#ifndef PAGE_BEST_H
#define PAGE_BEST_H
#include "hashTable.h"
#include "maxHeap.h"

typedef struct PageBest_DynamicWindow {
	PageHeap* dynamicHeap;
	HashTable* dynamicHashTable;
} PageBest_DynamicWindow;

typedef struct PageBest_GlobalStructs {
	int* dynamicPagesArray;
	int dynamicPagesArrayIndex;
	HashTable* startPagesHashTable;
} PageBest_GlobalStructs;

typedef struct PageBest_Evaluation {
	int lostPageCount;
	int insteadCount;
	int totalPages;
} PageBest_Evaluation;

int pageBest_showEvaluation(PageBest_Evaluation* pageBest_Evaluation);

int pageBest_showDynamic(HashNode* hashNode, int size, void* ctx);

int pageBest_updateDynamicWindow(PageBest_DynamicWindow* pageBest_DynamicWindow
								,PageBest_GlobalStructs* pageBest_GlobalStructs
								,PageBest_Evaluation* pageBest_Evaluation);

int pageBest_preHanlePagesData(PageBest_GlobalStructs* pageBest_GlobalStructs);

int pageBest_initAllStructs(PageBest_GlobalStructs* pageBest_GlobalStructs
							,PageBest_DynamicWindow* pageBest_DynamicWindow
							,PageBest_Evaluation* pageBest_Evaluation);
int pageBest_dynamicUpdateWindow();

int pageBest_freeAllStructs(PageBest_GlobalStructs* pageBest_GlobalStructs
							, PageBest_DynamicWindow* pageBest_DynamicWindow
							, PageBest_Evaluation* pageBest_Evaluation);




#endif