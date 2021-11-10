#include <iostream>
#include "SharedObject.h"
#include "thread.h"
#include "Semaphore.h"

struct MyShared{
	int threadId;
	int reportId;
	time_t time;
};

int main(void)
{
	std::cout << "I am a reader" << std::endl;
	Shared<MyShared> sharedMemory ("sharedMemory");
    Semaphore* read_guard = new Semaphore("read_guard");
	Semaphore* write_guard = new Semaphore("write_guard");

	while(true){
        read_guard->Wait();
		std::cout << sharedMemory->threadId;
		std::cout << " ";
		std::cout << sharedMemory->reportId;
		std::cout << " ";
		std::cout << sharedMemory->time << std::endl;
        write_guard->Signal();
		sleep(1);
	}
	return 0;

	
}
