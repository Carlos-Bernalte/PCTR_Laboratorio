#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include "SemCounter.cpp"

#define N 5

SemCounter full(0);
SemCounter empty(N);
std::mutex access;
std::vector<int> buffer;
std::vector<std::thread> vThreads;

void productor(int i){
    while (1){
        empty.wait();
        access.lock();
        buffer.push_back(1);
        std::cout << "Producer ["<<i<<"]--Buffer Size: " << buffer.size()<<"---("<<full.getValue() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        access.unlock();
        full.signal();
    }
}

void consumidor(int i){
    while (1){
        full.wait();
        access.lock();
        buffer.pop_back();
        std::cout << "Consumer ["<<i<<"]--Buffer Size: " << buffer.size() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        access.unlock();
        empty.signal();
    }
}

int main(int argc, char *argv[]){
    for (int i = 0; i < 10; i++){
        if (i % 2 == 0){
            vThreads.push_back(std::thread(productor, i));
        }else{
            vThreads.push_back(std::thread(consumidor, i));
        }
    }

    std::for_each(vThreads.begin(), vThreads.end(), std::mem_fn(&std::thread::join));
    return EXIT_SUCCESS;
}