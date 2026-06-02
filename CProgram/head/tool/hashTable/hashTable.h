#ifndef HASHTABLE_H
#define HASHTABLE_H
//加入static，避免重定义行为
const static int MAX_HASHBUKKETANDNODE = 10086;
typedef struct HashNode {
	int key;
	int value;
	struct HashNode* next;
}HashNode;

typedef struct HashTable {
	HashNode** hashBucket;
	int size;
	int count;
} HashTable;

HashTable* newHashTable(int size);
int insertHash(HashTable* hashTable,int key,int value);
HashNode* getHash(HashTable* hashTable, int key);
int deleteHash(HashTable* hashTable, int key);
int freeHash(HashTable* hashTable);
int isHashTable(HashTable* hashTable);
int foundHashIndex(HashTable* hashTable, int key);
int hashForEach(HashTable* hashTable, int (*func)(HashNode*,int,void*),void* anyType);

#endif