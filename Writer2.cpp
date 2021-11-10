#include <iostream>
#include <queue>
#include <chrono>
#include <thread>
#include "SharedObject.h"
#include "thread.h"
#include "semaphore.h"

using namespace std;

struct MyShared{
	int threadId;
	int reportId;
	time_t time;
};

class WriterThread : public Thread{
	public:
		int 	threadNum;
		int 	reportId;
		bool	flag;
		int 	delay;
		time_t startTime;
		
		WriterThread(int in, int timeDelay):Thread(8*1000){
			this->threadNum = in; 
			this->delay = timeDelay;
			this->reportId = 1;
			this->flag = false;
			this->startTime = time(NULL);
		}

		virtual long ThreadMain(void) override{
			//declare shared memory var so this thread can access it
			Shared<MyShared> sharedMemory ("sharedMemory");
            Semaphore* read_guard = new Semaphore("read_guard");
	        Semaphore* write_guard = new Semaphore("write_guard");
			while(true)
			{	
                write_guard->Wait();
				sharedMemory->threadId = this->threadNum;
				sharedMemory->reportId = this->reportId;
				this->reportId += 1;

				time_t currentTime;
				time(&currentTime);
				sharedMemory->time = difftime(currentTime, startTime);
				this->startTime = currentTime;
                read_guard->Signal();

				if(flag){//Exit loop to end the thread
					break;
				}
				sleep(delay);
			}
			return 0;
		}
};


int main(void)
{
	std::cout << "I am a Writer" << std::endl;
	
	Shared<MyShared> shared("sharedMemory", true);
    Semaphore* write_guard = new Semaphore("write_guard", 1, true); 
	Semaphore* read_guard = new Semaphore("read_guard", 0, true); 

	int counter = 0;
	std::queue<WriterThread *> childThreads;
	
	while(true){
		string createNew;
		int timeDelay;
		cout << "Do you want to create another Writer thread?" << endl;
		cin >> createNew;
		if (createNew == "y"){
			cout << "What is the delay time for this thread?" << endl;
			cin >> timeDelay;
			//create thread using user input
			WriterThread * thread1 = new WriterThread(counter++, timeDelay); 
			childThreads.push(thread1);
		}
		if (createNew != "y"){
			break;
		}
		
	}
	while(!childThreads.empty()){
		WriterThread * t = childThreads.back();
		t -> flag = true;
		delete t;
		childThreads.pop();
	}
	
}