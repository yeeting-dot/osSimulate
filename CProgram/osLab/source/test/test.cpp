#include "test/test.h"
#include <iostream>
#include <fstream>
#include "disk/disk.h"
#include "instead/FIFOInstead.h"
#include "instead/ClockInstead.h"
#include "schedule/schedule.h"
#include "tool/tool.h"
#include "memory/memory.h"
#include <vector>
namespace osLab {
	namespace test {
		using osLab::test::Test;
		using std::vector;
		using std::cin;
		using std::cout;
		using std::endl;
		using std::ifstream;
		using std::ofstream;
		using std::string;
		using osLab::disk::DiskListManage;
		using osLab::schedule::Base_Schedule;
		using osLab::schedule::PCB;
		const static string outputFiles = "../../exChangeFile/groupOutputForCpp.txt";
		const static string inputFiles = "../../exChangeFile/groupInputForCpp.txt";
		

		int Test::FileOperator() {
			ifstream fin(inputFiles, std::ios::in);
			ofstream fout(outputFiles, std::ios::out);
			DiskListManage diskListManage;
			int n;
			int getTime;
			int temp;
			int getContent;
			if (!fin.is_open() || !fout
				.is_open()) {
				cout << "File can not open" << endl;
				exit(EXIT_FAILURE);
			}
			fin >> n;
			for (int i = 0; i < n; i++) {
				fin >> temp;
				diskListManage.push(temp);
			}
			diskListManage.show(fout);
			fin >> getTime;
			for (int i = 0; i < getTime; i++) {
				diskListManage.pop(getContent);
			}
			fout << "this is After get: " << endl;
			diskListManage.show(fout);
			fin.close();
			fout.close();
			return 1;
		}

		int Test::startFiFo() {
			osLab::instead::FifoInstead<int> fifo(12, 5);
			fifo.handle(std::cin, true);
			fifo.printReview();
			return 1;
		}

		int Test::startClockInstead() {
			osLab::instead::ClockInstead<int> clock(17, 4);
			clock.handle(std::cin);
			clock.print(std::cout);
			return 1;
		}

		int Test::testTool() {
			std::vector<int> test{ -4,-3,-3,-3,-3 };
			int k = Tool::find(test, 3, true);
			std::cout << "index: " << k << std::endl;
			return 1;
		}

		int Test::testStructer() {
			int fixedSize = 300;
			osLab::memory::successive::Physical_Memory memory(1024, osLab::memory::successive::ALLOCTAE_FIXED, &fixedSize);
			memory.first_batch(120);
			memory.first_batch(300);
			memory.first_batch(300);
			memory.first_batch(300);
			return 1;
		}

		int Test::testSchedule() {
			Base_Schedule<PCB> baseSchedule(3);
			baseSchedule.Run();
			baseSchedule.Show();
			return 1;
		}

		int Test::testSchedule_Seize() {
			Base_Schedule<PCB> timeSchedule(3, osLab::schedule::SEIZE, 3);
			timeSchedule.Run();
			timeSchedule.Show();
			return 1;
		}

		int Test::testSchedule_Priority() {
			Base_Schedule<PCB,std::priority_queue<PCB, std::vector<PCB>,
				std::function<bool(const PCB& a, const PCB& b)> >> prioritySchedule(2, osLab::schedule::SEIZE, 3);
			prioritySchedule.Run();
			prioritySchedule.Show();
			return 1;
		}
	
	}
}

