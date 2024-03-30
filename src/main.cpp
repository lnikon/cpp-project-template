#include <cmath>
#include <cstring>
#include <numeric>
#include <algorithm>
#include <execution>
#include <numeric>
#include <benchmark/benchmark.h>
#include <random>

namespace bm = benchmark;

namespace vagag {

  /**
   * @brief search for the first element e such that e >= x
   *
   * @param x first element not less then
   * @param a the vector to search in
   * @return 
   */
  int lower_bound(int x, std::vector<int> a) {
    int l = 0, r = a.size() - 1;
    while (l < r) {
      int t = (l + r) / 2;
      if (a[t] >= x) {
        r = t;
      } else {
        l = t + 1;
      }
    }

    return a[l];
  }

  int eytzinger(const std::vector<int>& input, int32_t* output, std::size_t i = 0, std::size_t k = 1) {
    const auto n = input.size();
    if (k <= n) {
      i = eytzinger(input, output, i, 2 * k);
      output[k] = input[i++];
      i = eytzinger(input, output, i, 2 * k + 1 );
    }
    return i;
  }

  int search(const int32_t* eytz, const size_t sz, const int x) {
    static constexpr const auto block_size{16}; // int's needed to cover a cache line
    std::size_t k = 1;
    while (k <= sz) {
      __builtin_prefetch(eytz + k * block_size);
      k = 2 * k + (eytz[k] < x);
    }
    k >>= __builtin_ffs(~k);
    return eytz[k];
  }
}

[[nodiscard]] std::vector<int> get_random_vector(const std::size_t size) noexcept
{
  std::vector<int> result(size);
  std::random_device r;
  std::uniform_int_distribution<int> uniform_dist(0, 100);
  std::mt19937 rng;
  std::generate_n(result.begin(), size, [&rng, &uniform_dist] { return uniform_dist(rng); });
  return result;
}

template <typename execution_policy_t>
static void std_lower_bound(bm::State& state, execution_policy_t&& policy) {
  auto count = static_cast<size_t>(state.range(0));
  std::vector<int32_t> array(count);
  std::iota(array.begin(), array.end(), 1);

  for (auto _ : state) {
    bm::DoNotOptimize(std::lower_bound(array.begin(), array.end(), 13));
  }

  //state.SetComplexityN(count);
  state.SetItemsProcessed(count * state.iterations());
  state.SetBytesProcessed(count * state.iterations() * sizeof(int32_t));
}

BENCHMARK_CAPTURE(std_lower_bound, seq, std::execution::seq)
  ->RangeMultiplier(8)
  ->Range(1 << 20, 1 << 25);

//BENCHMARK_CAPTURE(std_lower_bound, seq, std::execution::seq)
//    ->RangeMultiplier(8)
//    ->Range(1l << 20, 1l << 32);
//    //->MinTime(10);
//    //->Complexity(bm::oNLogN);

template <typename execution_policy_t>
static void eytz(bm::State& state, execution_policy_t&& policy) {
  auto count = static_cast<size_t>(state.range(0));
  std::vector<int32_t> array(count);
  std::iota(array.begin(), array.end(), 1);
  int32_t* eytz = (int32_t*)std::aligned_alloc(64, 4 * (count+1));
  //int32_t* eytz = new int32_t[count+1];
  vagag::eytzinger(array, eytz);
  for (auto _ : state) {
    bm::DoNotOptimize(vagag::search(eytz, count, 13));
  }

  //state.SetComplexityN(count);
  state.SetItemsProcessed(count * state.iterations());
  state.SetBytesProcessed(count * state.iterations() * sizeof(int32_t));

  std::free(eytz);
  //delete[] eytz;
}

BENCHMARK_CAPTURE(eytz, seq, std::execution::seq)
  ->RangeMultiplier(8)
  ->Range(1 << 20, 1 << 25);


//BENCHMARK_CAPTURE(eytz, seq, std::execution::seq)
//    ->RangeMultiplier(8)
//    ->Range(1l << 20, 1l << 32);
//    //->MinTime(10);
//    //->Complexity(bm::oNLogN);

template <typename execution_policy_t>
static void super_sort(bm::State& state, execution_policy_t&& policy) {
  auto count = static_cast<size_t>(state.range(0));
  std::vector<int32_t> array(count);
  std::iota(array.begin(), array.end(), 1);

  for (auto _ : state) {
    std::reverse(policy, array.begin(), array.end());
    std::sort(policy, array.begin(), array.end());
    bm::DoNotOptimize(array.size());
  }

  state.SetComplexityN(count);
  state.SetItemsProcessed(count * state.iterations());
  state.SetBytesProcessed(count * state.iterations() * sizeof(int32_t));
}

//BENCHMARK_CAPTURE(super_sort, seq, std::execution::seq)
//    ->RangeMultiplier(8)
//    ->Range(1l << 10, 1l << 20)
//    ->MinTime(10)
//    ->Complexity(bm::oNLogN);
//
//BENCHMARK_CAPTURE(super_sort, par_unseq, std::execution::par_unseq)
//    ->RangeMultiplier(8)
//    ->Range(1l << 10, 1l << 20)
//    ->MinTime(10)
//    ->Complexity(bm::oNLogN);

//static void BM_memcpy(bm::State& state) {
//  char* src = new char[state.range(0)];
//  char* dst = new char[state.range(0)];
//  memset(src, 'x', state.range(0));
//  for (auto _ : state) {
//    memcpy(dst, src, state.range(0));
//  }
//  state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(state.range(0)));
//  delete[] src;
//  delete[] dst;
//}
//BENCHMARK(BM_memcpy)->Range(8, 8<<10);

BENCHMARK_MAIN();
