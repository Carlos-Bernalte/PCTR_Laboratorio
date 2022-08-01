#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#define LIMIT 100

long normal_sum = 0;
long sum1 = 0;
long sum2 = 0;
std::mutex mutex;
std::vector<int> numbers;

void protected_sum(int begin, int end)
{
    for (int i = begin; i <= end; i++)
    {
        std::lock_guard<std::mutex> guard(mutex);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        sum1 += numbers[i];
    }
}

void unprotected_sum(int begin, int end)
{
    for (int i = begin; i <= end; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        sum2 += numbers[i];
    }
}

int main(int argc, char *argv[])
{
    int nThreads = 10;

    std::vector<std::thread> vThreads;
    srand(time(NULL));

    int x = LIMIT / nThreads;
    int begin = 0;
    int end = x;

    for (int i = 0; i < LIMIT; i++)
    {
        numbers.push_back(rand());
        normal_sum += numbers[i];
    }

    for (int i = 0; i < nThreads; i++)
    {
        begin = i * x;
        end = (begin + x) - 1;
        vThreads.push_back(std::thread(protected_sum, begin, end));
        vThreads.push_back(std::thread(unprotected_sum, begin, end));
    }

    std::for_each(vThreads.begin(), vThreads.end(), std::mem_fn(&std::thread::join));

    std::cout << "Final sum without threads: " << normal_sum << std::endl;
    std::cout << "Final sum with protected threads: " << sum1 << std::endl;
    std::cout << "Final sum without protected threads: " << sum2 << std::endl;
    return EXIT_SUCCESS;
}