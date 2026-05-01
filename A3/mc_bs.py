import time
from math import erf, exp, log, sqrt

import cupy as cp


# param
S0     = 50.0
K      = 50.0
r      = 0.05
sigma  = 0.30
T      = 1.0
N      = 10_000_000
SEED   = 42
DTYPE  = cp.float32


# closed-form Black-Scholes (CPU, for verification)
def bs_closed_form(S0, K, r, sigma, T):
    d1 = (log(S0 / K) + (r + 0.5 * sigma ** 2) * T) / (sigma * sqrt(T))
    d2 = d1 - sigma * sqrt(T)
    Phi = lambda x: 0.5 * (1.0 + erf(x / sqrt(2.0)))
    call = S0 * Phi(d1) - K * exp(-r * T) * Phi(d2)
    put  = K * exp(-r * T) * Phi(-d2) - S0 * Phi(-d1)
    return call, put


# GPU MC
def mc_price_gpu(S0, K, r, sigma, T, N, seed=SEED, dtype=DTYPE):
    rng = cp.random.default_rng(seed)
    Z = rng.standard_normal(N, dtype=dtype)

    drift = (r - 0.5 * sigma ** 2) * T
    diff  = sigma * sqrt(T)
    ST = S0 * cp.exp(drift + diff * Z)

    disc = exp(-r * T)
    call_payoff = cp.maximum(ST - K, 0)
    put_payoff  = cp.maximum(K - ST, 0)

    call = disc * call_payoff.mean()
    put  = disc * put_payoff.mean()

    call_se = disc * call_payoff.std(ddof=1) / sqrt(N)
    put_se  = disc * put_payoff.std(ddof=1)  / sqrt(N)

    cp.cuda.Stream.null.synchronize()
    return float(call), float(put), float(call_se), float(put_se)


if __name__ == "__main__":
    dev = cp.cuda.Device(0)
    name = cp.cuda.runtime.getDeviceProperties(0)["name"].decode()
    print(f"CuPy {cp.__version__} | CUDA runtime {cp.cuda.runtime.runtimeGetVersion()} | device: {name}")
    print(f"Paths: {N:,} | dtype: {cp.dtype(DTYPE).name}")

    # warm up run
    _ = mc_price_gpu(S0, K, r, sigma, T, 1 << 14, seed=0)
    cp.cuda.Stream.null.synchronize()

    # timed run
    t0 = time.perf_counter()
    call, put, call_se, put_se = mc_price_gpu(S0, K, r, sigma, T, N)
    elapsed = time.perf_counter() - t0

    t1 = time.perf_counter()
    _ = mc_price_gpu(S0, K, r, sigma, T, N, seed=SEED + 1)
    elapsed_2 = time.perf_counter() - t1

    bs_call, bs_put = bs_closed_form(S0, K, r, sigma, T)

    print()
    print(f"{'':10s}{'MC':>14s}{'SE (1.96)':>14s}{'closed-form':>16s}{'error':>12s}")
    print(f"{'Call':10s}{call:>14.6f}{1.96*call_se:>14.6f}{bs_call:>16.6f}{call-bs_call:>12.2e}")
    print(f"{'Put':10s}{put:>14.6f}{1.96*put_se:>14.6f}{bs_put:>16.6f}{put-bs_put:>12.2e}")
    print()
    print(f"GPU time (1st timed run): {elapsed*1e3:.1f} ms")
    print(f"GPU time (2nd timed run): {elapsed_2*1e3:.1f} ms")
