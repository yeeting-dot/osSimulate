#include <iostream>
#include <fstream>
#include "disk/disk.h"
#include "instead/FIFOInstead.h"
#include <vector>
using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using osLab::disk::DiskListManage;
const static string outputFiles = "../../exChangeFile/groupOutputForCpp.txt";
const static string inputFiles = "../../exChangeFile/groupInputForCpp.txt";

int FileOperator() {
	ifstream fin(inputFiles,std::ios::in);
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

int startFiFo() {
	osLab::instead::FifoInstead<int> fifo(12, 5);
	fifo.handle(std::cin,true);
	fifo.printReview();
	return 1;
}
int main() {
	//return FileOperator();
	return startFiFo();
}

