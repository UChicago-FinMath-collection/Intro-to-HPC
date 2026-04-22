# Lecture Summaries — FINM HPC (for Claude reference)

The course covers three pillars of parallel programming for finance: **vectorization (SIMD)**, **multicore (threads/OpenMP/TBB)**, and **GPGPU (CUDA)**, applied to financial workloads (Black-Scholes, Monte Carlo). Linux/Midway content is excluded.

---

## L1 — Single-Core Optimization & Vectorization (113 pages)

**Theme:** Profile-guided optimization of a single-core program, plus modern C++ features that enable performance.

Key topics:
- **Profile-guided optimization** — 80/20 rule, hotspots, VTune (User-Mode vs HW Event sampling). Reduce *Elapsed Time*, not just CPU Time.
- **Vectorization (SIMD)** — SSE 128-bit, AVX2 256-bit, AVX-512 512-bit. Compiler does it automatically with `-O2`+ in `icc`; reports via `-qopt-report`.
- **Vectorization obstacles** — loop-carried deps, function calls in loops, pointer aliasing, multiple exits, unknown bounds. Compiler may *multiversion* (vector + scalar runtime choice).
- **Vectorization pragmas** — `#pragma simd`, `#pragma ivdep`, `#pragma vector always`.
- **Loop optimizations** — loop-invariant code motion, loop unrolling.
- **Data types** — use `sqrtf`/`expf`/`logf`/`erff` with floats to avoid double promotion.
- **Containers** — contiguous (`vector`, array) > non-contiguous (`list`, `map`) for cache.
- **Modern C++ features** — `const&`, `std::span` (C++20), `std::string_view`, move semantics, `emplace_back`, `inline`, `constexpr`, `static_assert`. Avoid virtual functions in hot paths.
- **Running example:** Black-Scholes pricer for 10M options.

---

## L2 — Multicore Parallelism (91 pages)

**Theme:** Move from single-core to shared-memory parallelism using C++ threads, parallel patterns, OpenMP, and Intel TBB.

Key topics:
- **`std::thread`** — construct from callable, `join()` to wait, `detach()` to release. `<thread>` header.
- **Lambdas** — `[capture](params){body}`. Captures: `[=]`, `[&]`, `[=,&x]`, `[&,x]`.
- **Threaded matrix multiply** — granularity matters: per-row balanced, per-element catastrophic.
- **Race conditions & mutexes** — `std::mutex` with `lock()`/`unlock()`. Better: `std::lock_guard` (RAII).
- **Parallel patterns** — fork-join, map, stencil (map + neighbors), reduction, pipeline. Map-reduce is the bread and butter.
- **OpenMP** — `#pragma omp parallel`, `parallel for`, `sections`, `schedule(static|dynamic)`, sync (`barrier`, `critical`, `atomic`, `master`), `reduction(op:var)`.
- **Intel TBB** — `tbb::task_group` + `run()` / `wait()`. Scheduler handles load balancing via work-stealing.
- **Function objects (functors)** — class with `operator()`, carries state.

---

## L3 — Parallel RNG, Python Performance, CUDA Intro (99 pages)

**Theme:** Three areas: (1) Intel MKL RNG for parallel Monte Carlo, (2) Python performance via profiling + Cython + OpenMP, (3) GPU programming with CUDA C.

Key topics:
- **MKL RNG** — `VSLStreamStatePtr`, `vslNewStream`, `vsRngGaussian` (ICDF method), `vslDeleteStream`. Parallel: block splitting (`nskip`) and leap frogging (`nstreams`).
- **Python timing/profiling** — `time.time()`, `%timeit`, `python -m cProfile -s cumulative`, line profiler with `@profile` + `kernprof -l -v`.
- **Cython** — superset of Python compiled to C. `setup.py` with `cythonize`, build with `python setup.py build_ext --inplace`. `cdef` for C types/structs/enums. `def` (Python-callable) vs `cdef` (C-only). Static typing in hot loops gives major speedups.
- **Cython + OpenMP** — `prange` + `nogil` releases GIL for parallel loops. Need C types (no Python objects) inside `nogil`. Compile with `-fopenmp`, link `-lgomp`.
- **CUDA C** — `__global__` kernel, launch with `<<<blocks, threads>>>`. Qualifiers: `__global__`, `__device__`, `__host__`. Grid → blocks → threads (warp = 32). Built-ins: `threadIdx`, `blockIdx`, `blockDim`, `gridDim`.
- **CUDA indexing** — `int tid = threadIdx.x + blockIdx.x * blockDim.x;` Grid-stride loop for `N > total threads`.
- **CUDA memory** — `cudaMalloc`/`cudaFree`, `cudaMemcpy` with `cudaMemcpyHostToDevice` / `cudaMemcpyDeviceToHost`. Convention: `h_` host, `d_` device. `cudaDeviceSynchronize()` after async launches. Errors via `cudaError_t`.
- **Profiling** — `nvprof`, `nvvp`. Compile: `nvcc file.cu -o file`.

---

## L4 — GPU for Finance, Python GPU Tools (50 pages)

**Theme:** CUDA applied to Black-Scholes and Monte Carlo (with cuRAND), then Python GPU computing via CuPy and Numba. Course wrap-up.

Key topics:
- **CUDA Black-Scholes** — kernel signature with input arrays + output, per-thread d1/d2 calc, `__device__` helper for `cdf_normal`.
- **cuRAND** — `curandCreateGenerator(&gen, CURAND_RNG_PSEUDO_MTGP32)`, seed, `curandGenerateUniform/Normal`, `curandDestroyGenerator`. Link with `-lcurand`.
- **CuPy** — NumPy-like API on GPU. `cp.array`, `cp.zeros`, `cp.dot`, `@`, `cp.multiply`. Transfers: `cp.asarray()` (CPU→GPU), `cp.asnumpy()`/`.get()` (GPU→CPU). RNG: `cp.random.default_rng()`. Custom kernels: `ElementwiseKernel`, `ReductionKernel`, `preamble` for `__device__` helpers.
- **Numba** — JIT compiler for Python. `@cuda.jit` decorates kernel. `cuda.grid(1)` for global thread ID. Launch: `kernel.forall(N)(args)` (auto) or `kernel[blocks, threads](args)` (manual). Memory: `cuda.to_device`, `cuda.device_array_like`, `.copy_to_host()`.
- **Course wrap-up** — three pillars: vectorization, multicore, GPGPU. Workflow: measure → profile → experiment → repeat.

---

## Notes for cheat-sheet generation

- Every code example below is sourced from one of the lectures — do not invent.
- Group by *topic*, not by lecture, since topics often span lectures (e.g., Black-Scholes appears in L1, L4).
- Skip Linux shell, Midway cluster, build infrastructure beyond the bare compile commands.
- Cheat sheet is for a open-book exam; user can `cmd-f` it. Use clear, consistent headers.
