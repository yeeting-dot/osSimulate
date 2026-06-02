#ifndef SCHEDULING_H
#define SCHEDULING_H

const static int accessNameSize = 100;
static int accessNum;
typedef struct RoundSingle {
	int runTime;
	int enterTime;
	char* scheduleCode;
} RoundSingle;

typedef struct RoundNode {
	RoundSingle roundSingle;
	int currentTime;
} RoundNode;

typedef struct RoundTable {
	RoundSingle roundSingle;
	int enterTime;
	struct RoundTable* next;
} RoundTable;

typedef struct RoundList {
	RoundNode roundNode;
	struct RoundList* next;
} RoundList;

typedef struct RoundManage {
	int timeClip;
	struct rounList {
		RoundList* roundListHead;
		RoundList* roundListTail;
	};
	struct roundTable {
		RoundTable* roundTableHead;
		RoundTable* roundTableTail;
	};
} RoundManage;

typedef struct RoundIndicator {
	int roundTime;
	double roundTimeWithPower;
	char* accessName;
} RoundIndicator;

typedef struct RoundIndicatorSystem {
	RoundIndicator* roundIndicator;
	double sumIndicator;
	int accessNumber;
} RoundIndicatorSystem;

RoundIndicatorSystem* newRoundIndicatorSystem();
RoundManage* newRoundManage();
void pushToBack(RoundManage* roundManage);
void gotoBack(RoundManage* roundManage);
void RoundIndicatorCompute(RoundIndicatorSystem* roundIndicatorSystem,char* name,int runtime, int finishedTime, int startTime);
RoundNode* getHead(RoundManage* roundManage);
void checkUpdate(int time,RoundManage* roundManage);
void finishHead(RoundManage* roundManage);
int work();
void clean(RoundIndicatorSystem* roundIndicatorSystem, RoundManage* roundManage);




#endif // !SCHEDULING_H
