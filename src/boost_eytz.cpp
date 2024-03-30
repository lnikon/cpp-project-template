#include <cstddef>
#include <vector>
#include <algorithm>
#include <numeric>
#include <benchmark/benchmark.h>
#include <boost/align.hpp>
#include <boost/align/align.hpp>

#define USE_ALIGNED_VECTOR

namespace bm = benchmark;

template <typename T = int>
struct eytzinger_search {
  eytzinger_search(const std::vector<T>& a) noexcept  : b_(a.size() + 1) {
    init(a);
  }

  int index(T const& x) const noexcept {
    size_t k = 1;
    while (k < b_.size()) {
      constexpr int block_size = 64 / sizeof(T);
      __builtin_prefetch(b_.data() + k * block_size);
      k = 2 * k + !(x < b_[k]); // double negation to handle nan correctly
    }
    k >>= __builtin_ffs(~k);
    return b_[k];
  }

  size_t init(const std::vector<T>& a, size_t i = 0, size_t k = 1) noexcept {
    if (k <= a.size()) {
      i = init(a, i, 2 * k);
      b_[k] = a[i++];
      i = init(a, i, 2 * k + 1);
    }
    return i;
  }

#ifdef USE_ALIGNED_VECTOR
  std::vector<T, boost::alignment::aligned_allocator<T, 64>> b_;
#else
  std::vector<T> b_;
#endif
};

static void BM_boost_eytz(bm::State& state) {
  const auto count = state.range(0);
  std::vector<int> a(count);
  std::iota(a.begin(), a.end(), 1);
  eytzinger_search eytz_search(a);

  for (auto _ : state) {
    bm::DoNotOptimize(eytz_search.index(13));
  }

  state.SetItemsProcessed(count * state.iterations());
  state.SetBytesProcessed(count * state.iterations() * sizeof(int32_t));
}
BENCHMARK(BM_boost_eytz)->RangeMultiplier(8)->Range(1l << 20, 1l << 32);

static void std_lower_bound(bm::State& state) {
  auto count = static_cast<size_t>(state.range(0));
  std::vector<int32_t> array(count);
  std::iota(array.begin(), array.end(), 1);

  for (auto _ : state) {
    bm::DoNotOptimize(std::lower_bound(array.begin(), array.end(), 13));
  }

  state.SetItemsProcessed(count * state.iterations());
  state.SetBytesProcessed(count * state.iterations() * sizeof(int32_t));
}
BENCHMARK(std_lower_bound)->RangeMultiplier(8)->Range(1l << 20, 1l << 32);

BENCHMARK_MAIN();

