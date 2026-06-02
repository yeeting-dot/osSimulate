#include "groupConnect.h"
#include <stdio.h>
#include <stdlib.h>
//Please Enter the Number you want insert:
//Please Enter the Content you want insert:
int gc_write() {
	FILE* outPutFile = fopen(outputFileName, "w");
	if (outPutFile == NULL) {
		printf("can not open the output file");
		return 0;
	}
	int insertTotal = 0;
	printf("Please Enter the Number you want insert: ");
	scanf("%d", &insertTotal);
	fprintf(outPutFile, "%d\n", insertTotal);
	int* insertContent = (int*)malloc(insertTotal * sizeof(int));;
	printf("Please Enter the Content you want insert (%d) : ",insertTotal);
	for(int n = 0; n < insertTotal; n++){
		scanf("%d", &insertContent[n]);
		if (n < insertTotal - 1)fprintf(outPutFile,"%d ", insertContent[n]);
		else fprintf(outPutFile,"%d\n", insertContent[n]);
	}
	int getNumber;
	printf("Please Enter the number you want to get: ");
	scanf("%d", &getNumber);
	fprintf(outPutFile,"%d", getNumber);
	fclose(outPutFile);
	free(insertContent);
	return 1;
}
int gc_read() {
	FILE* inputFile = fopen(inputFileName, "r");
	printf("\n");
	if (inputFile == NULL) {
		printf("can not open the input file");
		return 0;
	}
	char buffer[255];
	while (fgets(buffer, sizeof(buffer) - 1, inputFile)) {
		printf("%s", buffer);
	}
	fclose(inputFile);
	return 1;
}
int gc_execute() {
	int result = system("cd ..\\osLab\\target && .\\osLab.exe && cd ..\\..\\");
	return result;
}
int gc_main() {
	return gc_write() && gc_execute() && gc_read();
}