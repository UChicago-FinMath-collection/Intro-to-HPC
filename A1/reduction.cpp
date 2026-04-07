
#include "patterns.h"
#include <iostream>
#include <thread>
#include <random>
#include <chrono>
#include <ctime>

using namespace std;

void thread_function(int* arr, int start, int end, int* results) {

    int sum = 0;

    for (int i = start; i < end; i++) {
        sum += arr[i];
    }

    std::cout << "thread done, sum:" << sum << endl;

    *results = sum;
}

int reduction_demo() {
    // seed for rand
    srand(::time(0));

    int arr[1000000];
    for (int i = 0; i < 1000000; i++) {
        arr[i] = rand() % 1000; // random between 0 and 1000
    }

    int results[4];

    std::vector<thread> threads(4);

    for (int i = 0; i < 4; i++) {
        threads[i] = thread(thread_function, arr, i * 250000, (i + 1) * 250000, &results[i]);
    }
    for (int i = 0; i < 4; i++) {
        threads[i].join();
    }

    //sum up results
    int total_sum = 0;
    for (int i = 0; i < 4; i++) {
        total_sum += results[i];
    }

    std::cout << "total sum: " << total_sum << endl;

    return 0;
}