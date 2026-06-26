#ifndef FIFO_INSTEAD_H
#define FIFO_INSTEAD_H
#include <unordered_map>
#include <iostream>
#include <vector>
#include <format>
#include <exception>
namespace osLab {
	namespace instead {
		typedef struct Review {
			int lost_page_time;
			int insteadOfTime;
		} Review;
		template <typename T>
		class FifoInstead {
			private:
				class DYM_info {
					public:
						int head, tail;
						std::vector<T> DYM_data;
						DYM_info(int len) :DYM_data(len), head(0), tail(0) {};
						bool emptyQueue() { return head == tail; }
						bool fullQueue() { return ((tail + 1) % DYM_data.size()) == head; }
				};
				std::vector<int> GLB_table;
				DYM_info dymInfo;
				Review review;
				std::unordered_map<T, int> maps;
				bool getHead(T& data);
				void moveQueue();
				void insertQueue(const T& data);
			public:
				void handle(std::istream& in,bool serverFirst=false);
				FifoInstead(int totalPage, int windowSize) :GLB_table(totalPage),dymInfo(windowSize) {
					if (windowSize <= 0 || windowSize > 1000) {
						throw std::exception("windows size to short");
					}
					dymInfo.head = dymInfo.tail = 0;
					review.lost_page_time = 0;
					review.insteadOfTime = 0;
				};
				void printReview() {
					/*std::cout << std::format("losePageTime:{}\tinsteadOfTime:{}\tinsteadRate:{.4f}", review.lost_page_time
						, review.insteadOfTime, static_cast<float>(review.lost_page_time) / dymInfo.DYM_data.size());*/
					std::cout << "lostPageTime: " << review.lost_page_time << "\t";
					std::cout << "insteadOfTime: " << review.insteadOfTime << "\t";
					std::cout << "lost_rate: " << static_cast<float>(review.lost_page_time) / this->GLB_table.size() << std::endl;
				}
		};

		template<typename T>
		void FifoInstead<T>::insertQueue(const T& data) {
			if (this->dymInfo.fullQueue())return;
			this->dymInfo.DYM_data[(++this->dymInfo.tail) % this->dymInfo.DYM_data.size()] = data;
			return;
		}

		template<typename T>
		bool FifoInstead<T>::getHead(T& data) {
			if (this->dymInfo.emptyQueue())return false;
			data = dymInfo.DYM_data[dymInfo.head];
			return true;
		}

		template<typename T>
		void FifoInstead<T>::moveQueue() {
			if (this->dymInfo.fullQueue()) {
				this->dymInfo.head = (this->dymInfo.head + 1) % (this->dymInfo.DYM_data.size());
				this->dymInfo.tail = (this->dymInfo.tail + 1) % (this->dymInfo.DYM_data.size());
				return;
			}
			this->dymInfo.tail = (this->dymInfo.tail + 1) % this->dymInfo.DYM_data.size();
			this->dymInfo.head = (this->dymInfo.head + 1) % this->dymInfo.DYM_data.size();
		}
		/*3 2 1 0 3 2 4 3 2 1 0 4*/
		template<typename T>
		void FifoInstead<T>::handle(std::istream& in,bool serverFirst) {
			std::cout << "Please Enter some pages: " << std::endl;
			for (int i = 0; i < GLB_table.size(); i++) {
				in >> GLB_table[i];
				if (maps.find(GLB_table[i]) != maps.end()) {
					//需要重新实现，找到对应位置，是时候让我们的哈希表的value发挥作用了，并把它放到tail，将head+1更新为原来的head，move一次
					//需要注意首部是需要替换的特殊情况
					if (serverFirst) {
						this->dymInfo.DYM_data[this->dymInfo.tail] = this->dymInfo.DYM_data[maps[GLB_table[i]]];
						for (int j = maps[GLB_table[i]]; j != this->dymInfo.tail; j = ((j + 1) % (this->dymInfo.DYM_data.size()))) {
							this->dymInfo.DYM_data[j] = this->dymInfo.DYM_data[(j + 1) % (this->dymInfo.DYM_data.size())];
							int oldKey = this->dymInfo.DYM_data[(j + 1) % (this->dymInfo.DYM_data.size())];
							maps[oldKey] = maps[oldKey] - 1 < 0 ? this->dymInfo.DYM_data.size() - 1 : (maps[oldKey] - 1) % (this->dymInfo.DYM_data.size());
						}
						//记得要更新命中索引
						maps[GLB_table[i]] = (this->dymInfo.tail - 1) % this->dymInfo.DYM_data.size();
					}
					continue;
				}
				
				//当没有找到的时候
				maps[GLB_table[i]] = this->dymInfo.tail;
				if (this->dymInfo.fullQueue()) {
					this->maps.erase(this->dymInfo.DYM_data[this->dymInfo.head]);
					this->dymInfo.DYM_data[(this->dymInfo.tail) % this->dymInfo.DYM_data.size()] = GLB_table[i];
					moveQueue();
					this->review.insteadOfTime += 1;
				}
				else {
					this->dymInfo.DYM_data[(this->dymInfo.tail) % this->dymInfo.DYM_data.size()] = GLB_table[i];
					this->dymInfo.tail = (this->dymInfo.tail + 1) % this->dymInfo.DYM_data.size();
				}
				this->review.lost_page_time += 1;
			}
			
			
		}
	}
}
#endif