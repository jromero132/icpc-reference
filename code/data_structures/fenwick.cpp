// =============================================================================
// @author     Jose A. Romero (jromero132)
// @algorithm  Binary Indexed Tree (Fenwick Tree)
// @docs       1-indexed point update and prefix/range sum queries.
// @time       $O(\log N)$ per operation
// @space      $O(N)$
// =============================================================================

#include <vector>

template <typename T>
struct fenwick {  // 1-index
  private:
  const int n;
  std::vector<T> tree;

  public:
  fenwick(const int n) : n(n), tree(n + 1) {}

  // Returns prefix sum from 1 to pos
  T query(int pos) const {
    T ans = T();
    for (; pos; pos -= pos & -pos) ans += tree[pos];
    return ans;
  }

  // Returns range sum from l to r [l, r]
  T query(int l, int r) const { return query(r) - query(l - 1); }

  // Adds val to the element at pos
  void update(int pos, const T val) {
    for (; pos <= n; pos += pos & -pos) tree[pos] += val;
  }
};

#ifdef LOCAL
#include <iostream>
using namespace std;

int main() {
  fenwick<long long> ft(10);
  ft.update(3, 5);
  const int expected_sum_1 = 5;
  const int result_1 = ft.query(5);
  cout << "Prefix sum up to 5: " << result_1;
  cout << " (Expected: " << expected_sum_1 << ")";
  if (result_1 != expected_sum_1) {
    cout << " [ERROR]";
  }
  cout << endl;

  ft.update(5, 10);
  const int expected_sum_2 = 10;
  const int result_2 = ft.query(4, 5);
  cout << "Range sum [4, 5]: " << result_2 << " (Expected: " << expected_sum_2 << ")" << endl;
  if (result_2 != expected_sum_2) {
    cout << " [ERROR]";
  }
  cout << endl;
  return 0;
}
#endif
