#ifndef DISK_H
#define DISK_H
#include <vector>
#include <iostream>
namespace osLab{
    namespace disk{
        using std::vector;
        using std::ostream;
        class DiskListManage;
        class DiskList{
            public:
                struct NextDisk{
                    int DiskNumber;
                    DiskList* next;

                };
                friend class DiskListManage;
                const static int MAXLIST = 10;
            private:
                int length;
                NextDisk nextDisk;
                vector<int> diskNode;
                void push(int diskNumber);
                void pop(int& recieve);
                DiskList(NextDisk nextDisk = { -1,nullptr }) {
                    this->length = 1;
                    this->diskNode = vector<int>(MAXLIST - 1);
                    this->nextDisk = nextDisk;
                }
        };
        class DiskListManage {
            private:
                DiskList* currentDiskList;
            public:
                DiskListManage& push(int diskNumber);
                DiskListManage& pop(int& diskNumber);
                DiskListManage() { currentDiskList = new DiskList(); }
                ~DiskListManage();
                void show(ostream& out) const;
                friend ostream& operator<<(ostream& out, const DiskListManage& diskListManage);
        };
    }
}

#endif // DISK_H
