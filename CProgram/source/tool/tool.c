#include "tool.h"
#include "round.h"
#include <stdio.h>

//归并排序单层List排序
RoundTable* sortLevel(RoundTable* start,RoundTable* end) {
	RoundTable dummy; RoundTable* tail = &dummy;
	while (start && end) {
		if (start->roundSingle.enterTime < end->roundSingle.enterTime) {
			tail->next = start;
			start = start->next;
		}
		else {
			tail->next = end;
			end = end->next;
		}
		tail = tail->next;
	}
	tail->next = (start != NULL) ? start : end;
	return (&dummy)->next;
}

//使用快慢指针找到中间节点
RoundTable* findTheMid(RoundTable* start) {
	if (start == NULL) return start;
	RoundTable* slow = start;
	RoundTable* fast = slow->next;
	while (fast && fast->next) {
		slow = slow->next;
		fast = fast->next->next;
	}
	return slow;
}

RoundTable* sort(RoundTable* start, RoundTable* end) {
	if (start == end) {
		return start;
	}
	RoundTable* mid = findTheMid(start);
	RoundTable* midRight = mid->next;
	mid->next = NULL;
	RoundTable* leftValue = sort(start, mid);
	RoundTable* rightValue = sort(midRight, end);
	return sortLevel(leftValue, rightValue);
}

