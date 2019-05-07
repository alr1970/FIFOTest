#include <thread>
#include <iostream>

#include "fifo.hpp"

TSFifo<int> sharedQueue(5);

void threadHandler()
{
    while(true) {
        std::pair<bool, int> popped = sharedQueue.pop_try();
        if(popped.first) {
            std::cout << "1 Popped " << popped.second << std::endl;
        }
    }
}
void threadHandler2()
{
    while(true) {
        std::pair<bool, int> popped = sharedQueue.pop_try();
        if(popped.first) {
            std::cout << "2 Popped " << popped.second << std::endl;
        }
    }
}


int main()
{
    //launch the threads
    std::thread runner(threadHandler);
    std::thread runner2(threadHandler2);
    for(int i = 0; i < 500; ) {
        if(!sharedQueue.push(i)) {
            std::cout<< "Queue full" << std::endl;
        }
        else {
            i++;
        }
    }
    runner.join();
    runner2.join();
}
