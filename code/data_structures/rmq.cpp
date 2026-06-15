// =============================================================================
// @author     Jose A. Romero (jromero132)
// @algorithm  Sparse Table (Static Range Minimum Query)
// @docs       An idempotent data structure built on binary scaling intervals to
//             answer static range queries (min, max, gcd) in O(1) time after an
//             initial layout preprocessing phase.
//             Works for any idempotent & associative operator.
// @time       O(n log n) construction phase; O(1) lookup per interval query
// @space      O(n log n) memory layout allocation table
// =============================================================================

#include <algorithm>
#include <cmath>
#include <vector>

template <typename T>
struct min_op {
  T operator()(const T& a, const T& b) const { return std::min(a, b); }
};

template <typename T, typename Combine = min_op<T>>
struct rmq {  // 0-based
  public:
  rmq() : n(0), levels(0) {}

  template <typename Iter>
  rmq(Iter first, Iter last, const Combine& cmb = Combine()) : n(distance(first, last)), cmb(cmb) {
    if (n == 0) {
      levels = 0;
      return;
    }

    levels = std::__lg(n) + 1;
    blocks.assign(n, std::vector<T>(levels));
    for (int i = 0; i < n; ++i) {
      blocks[i][0] = first[i];
    }
    for (int j = 1; j < levels; ++j) {
      for (int i = 0; i <= n - (1 << j); ++i) {
        blocks[i][j] = cmb(blocks[i][j - 1], blocks[i + (1 << (j - 1))][j - 1]);
      }
    }
  }

  // Interrogates the interval range [l, r] inclusively: 0-based indexing
  T query(int l, int r) {
    int k = std::__lg(r - l + 1);
    return cmb(blocks[l][k], blocks[r - (1 << k) + 1][k]);
  }

  private:
  int n, levels;
  Combine cmb;
  std::vector<std::vector<T>> blocks;
};

#ifdef LOCAL
#include <iostream>
using namespace std;

int main() {
  vector<int> a = {4, 1, 8, 2, 9, 0, 3};
  rmq<int> table(a.begin(), a.end());

  cout << "Initial array:";
  for (int i = 0; i < a.size(); ++i) {
    cout << " " << a[i];
  }
  cout << endl;

  for (int i = 0; i < a.size(); ++i) {
    int expected_min_val = a[i];
    for (int j = i; j < a.size(); ++j) {
      expected_min_val = std::min(expected_min_val, a[j]);
      int found_min_val = table.query(i, j);
      cout << "Minimum index in range [" << i << ", " << j << "]: " << found_min_val;
      cout << " (Expected: " << expected_min_val << ")";
      if (found_min_val != expected_min_val) {
        cout << " [ERROR]";
      }
      cout << endl;
    }
  }
  return 0;
}
#endif
