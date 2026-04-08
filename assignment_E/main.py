import math
import random
import time

def norm_cdf(x):
    return 0.5 * (1.0 + math.erf(x / math.sqrt(2.0)))

def black_scholes_call(S0, K, r, sigma, T):
    d1 = (math.log(S0 / K) + (r + sigma * sigma / 2.0) * T) / (sigma * math.sqrt(T))
    d2 = d1 - sigma * math.sqrt(T)
    price = S0 * norm_cdf(d1) - K * math.exp(-r * T) * norm_cdf(d2)
    return price

def generate_random_option():
    S0 = 80.0 + random.random() * 40.0
    K = 80.0 + random.random() * 40.0
    r = 0.01 + random.random() * 0.09
    sigma = 0.1 + random.random() * 0.4
    T = 0.25 + random.random() * 1.75
    return S0, K, r, sigma, T

num_options = 1000000

print(f"Pricing {num_options} options using Black-Scholes...")

start = time.time()

for i in range(num_options):
    S0, K, r, sigma, T = generate_random_option()
    price = black_scholes_call(S0, K, r, sigma, T)

    # if i < 5:
    #     print(f"Option {i}: S0={S0:.2f} K={K:.2f} r={r:.4f} sigma={sigma:.4f} T={T:.4f} -> price={price:.4f}")

end = time.time()

print(f"Time to price {num_options} options: {end - start:.4f} seconds")
