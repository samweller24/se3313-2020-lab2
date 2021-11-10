#include <iostream>
#include "SharedObject.h"
#include "thread.h"

struct MyShared{
	int threadId;
	int reportId;
	time_t time;
};

int main(void)
{
	std::cout << "I am a reader" << std::endl;
	Shared<MyShared> sharedMemory ("sharedMemory");
	while(true){
		std::cout << sharedMemory->threadId;
		std::cout << " ";
		std::cout << sharedMemory->reportId;
		std::cout << " ";
		std::cout << sharedMemory->time << std::endl;
		sleep(1);
	}
	return 0;

	
}
