#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

double estimate_pi(int N) {
    int M = 0;
    for (int i = 0; i < N; i++) {
        double x = (double)rand() / RAND_MAX * 2.0 - 1.0; // uniform [-1, 1]
        double y = (double)rand() / RAND_MAX * 2.0 - 1.0;
        if (x * x + y * y <= 1.0) {
            M++;
        }
    }
    return 4.0 * M / N;
}

int main() {
    srand(time(0));

    int sizes[] = {100, 1000, 10000, 100000};

    for (int i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
        int N = sizes[i];
        double pi = estimate_pi(N);
        cout << "N = " << N << " -> pi estimate: " << pi << endl;
    }

    return 0;
}
