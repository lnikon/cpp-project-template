#include <ctime>
#include <cstdio>
#include <algorithm>
#include <vector>

void build(int* a, int* t, int n, int k = 1) {
    static int i = 0;
    if (k <= n) {
        build(a, t, n, 2 * k);
        t[k] = a[i++];
        build(a, t, n, 2 * k + 1);
    }
}

int baseline(int* a, int n, int x) {
    return *std::lower_bound(a, a + n, x);
}

int branchless(int* a, int n, int x) {
    int *base = a, len = n;
    while (len > 1) {
        int half = len / 2;
        base = (base[half] < x ? &base[half] : base);
        len -= half;
    }
    return *(base + (*base < x));
}

int eytzinger(int* t, int n, int x) {
    int k = 1;
    while (k <= n) {
        __builtin_prefetch(t + k * 16);
        k = 2 * k + (t[k] < x);
    }
    k >>= __builtin_ffs(~k);
    return t[k];
}

float timeit(int* a, int* q, int n, int m, int (*lower_bound)(int*, int, int)) {
    clock_t start = clock();

    int checksum = 0;

    for (int i = 0; i < m; i++)
        checksum += lower_bound(a, n, q[i]);
    
    float duration = float(clock() - start) / CLOCKS_PER_SEC;

    printf("  checksum: %d\n", checksum);
    printf("  latency: %.2fns\n", 1e9 * duration / m);

    return duration;
}

int main() {
  // play with the array size (n) and compiler versions
  const int n = 1 << 28, m = 2e6;

  std::vector<int> a(n);
  std::vector<int> q(n);
  int* t = (int*)std::aligned_alloc(64, 4*(n+1));

  //int* a = new int[n];
  //int* q = new int[m];
  //int* results = new int[m];
  //int* t = (int*)std::aligned_alloc(64, 4*(n+1));

  for (int i = 0; i < n; i++)
    a[i] = rand(); // <- careful on 16-bit platforms
  for (int i = 0; i < m; i++)
    q[i] = rand();
    
    a[0] = RAND_MAX; // to avoid dealing with end-of-array iterators 
 
    std::sort(a.data(), a.data() + n);
    build(a.data(), t, n);

    printf("std::lower_bound:\n");
    float x = timeit(a.data(), q.data(), n, m, baseline);
    
    printf("branchless:\n");
    printf("  speedup: %.2fx\n", x / timeit(a.data(), q.data(), n, m, branchless));
 
    printf("eytzinger:\n");
    printf("  speedup: %.2fx\n", x / timeit(t, q.data(), n, m, eytzinger));

    //delete[] a;
    //delete[] q;
    //delete[] results;
    std::free(t);
 
    return 0;
}

