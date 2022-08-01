#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include "SemCounter.cpp"

#define N 5

SemCounter turn(N);
SemCounter write_access(1);
std::mutex mutex;
std::vector<int> buffer;
std::vector<std::thread> vHilos;

void writer(int i){
    while (1){
        turn.wait();
        std::cout << "Writer [" << i << "] waiting...\n";
        write_access.wait();
        std::cout << "Writer [" << i << "] is editing the file.\n";
        std::this_thread::sleep_for(std::chrono::seconds(4));
        turn.signal();
        std::cout << "Writer [" << i << "] finished editing the file.\n";
        write_access.signal();
    }
}

void reader(int i){
    while (1){
        turn.wait();
        turn.signal();
        write_access.wait();
        std::cout << "Reader [" << i << "] is reading the file.\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
        write_access.signal();
    }
}

int main(int argc, char *argv[]){
    for (int i = 0; i < 4; i++){
        vHilos.push_back(std::thread(reader, i));
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    for (int i = 0; i < 1; i++){
        vHilos.push_back(std::thread(writer, i));
    }
    std::for_each(vHilos.begin(), vHilos.end(), std::mem_fn(&std::thread::join));

    return EXIT_SUCCESS;
}