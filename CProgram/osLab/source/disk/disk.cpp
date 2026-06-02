#include "disk/disk.h"
#include <iostream>
#include <iomanip>
using std::ostream;
using std::cout;
using std::endl;
using std::setw;
using osLab::disk::DiskList;
using osLab::disk::DiskListManage;

void DiskList::DiskList::push(int diskNumber){
	diskNode[(length++)-1] = diskNumber;
}

void DiskList::DiskList::pop(int& diskNumber) {
	 diskNumber = diskNode[(--length) - 1];
}

DiskListManage& DiskListManage::push(int diskNumber) {
	if (this->currentDiskList->length == DiskList::MAXLIST) {
		currentDiskList = new DiskList({ diskNumber,currentDiskList });
		return *this;
	}
	currentDiskList->push(diskNumber);
	return *this;
}

DiskListManage& DiskListManage::pop(int& diskNumber) {
	if (currentDiskList->length == 1) {
		DiskList* temp = this->currentDiskList->nextDisk.next;
		diskNumber = this->currentDiskList->nextDisk.DiskNumber;
		this->currentDiskList->nextDisk.next = nullptr;
		delete this->currentDiskList;
		this->currentDiskList = temp;
		return *this;
	}
	currentDiskList->pop(diskNumber);
	return *this;
}

void DiskListManage::show(ostream& out) const {
	DiskList* cur = currentDiskList;
	while (cur) {
		if (cur->nextDisk.DiskNumber != -1) {
			out << setw(3) << cur->nextDisk.DiskNumber;
			out << setw(20) << (void*)&(cur->nextDisk.next) << endl;
		}
		for (int num = 0; num < (cur->length) - 1; num++) {
			out << setw(3) << cur->diskNode[num];
			out << setw(20) << (void*)&(cur->diskNode[num]) << endl;
		}
		out << endl;
		cur = cur->nextDisk.next;
	}
}
DiskListManage::~DiskListManage() {
	DiskList* cur = currentDiskList;
	while (cur) {
		DiskList* temp = cur;
		cur = temp->nextDisk.next;
		temp->nextDisk.next = nullptr;
		delete temp;
	}
}
ostream& osLab::disk::operator<<(ostream& out, const DiskListManage& diskListManage) {
	diskListManage.show(out);
	return out;
}