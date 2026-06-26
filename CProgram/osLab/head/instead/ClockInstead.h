#ifndef CLOCK_INSTEAD_H
#define CLOCK_INSTEAD_H
#include "FIFOInstead.h"
#include <vector>
#include <unordered_map>
#include <iostream>
namespace osLab {
	namespace instead {
		template<typename T>
		class ClockInstead {
			private:
				std::vector<T> DYM_content;
				std::vector<int> DYM_useBit;
				std::unordered_map<T, int> DYM_exist;
				osLab::instead::Review review;
				int foundBit_0();
				int total;
				int current;
				int foundStart;
			public:
				ClockInstead(int total, int window) :current(0),total(total), DYM_content(window), DYM_useBit(window,0),foundStart(0) {
					if (window <= 0 || window > 1000) { std::cerr << "Error windows Settings" << std::endl; std::exit(EXIT_FAILURE); }
					review.insteadOfTime = 0;
					review.lost_page_time = 0;
				}
				void handle(std::istream& cin);
				std::ostream& print(std::ostream& out);
		
		};
		
		template<typename T>
		int ClockInstead<T>::foundBit_0() {
			int start = foundStart;
			while (DYM_useBit[(start) % DYM_useBit.size()] != 0) {
				DYM_useBit[(start) % DYM_useBit.size()] = 0;
				start = (start + 1) % DYM_useBit.size();
			}
			foundStart = (start + 1) % DYM_useBit.size();
			return start;
		}
		template<typename T>
		void ClockInstead<T>::handle(std::istream& cin) {
			std::cout << "Please Enter the pages you want to insert: ";
			for (int i = 0; i < total; i++) {
				int temp;
				cin >> temp;
				if (DYM_exist.find(temp) != DYM_exist.end()) {
					DYM_useBit[DYM_exist[temp]] = 1; continue;
				}
				int nextPut = current >= DYM_content.size() ? foundBit_0():current;
				if (current > DYM_content.size() - 1) {
					DYM_exist.erase(DYM_content[nextPut]);
					review.insteadOfTime++;
				}
				current++;
				DYM_useBit[nextPut] ^= 1;
				DYM_content[nextPut] = temp;
				DYM_exist[temp] = nextPut;
				review.lost_page_time++;

			}
		}

		template<typename T>
		std::ostream& ClockInstead<T>::print(std::ostream& out) {
			std::cout << "lost pages times: " << review.lost_page_time << "\tinstead pages Time: " << review.insteadOfTime
				<< "\tlost_page_rate: " << (float)review.lost_page_time / total << std::endl;
			return out;
		}
		
	}
}
#endif