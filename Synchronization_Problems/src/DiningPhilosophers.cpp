#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include "SemCounter.cpp"

#define N 5

SemCounter counter(N);
std::mutex sticks[N];

void philosopher(int i)
{
    while (1)
    {
        //thinking
        counter.wait();
        std::cout << "Philosopher [" << i << "] is thinking " << std::endl;
        sticks[i].lock();
        sticks[(i + 1) % N].lock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //Add some complexity to see results

        //eating
        std::cout << "Philosopher [" << i << "] is eating " << std::endl;
        sticks[i].unlock();
        sticks[(i + 1) % N].unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000)); //Add some complexity to see results
        counter.signal();
    }
}

int main(int argc, char *argv[])
{
    std::vector<std::thread> philosophers;
    for (int i = 0; i < 10; i++)
    {
        philosophers.push_back(std::thread(philosopher, i));
    }
    std::for_each(philosophers.begin(), philosophers.end(), std::mem_fn(&std::thread::join));

    return EXIT_SUCCESS;
}