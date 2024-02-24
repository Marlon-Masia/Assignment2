#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>

#define N 40

std::mutex mutex;
std::atomic<int> guestsEnteredLabyrinth = 0;
std::atomic<bool> allGuestsVisited = false;

void enterLabyrinth(int guestId)
{
    mutex.lock();
    guestsEnteredLabyrinth++;
    std::cout << "Guest " << guestId + 1 << " has entered the labyrinth." << std::endl;
    if (guestsEnteredLabyrinth == N)
    {
        allGuestsVisited = true;
        std::cout << "All guests have visited the labyrinth." << std::endl;
    }

    mutex.unlock();
}

void eatCupcake(int guestId)
{

    mutex.lock();
    if (allGuestsVisited)
    {
        std::cout << "Guest " << guestId + 1 << " is eating the cupcake." << std::endl;
    }
    else
    {
        std::cout << "Guest " << guestId + 1 << " sees the cupcake but decides not to eat it." << std::endl;
    }
    mutex.unlock();
}

int main()
{
    auto start_time = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> guests;
    for (int i = 0; i < N; ++i)
    {
        guests.emplace_back(enterLabyrinth, i);
    }

    for (int i = 0; i < N; ++i)
    {
        guests.emplace_back(eatCupcake, i);
    }

    for (auto &guest : guests)
    {
        guest.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();

    auto execution_time = end_time - start_time;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(execution_time);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(execution_time - seconds);

    std::cout << "It took " << seconds.count() << "." << milliseconds.count() << " seconds for the program to execute" << std::endl;

    return 0;
}