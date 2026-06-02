#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "round.h"
#include "tool.h"

//对进程表归并排序，确保先来的先被push到就绪队列中


//初始化并返回RoundManage
RoundManage* newRoundManage() {
	RoundManage* roundManage = malloc(sizeof(RoundManage));
	roundManage->roundListHead = roundManage->roundListTail = NULL;
	roundManage->roundTableHead = roundManage->roundTableTail = NULL;
	int access_number = 0;
	printf("Please Enter the timeClip: ");
	scanf("%d", &(roundManage->timeClip));

	printf("Please Enter thr Access Number you want to insert(access_name start_time run_time): ");
	scanf("%d", &(access_number));
	accessNum = access_number;
	for (int i = 0; i < access_number; i++) {
		RoundTable* p = malloc(sizeof(RoundTable));
		p->roundSingle.scheduleCode = (char*)malloc(accessNameSize * sizeof(char));
		scanf("%s", p->roundSingle.scheduleCode);
		scanf("%d", &(p->roundSingle.enterTime));
		scanf("%d", &(p->roundSingle.runTime));
		p->next = NULL;
		if (roundManage->roundTableTail == NULL && roundManage->roundListTail == NULL) {
			roundManage->roundTableHead = roundManage->roundTableTail = p;
		}
		else
		{
			roundManage->roundTableTail->next = p;
			roundManage->roundTableTail = p;
		}
	}
	roundManage->roundTableHead = sort(roundManage->roundTableHead, roundManage->roundTableTail);
	return roundManage;
}

RoundIndicatorSystem* newRoundIndicatorSystem() {
	RoundIndicatorSystem* roundIndicatorSystem = (RoundIndicatorSystem*)malloc(sizeof(RoundIndicatorSystem));
	roundIndicatorSystem->roundIndicator = (RoundIndicator*)malloc(accessNum * sizeof(RoundIndicator));
	roundIndicatorSystem->accessNumber = 0;
	roundIndicatorSystem->sumIndicator = 0.0;
	return roundIndicatorSystem;
}


void pushToBack(RoundManage* roundManage) {
	//从进程表中拿出头节点
	if (roundManage == NULL || roundManage->roundTableHead == NULL) {
		return;
	}
	RoundList* p = (RoundList*)malloc(sizeof(RoundList));
	p->next = NULL;

	//这里需要深拷贝，不然删掉temp的时候会出现悬垂引用的情况
	p->roundNode.roundSingle.runTime = roundManage->roundTableHead->roundSingle.runTime;
	p->roundNode.roundSingle.enterTime = roundManage->roundTableHead->roundSingle.enterTime;
	p->roundNode.roundSingle.scheduleCode = (char*)malloc((strlen(roundManage->roundTableHead->roundSingle.scheduleCode) + 1) * sizeof(char));
	strcpy(p->roundNode.roundSingle.scheduleCode, roundManage->roundTableHead->roundSingle.scheduleCode);
	p->roundNode.currentTime = p->roundNode.roundSingle.runTime;
	RoundTable* temp = roundManage->roundTableHead;
	roundManage->roundTableHead = temp->next;
	temp->next = NULL;
	free(temp);

	if (roundManage->roundListHead == NULL && roundManage->roundListTail == NULL) {
		roundManage->roundListHead = roundManage->roundListTail = p;
		return;
	}
	//push头节点进就绪队列
	roundManage->roundListTail->next = p;
	roundManage->roundListTail = p;
	return;
}


void gotoBack(RoundManage* roundManage) {
	//包含都指向空或者都指向第一个节点，这个时候不需要变换顺序
	if (roundManage->roundListHead == roundManage->roundListTail) {
		return;
	}
	RoundList* temp = roundManage->roundListHead;
	roundManage->roundListHead = temp->next;
	temp->next = NULL;
	roundManage->roundListTail->next = temp;
	roundManage->roundListTail = temp;
}

//取得就绪队列的队首
RoundNode* getHead(RoundManage* roundManage) {
	return &(roundManage->roundListHead->roundNode);
}

//检查该时间点下是否有（多个）进程到达，有则放入就绪队列
void checkUpdate(int time, RoundManage* roundManage) {
	while(roundManage->roundTableHead && roundManage->roundTableHead->roundSingle.enterTime <= time) {
		pushToBack(roundManage);
	}
}

//进程完成，从队列移除，将队首给下一个进程
void finishHead(RoundManage* roundManage) {
	RoundList* temp = roundManage->roundListHead;
	roundManage->roundListHead = temp->next;
	temp->next = NULL;
	free(temp->roundNode.roundSingle.scheduleCode);
	free(temp);
	if (roundManage->roundListHead == NULL) {
		roundManage->roundListTail = NULL;
	}
}

void RoundIndicatorCompute(RoundIndicatorSystem* roundIndicatorSystem, const char* name, int runtime, int finishedTime, int startTime) {
	int num = roundIndicatorSystem->accessNumber;
	roundIndicatorSystem->roundIndicator[num].roundTime = finishedTime - startTime;
	roundIndicatorSystem->roundIndicator[num].roundTimeWithPower = (double)roundIndicatorSystem->roundIndicator[num].roundTime / runtime;
	roundIndicatorSystem->sumIndicator += roundIndicatorSystem->roundIndicator[num].roundTime;
	roundIndicatorSystem->roundIndicator[num].accessName = (char*)malloc( (strlen(name) + 1) * sizeof(char));
	strcpy(roundIndicatorSystem->roundIndicator[num].accessName, name);
	roundIndicatorSystem->accessNumber++;
}

void clean(RoundIndicatorSystem* roundIndicatorSystem, RoundManage* roundManage) {
	for (int i = 0; i < roundIndicatorSystem->accessNumber; i++) {
		free(roundIndicatorSystem->roundIndicator[i].accessName);
	}
	free(roundIndicatorSystem->roundIndicator);
	free(roundIndicatorSystem);
	RoundList* rdStart = roundManage->roundListHead;
	RoundTable* roundTable = roundManage->roundTableHead;
	while (rdStart) {
		free(rdStart->roundNode.roundSingle.scheduleCode);
		RoundList* temp = rdStart;
		rdStart = rdStart->next;
		free(temp);
	}
	while (roundTable) {
		free(roundTable->roundSingle.scheduleCode);
		RoundTable* temp = roundTable;
		roundTable = temp->next;
		free(temp);
	}
	free(roundManage);
}

int work(){
	RoundManage* roundManage = newRoundManage();
	RoundIndicatorSystem* roundIndicatorSystem = newRoundIndicatorSystem();
	int currentTime = 0;
	printf("当前时间点\t进程名称\t剩余服务时间\n");
	while (roundManage->roundListHead != NULL || roundManage->roundTableHead != NULL) {
		checkUpdate(currentTime, roundManage);
		RoundNode* runNode = getHead(roundManage);
		if (runNode == NULL) {
			currentTime = roundManage->roundTableHead->roundSingle.enterTime;
			checkUpdate(currentTime, roundManage);
			runNode = getHead(roundManage);
		}
		if (runNode == NULL) { printf("错误：依然为空"); return; }
		int runTime = runNode->currentTime;
		if (runTime <= roundManage->timeClip) {
			currentTime += runTime;
			RoundIndicatorCompute(roundIndicatorSystem,runNode->roundSingle.scheduleCode, runNode->roundSingle.runTime,currentTime,runNode->roundSingle.enterTime);
			printf("%-16d%-16s%-16s\n", currentTime, runNode->roundSingle.scheduleCode,"服务完成");
			//打印和其余操作均需要在此之间，因为这个函数会删就绪队列中的节点
			finishHead(roundManage);
			continue;
		}
		currentTime += roundManage->timeClip;
		runNode->currentTime -= roundManage->timeClip;
		printf("%-16d%-16s%-16d\n", currentTime, runNode->roundSingle.scheduleCode, runNode->currentTime);
		checkUpdate(currentTime, roundManage);

		gotoBack(roundManage);
	}
	//打印每个进程的周转时间和整个系统的平均周转时间
	printf("进程名称\t周转时间\t带权周转时间\n");
	for (int i = 0; i < roundIndicatorSystem->accessNumber; i++) {
		printf("%-16s%-16d%-16lf\n", roundIndicatorSystem->roundIndicator[i].accessName,
			roundIndicatorSystem->roundIndicator[i].roundTime,
			roundIndicatorSystem->roundIndicator[i].roundTimeWithPower);
	}
	if (roundIndicatorSystem->accessNumber == 0)return;
	printf("平均周转时间: %lf\n", roundIndicatorSystem->sumIndicator / roundIndicatorSystem->accessNumber);
	return 1;
}

