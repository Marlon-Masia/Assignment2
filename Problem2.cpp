#include <array>
#include <chrono>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_set>

#define N 10

enum Occupancy
{
    AVAILABLE,
    BUSY
};

std::unordered_set<std::thread::id> guestsVisited{};
std::mutex mutex;
Occupancy roomOccupancy = Occupancy::AVAILABLE;

void admireVase(int threadIndex)
{
    std::thread::id threadId = std::this_thread::get_id();

    while (guestsVisited.size() < N)
    {
        mutex.lock();

        if (roomOccupancy == Occupancy::AVAILABLE && guestsVisited.find(threadId) == guestsVisited.end())
        {
            roomOccupancy = Occupancy::BUSY;
            std::cout << "Guest " << threadIndex + 1 << " has looked at the vase" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            roomOccupancy = Occupancy::AVAILABLE;

            guestsVisited.insert(threadId);
        }

        mutex.unlock();
    }
}

int main()
{
    auto start_time = std::chrono::high_resolution_clock::now();
    std::array<std::thread, N> guests{};

    for (size_t i = 0; i < guests.size(); i++)
    {
        guests[i] = std::thread(admireVase, i);
    }

    for (auto &thread : guests)
    {
        thread.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();

    auto execution_time = end_time - start_time;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(execution_time);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(execution_time - seconds);

    std::cout << "All guests have viewed the vase." << std::endl;
    std::cout << "It took " << seconds.count() << "." << milliseconds.count() << " seconds for the program to execute" << std::endl;
}