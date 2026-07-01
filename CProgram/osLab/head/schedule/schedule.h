//后续可使用delctype和constexpr进行动态调整
#ifndef SCHEDULE_H
#define SCHEDULE_H
#include <queue>
#include <iostream>
#include <functional>
#include <stdexcept>
#include <exception>
#include <algorithm>
#include <iomanip>
#include <type_traits>
namespace osLab {
	namespace schedule {
		typedef struct PCB {
			int PID;
			int enterTime;
			int runTime;
			int LeftrunTime;
			int priority = 0;
		} PCB;
		typedef struct evaluate {
			int PID;
			int roundTime;
			float rounTimeWithValue;
			int waitTime;
		} evaluate;
		typedef enum MODE {
			SEIZE,
			NON_SEIZE
		} MODE;

		typedef struct GLB_evalute {
			float everageTRound = 0.0;
			float throughput = 0.0;
		} GLB_evaluate;

		auto WAIT_CMP = [](const PCB& a,const PCB& b) {
			return a.enterTime > b.enterTime;
		};

		template<typename T,typename defaultQueue = std::queue<PCB> >
		class Base_Schedule {
			protected:
				std::priority_queue<PCB,std::vector<PCB>,decltype(WAIT_CMP)> waitSchedule;
				std::vector<evaluate> evaluator;
				defaultQueue runtimeSchedule;
				auto getRuntimeScheduleFront() -> decltype(auto) {
					if constexpr (std::is_same_v<defaultQueue, std::queue<PCB>>) {
						return runtimeSchedule.front();
					}
					else if constexpr (std::is_same_v<defaultQueue,std::priority_queue<PCB,std::vector<PCB>,
							std::function<bool(const PCB& a,const PCB& b)>
						>> ) {
						return runtimeSchedule.top();
					}
					else {
						throw std::runtime_error("Error types");
					}
				};
				void init(int length);
				virtual bool checkForArrive();
				MODE mode;
				int length;
				int timeClip;
				int currentTime = 0;
				GLB_evalute ge;
				virtual void no_seizeHandle();
				virtual void seize_handle();
			public:
				Base_Schedule(int length,MODE mode = MODE::NON_SEIZE,int timeClip = 20);
				~Base_Schedule() {};
				static inline std::function<bool(const PCB& a, const PCB& b)> defaultCmp =
					[](const PCB& a, const PCB& b)->bool {
						return a.priority < b.priority;
					};
				virtual void Run();
				virtual void Show();
		};

		template<typename T,typename defaultQueue>
		void Base_Schedule<T,defaultQueue>::Show() {
			std::cout << std::setw(5) << "PID" << std::setw(10) << "RoundTime" << std::setw(10) << "RoundTimeWithValue"
				<< std::setw(10) << "WaitTime" << std::endl;
			std::for_each(evaluator.begin(), evaluator.end(), [](const evaluate& data) {
				std::cout << std::setw(5) << data.PID;
				std::cout << std::setw(10) << data.roundTime;
				std::cout << std::setw(10) << data.rounTimeWithValue;
				std::cout << std::setw(10) << data.waitTime;
				std::cout << std::endl;
			});
			std::cout << std::endl;
			std::cout << std::setw(10) << "Average RoundTime" << std::setw(10) << "Throughput" << std::setw(10);
			std::cout << std::setw(10) << ge.everageTRound << std::setw(10) << ge.throughput << std::endl;

		}

		template<typename T,typename defaultQueue>
		bool Base_Schedule<T,defaultQueue>::checkForArrive() {
			bool arrive = false;
			while (!waitSchedule.empty() && waitSchedule.top().enterTime <= currentTime) {
					runtimeSchedule.push(waitSchedule.top());
					waitSchedule.pop();
					if (arrive == false)arrive = true;
			}
			return arrive;
		}

		template<typename T,typename defaultQueue>
		void Base_Schedule<T,defaultQueue>::init(int length) {
			
			for (int i = 0; i < length; i++) {
				typename decltype(runtimeSchedule)::value_type tempPCB;
				if(std::is_same_v<defaultQueue,std::queue<PCB>>)
					std::cout << "Please enter the PCB" << i << ":(PID enterTime runTime) ";
				else
				{
					std::cout << "Please enter the PCB" << i << ":(PID enterTime runTime priority) ";
				}
				std::cin >> tempPCB.PID;
				std::cin >> tempPCB.enterTime;
				std::cin >> tempPCB.runTime;
				if (!std::is_same_v<defaultQueue, std::queue<PCB>>)std::cin >> tempPCB.priority;
				tempPCB.LeftrunTime = tempPCB.runTime;
				waitSchedule.push(tempPCB);
			}
		}

		template <typename T,typename defaultQueue>
		Base_Schedule<T,defaultQueue>::Base_Schedule(int length, MODE mode, int timeClip) :mode(mode),length(length) {
			if (mode == MODE::SEIZE) { this->timeClip = timeClip; }
			if constexpr (std::is_same_v<defaultQueue, std::priority_queue<PCB, std::vector<PCB>,
				std::function<bool(const PCB & a, const PCB & b)> >>) {
				runtimeSchedule = defaultQueue(defaultCmp);
			}
			waitSchedule = std::priority_queue<PCB, std::vector<PCB>, decltype(WAIT_CMP)>(WAIT_CMP);
			init(length);
		}

		template <typename T,typename defaultQueue>
		void Base_Schedule<T,defaultQueue>::Run() {
			while (!waitSchedule.empty() || !runtimeSchedule.empty()) {
				bool arrive = checkForArrive();
				if (!arrive && !waitSchedule.empty() && runtimeSchedule.empty()) {
					currentTime = waitSchedule.top().enterTime;
					checkForArrive();
				}
				if (mode == NON_SEIZE)
					no_seizeHandle();
				else
					seize_handle();
			}
			ge.throughput = static_cast<float>(length) / currentTime;
			std::for_each(evaluator.begin(), evaluator.end(), [this](const evaluate& ev) {
					(this->ge).everageTRound += ev.rounTimeWithValue;
				});
			ge.everageTRound /= length;
		}

		template<typename T,typename defaultQueue>
		void Base_Schedule<T,defaultQueue>::no_seizeHandle() {
			auto getTheFirst = getRuntimeScheduleFront();
			currentTime += getTheFirst.LeftrunTime;
			getTheFirst.LeftrunTime = 0;
			evaluator.push_back({getTheFirst.PID,currentTime - getTheFirst.enterTime,static_cast<float>(currentTime - getTheFirst.enterTime) / getTheFirst.runTime,
				currentTime - getTheFirst.enterTime - getTheFirst.runTime});
			runtimeSchedule.pop();
			return;
		}

		template<typename T,typename defaultQueue>
		void Base_Schedule<T,defaultQueue>::seize_handle() {
			auto getTheFirst = getRuntimeScheduleFront();
			bool less = (getTheFirst.LeftrunTime <= timeClip);
			if (less) { no_seizeHandle(); return; }
			getTheFirst.LeftrunTime -= timeClip;
			runtimeSchedule.pop();
			runtimeSchedule.push(getTheFirst);
			currentTime += timeClip;
			return;
		}

	}
}

#endif