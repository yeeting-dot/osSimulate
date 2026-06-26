#include "memory/memory.h"
#include "tool/tool.h"
#include <iostream>
#include <format>
#include <algorithm>
using osLab::memory::successive::Physical_Memory;
using osLab::memory::successive::pieceMes;

Physical_Memory::Physical_Memory(int memorysLength,  allocate_way alo, void* data) {
	if (alo == allocate_way::ALLOCTAE_FIXED) {
		int* dataParse = static_cast<int*>(data);
		if (*dataParse <= 0) { std::cerr << "Error input!!!" << std::endl; exit(EXIT_FAILURE); }
		auto fixNum = memorysLength / (*dataParse);
		auto extraNum = (memorysLength % (*dataParse) == 0) ? 0 : 1;
		auto totalNum = fixNum + extraNum;
		if (totalNum == 0) { return; }
		if (fixNum != 0) {
			allocMes = std::vector<pieceMes>(fixNum);
			for (int i = 0; i < fixNum; i++) {
				allocMes[i].pieceTag = i + 1;
				allocMes[i].Length = *dataParse;
				allocMes[i].allocateStatus = false;
				allocMes[i].useLength = 0;
			}
		}
		if (extraNum != 0)allocMes.push_back({ totalNum,memorysLength % (*dataParse),false });
		return;
	}
	if (alo == allocate_way::ALLOCTAE_MULTI) {
		std::vector<int>* dataParse = static_cast<std::vector<int>*>(data);
		int s = std::accumulate(dataParse->begin(), dataParse->end(), 0);
		if (s > memorysLength) { std::cerr << "ÎÞ·¨·ÖÅä" << std::endl; exit(EXIT_FAILURE); }
		allocMes = std::vector<pieceMes>(dataParse->size());
		for (int i = 0; i < dataParse->size(); i++) {
			if (dataParse->at(i) <= 0) { std::cerr << "Error settings!!!" << std::endl; break; }
			allocMes[i].pieceTag = i + 1;
			allocMes[i].Length = dataParse->at(i);
			allocMes[i].allocateStatus = false;
			allocMes[i].useLength = 0;
		}
		int extraSize = memorysLength - s;
		if (extraSize > 0)allocMes.push_back({ static_cast<int>(dataParse->size() + 1),extraSize,false });
	}
}

void Physical_Memory::first_batch(int needSpace) {
	for (int i = 0; i < allocMes.size(); i++) {
		if (allocMes[i].Length >= needSpace && !allocMes[i].allocateStatus) {
			allocMes[i].allocateStatus = true;
			allocMes[i].useLength = needSpace;
			std::cout << std::format("allocate pieceTag: {}\t allocate size: {}\t use size:{}\n",
				allocMes[i].pieceTag, allocMes[i].Length, allocMes[i].useLength);
			return;
		}
	}
	std::cerr << "can not to allocate space: " << needSpace << std::endl;
}

void Physical_Memory::best_batch(int needSpace) {
	static int best_IDX = 0;
	static std::vector<pieceMes> best(allocMes);
	if (best_IDX <= 0) {
		std::sort(best.begin(), best.end(), [](const pieceMes& a,const pieceMes& b )->bool {
			return a.Length < b.Length;
		});
	}
	best_IDX++;
	pieceMes temp;
	temp.Length = needSpace;
	int minEnough = Tool::find(best, temp);
	if (minEnough == -1) { std::cout << "No enough space to allocate" << std::endl; return; }
	for (auto start = best.begin() + minEnough; start != best.end();start++) {
		if (start->Length >= needSpace && !start->allocateStatus) {
			std::cout << "True allocate,The allocate Message: " << std::endl;
			std::cout << "pieceTag: " << start->pieceTag << std::endl;
			std::cout << "totalSize: " << start->Length << std::endl;
			std::cout << "useSuze" << (start->useLength += needSpace) << std::endl;
			start->allocateStatus = true;
			return;

		}
	}
	std::cout << "No enough space to allocate" << std::endl;
	return;
}

void Physical_Memory::worst_batch(int needSpace) {
	static int worse_IDX = 0;
	static std::vector<pieceMes> worst(allocMes);
	if (worse_IDX <= 0) {
		std::sort(worst.begin(), worst.end(), [](const pieceMes& a, const pieceMes& b)->bool {
			return a.Length > b.Length;
			});
	}
	worse_IDX++;
	pieceMes temp;
	temp.Length = needSpace;
	int minEnough = Tool::find(worst, temp,false);
	if (minEnough == -1) { std::cout << "No enough space to allocate" << std::endl; return; }
	for (auto start = worst.begin(); start != (worst.begin() + minEnough + 1); start++) {
		if (start->Length >= needSpace && !start->allocateStatus) {
			std::cout << "True allocate,The allocate Message: " << std::endl;
			std::cout << "pieceTag: " << start->pieceTag << std::endl;
			std::cout << "totalSize: " << start->Length << std::endl;
			std::cout << "useSuze" << (start->useLength += needSpace) << std::endl;
			start->allocateStatus = true;
			return;

		}
	}
	std::cout << "No enough space to allocate" << std::endl;
	return;
}

void Physical_Memory::for_batch(int needSpace) {
	static int lastForBatch = 0;
	int start = (lastForBatch + 1) % Physical_Memory::allocMes.size();
	while (start != lastForBatch){
		if (allocMes[start].Length >= needSpace && !allocMes[start].allocateStatus) {
			allocMes[start].allocateStatus = true;
			allocMes[start].useLength = needSpace;
			std::cout << std::format("allocate pieceTag: {}\t allocate size: {}\t use size:{}\n" ,
				allocMes[start].pieceTag, allocMes[start].Length, allocMes[start].useLength);
			lastForBatch = start;
			return;
		}
		start = (start + 1) % allocMes.size();
	};
	std::cerr << "can not to allocate space: " << needSpace << std::endl;
	return;
}

bool osLab::memory::successive::operator<(const pieceMes& a, const pieceMes& b) { return a.Length < b.Length; }
bool osLab::memory::successive::operator>=(const pieceMes& a, const pieceMes& b) { return !(a < b); }

