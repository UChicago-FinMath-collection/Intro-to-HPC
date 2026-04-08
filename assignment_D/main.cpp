#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <random>

using namespace std;

double monte_carlo_call(double S0, double K, double r, double sigma, double T, int num_sims) {
    default_random_engine gen(rand());
    normal_distribution<double> dist(0.0, 1.0);

    double sum_payoffs = 0.0;
    for (int i = 0; i < num_sims; i++) {
        double z = dist(gen);
        double ST = S0 * exp((r - sigma * sigma / 2.0) * T + sigma * z * sqrt(T));
        double payoff = ST - K;
        if (payoff < 0.0) payoff = 0.0;
        sum_payoffs += payoff;
    }

    double price = exp(-r * T) * (sum_payoffs / num_sims);
    return price;
}

void generate_random_option(double &S0, double &K, double &r, double &sigma, double &T) {
    S0 = 80.0 + (double)rand() / RAND_MAX * 40.0;
    K = 80.0 + (double)rand() / RAND_MAX * 40.0;
    r = 0.01 + (double)rand() / RAND_MAX * 0.09;
    sigma = 0.1 + (double)rand() / RAND_MAX * 0.4;
    T = 0.25 + (double)rand() / RAND_MAX * 1.75;
}

int main() {
    srand(time(0));

    int num_options = 1000000;
    int num_sims = 100; // MC per option

    cout << "Pricing " << num_options << " options with " << num_sims << " simulations each." << endl;

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < num_options; i++) {
        double S0, K, r, sigma, T;
        generate_random_option(S0, K, r, sigma, T);
        double price = monte_carlo_call(S0, K, r, sigma, T, num_sims);

        // tests
        // if (i < 5) {
        //     cout << "Option " << i << ": S0=" << S0 << " K=" << K
        //          << " r=" << r << " sigma=" << sigma << " T=" << T
        //          << " -> price=" << price << endl;
        // }
    }

    auto end = chrono::high_resolution_clock::now();
    double elapsed = chrono::duration<double>(end - start).count();

    cout << "Time to price " << num_options << " options: " << elapsed << " seconds" << endl;

    return 0;
}
