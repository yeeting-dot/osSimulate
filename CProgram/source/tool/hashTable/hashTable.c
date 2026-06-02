#include "hashTable.h"
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <stdarg.h>

//分配哈希表
HashTable* newHashTable(int size) {
	if (size <= 0 || size >= MAX_HASHBUKKETANDNODE) {
		printf("malloc size is error");
		return NULL;
	}
	HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
	if (hashTable == NULL) {
		printf("Hash表分配失败");
		return NULL;
	}
	HashNode** hashBucket = (HashNode**)malloc(size * sizeof(HashNode*));
	if (hashBucket == NULL) {
		printf("Hash桶分配失败");
		free(hashTable);
		return NULL;
	}
	for (int i = 0; i < size; i++) {
		hashBucket[i] = NULL;
	}
	hashTable->count = 0;
	hashTable->hashBucket = hashBucket;
	hashTable->size = size;
	return hashTable;
}

//判断是否为哈希表
int isHashTable(HashTable* hashTable) {
	if (hashTable == NULL) {
		printf("Hash表为空");
		return 0;
	}
	return 1;
}

//返回键所对应的一条hash链表
int foundHashIndex(HashTable* hashTable, int key) {
	return abs(key) % hashTable->size;
}

//插入 / 更新哈希
int insertHash(HashTable* hashTable, int key, int value) {
	if (!isHashTable(hashTable))return 0;
	int hashIndex = foundHashIndex(hashTable, key);
	HashNode* preNode = NULL;
	HashNode* currentNode = hashTable->hashBucket[hashIndex];
	while (currentNode != NULL) {
		if (key == currentNode->key) {
			currentNode->value = value;
			return 1;
		}
		preNode = currentNode;
		currentNode = currentNode->next;
	}
	currentNode = (HashNode*)malloc(sizeof(HashNode));
	if (currentNode == NULL) { printf("新分配节点失败"); return 0; }
	hashTable->count += 1;
	currentNode->key = key;
	currentNode->value = value;
	currentNode->next = NULL;
	if (preNode == NULL) { hashTable->hashBucket[hashIndex] = currentNode; return 1; }
	currentNode->next = hashTable->hashBucket[hashIndex];
	hashTable->hashBucket[hashIndex] = currentNode;
	return 1;
}

HashNode* getHash(HashTable* hashTable, int key) {
	if (!isHashTable(hashTable))return NULL;
	int hashIndex = foundHashIndex(hashTable, key);
	HashNode* currentNode = hashTable->hashBucket[hashIndex];
	while (currentNode != NULL) {
		if (key == currentNode->key) {
			return currentNode;
		}
		currentNode = currentNode->next;
	}
	return NULL;
}

//删除哈希节点
int deleteHash(HashTable* hashTable, int key) {
	if (!isHashTable(hashTable))return 0;
	int hashIndex = foundHashIndex(hashTable, key);
	HashNode* currentNode = hashTable->hashBucket[hashIndex];
	HashNode* prevNode = NULL;
	while (currentNode != NULL) {
		if (currentNode->key == key) {
			if (prevNode != NULL) {
				prevNode->next = currentNode->next;
			}
			else {
				hashTable->hashBucket[hashIndex] = currentNode->next;
			}
			currentNode->next = NULL;
			free(currentNode);
			hashTable->count -= 1;
			return 1;
		}
		prevNode = currentNode;
		currentNode = currentNode->next;
	}
	printf("哈希表无此键");
	return 0;
}

int freeHash(HashTable* hashTable) {
	if (!isHashTable(hashTable))return 1;
	for (int i = 0; i < hashTable->size; i++) {
		HashNode* hashNode = hashTable->hashBucket[i];
		if (hashNode == NULL)continue;
		while (hashNode != NULL) {
			HashNode* needDelete = hashNode;
			hashNode = needDelete->next;
			needDelete->next = NULL;
			free(needDelete);
		}
	}
	free(hashTable->hashBucket);
	free(hashTable);
	return 1;
}

int hashForEach(HashTable* hashTable, int (*func)(HashNode*,int,void*),void* anyType) {
	int results = 1;
	for (int i = 0; i < hashTable->size; i++) {
		HashNode* hashNode = hashTable->hashBucket[i];
		if (hashNode == NULL)continue;
		while (hashNode != NULL) {
			results = results && func(hashNode,hashTable->size,anyType);
			hashNode = hashNode->next;
		}
	}
	return results;
}
