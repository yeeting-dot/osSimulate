#ifndef TOOL_H
#define TOOL_H
#include <vector>
class Tool {
	private:
		template<typename T>
		static bool compare(std::vector<T>& data,int index,T target, bool isIncrease = true) {
			return ((isIncrease && data[index] >= target) || (!isIncrease && data[index] < target));
		}
	public:
		template<typename T>
		static int find(std::vector<T>& data,T target,bool isIncrease=true);
};

//返回第一个 >= target的元素
template<typename T>
int Tool::find(std::vector<T>& data, T target, bool isIncrease) {
	int start = -1;
	int end = data.size();
	while (start + 1 != end) {
		int mid = (start + end) / 2;
		if (compare(data, mid, target, isIncrease))end = mid;
		else start = mid;
	}
	if ((isIncrease && start == data.size() - 1) || (!isIncrease && end == 0))return -1;
	if (isIncrease)return end;
	else return start;
}
#endif