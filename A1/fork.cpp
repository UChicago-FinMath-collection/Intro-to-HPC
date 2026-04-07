#include "patterns.h"
#include <iostream>
#include <thread>
#include <random>
#include <chrono>
#include <ctime>

using namespace std;

void thread_function() {
    
    int time = rand() % 5 + 1; // random time between 1 and 5 seconds

    this_thread::sleep_for(chrono::seconds(time));

    std::cout << time << "s thread done" << endl;
}

int fork_demo() {
    // seed for rand
    srand(::time(0));

    thread t1(thread_function);
    thread t2(thread_function);
    thread t3(thread_function);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}