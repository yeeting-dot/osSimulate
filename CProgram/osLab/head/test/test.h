#ifndef TEST_H
#define TEST_H
namespace osLab {
	namespace test {
		class Test {
		public:
			static int FileOperator();
			static int startFiFo();
			static int startClockInstead();
			static int testTool();
			static int testStructer();
			static int testSchedule();
			static int testSchedule_Seize();
			static int testSchedule_Priority();
		};
	}
}

#endif