#include "test/test.h"
using osLab::test::Test;
int main() {
	//return Test::FileOperator();
	//return Test::startFiFo();
	//return Test::startClockInstead();
	//return Test::testTool();
	//return Test::testSchedule();
	//return Test::testSchedule_Seize();
	return Test::testSchedule_Priority();
}

