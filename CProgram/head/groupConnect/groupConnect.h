#ifndef GROUP_CONNECT_H
#define GROUP_CONNECT_H
/*
* 因为在C++实现了这样的算法，所以此处就不额外实现了
* 而是输入到输入文件，让C++输出到输出文件
*/
const static char* outputFileName = "../exChangeFile/groupInputForCpp.txt";
const static char* inputFileName = "../exChangeFile/groupOutputForCpp.txt";
int gc_write();
int gc_read();
int gc_execute();
#endif
