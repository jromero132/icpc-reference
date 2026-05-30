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
  ft.update(5, 10);

  cout << "Prefix sum up to 5: " << ft.query(5) << " (Expected: 15)" << endl;
  cout << "Range sum [4, 5]: " << ft.query(4, 5) << " (Expected: 10)" << endl;
  return 0;
}
#endif
