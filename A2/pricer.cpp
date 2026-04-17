#include "pricer.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <thread>
#include <vector>




struct OptionResult {
    int K;
    double sigma;        // implied vol
    double call_price;
    double put_price;
    double call_delta;
    double put_delta;
    double gamma;        // same for call and put
    double vega;         // same for call and put
    double call_theta;
    double put_theta;
    double call_rho;
    double put_rho;
};

// void price_range(int k_start, int k_end, double S0, double r, double T, OptionResult* results);

using namespace std;

double norm_cdf(double x) {
    return 0.5 * (1.0 + erf(x / sqrt(2.0)));
}

double norm_pdf(double x) {
    return (1.0 / sqrt(2.0 * M_PI)) * exp(-0.5 * x * x);
}

double vol_function(double K, double S0) {
    double m = log((double)K / S0);
    double sigma = 0.45 - 0.15 * m;
    if (sigma < 0.05) sigma = 0.05;
    return sigma;
}

void price_range(int k_start, int k_end, double S0, double r, double T, OptionResult* results) {

    for (int K = k_start; K <= k_end; K++) {
        double sigma = vol_function(K, S0);

        double sqrtT = sqrt(T);
        double d1 = (log(S0 / K) + (r + sigma * sigma / 2.0) * T) / (sigma * sqrtT);
        double d2 = d1 - sigma * sqrtT;

        double Nd1 = norm_cdf(d1);
        double Nd2 = norm_cdf(d2);
        double Nmd1 = norm_cdf(-d1);
        double Nmd2 = norm_cdf(-d2);
        double nd1 = norm_pdf(d1);

        double call = S0 * Nd1 - K * exp(-r * T) * Nd2;
        double put = K * exp(-r * T) * Nmd2 - S0 * Nmd1;

        double call_delta = Nd1;
        double put_delta = Nd1 - 1.0;
        double gamma = nd1 / (S0 * sigma * sqrtT);
        double vega = S0 * nd1 * sqrtT;
        double call_theta = -(S0 * nd1 * sigma) / (2.0 * sqrtT) - r * K * exp(-r * T) * Nd2;
        double put_theta = -(S0 * nd1 * sigma) / (2.0 * sqrtT) + r * K * exp(-r * T) * Nmd2;
        double call_rho = K * T * exp(-r * T) * Nd2;
        double put_rho = -K * T * exp(-r * T) * Nmd2;

        int idx = K - k_start;
        results[idx].K = K;
        results[idx].sigma = sigma;
        results[idx].call_price = call;
        results[idx].put_price = put;
        results[idx].call_delta = call_delta;
        results[idx].put_delta = put_delta;
        results[idx].gamma = gamma;
        results[idx].vega = vega;
        results[idx].call_theta = call_theta;
        results[idx].put_theta = put_theta;
        results[idx].call_rho = call_rho;
        results[idx].put_rho = put_rho;
    }

    cout << "thread done: strikes " << k_start << " to " << k_end << endl;
}

int run_pricer() {
    double S0 = 50.0;
    double r = 0.05;
    double T = 1.0;

    int k_min = 5;
    int k_max = 100;
    int total = k_max - k_min + 1; // 96 strikes

    int num_threads = 4;
    int chunk = total / num_threads; // 24

    // one big results array
    OptionResult* results = new OptionResult[total];

    vector<thread> threads(num_threads);

    for (int i = 0; i < num_threads; i++) {
        int start = k_min + i * chunk;
        int end;
        if (i == num_threads - 1) {
            end = k_max; // last thread takes the rest
        } else {
            end = start + chunk - 1;
        }
        // each thread writes into its slice of results - SEPARATION principle
        threads[i] = thread(price_range, start, end, S0, r, T, results + (start - k_min));
    }

    for (int i = 0; i < num_threads; i++) {
        threads[i].join();
    }

    // print the full option chain
    cout << fixed << setprecision(4);
    cout << "\n=== Option Chain ===" << endl;
    cout << "  K    sigma      Call       Put     C_Delta   P_Delta    Gamma     Vega    C_Theta   P_Theta    C_Rho     P_Rho" << endl;

    for (int i = 0; i < total; i++) {
        OptionResult r = results[i];
        cout << setw(4) << r.K << "  "
             << setw(7) << r.sigma << "  "
             << setw(9) << r.call_price << "  "
             << setw(9) << r.put_price << "  "
             << setw(8) << r.call_delta << "  "
             << setw(8) << r.put_delta << "  "
             << setw(8) << r.gamma << "  "
             << setw(8) << r.vega << "  "
             << setw(8) << r.call_theta << "  "
             << setw(8) << r.put_theta << "  "
             << setw(8) << r.call_rho << "  "
             << setw(8) << r.put_rho << endl;
    }

    delete[] results;
    return 0;
}
