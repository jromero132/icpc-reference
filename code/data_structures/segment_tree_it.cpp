// =============================================================================
// @author     Jose A. Romero (jromero132)
// @algorithm  Iterative Segment Tree (Point Update, Range Query)
// @docs       A 0-indexed iterative segment tree built on a flat array layout
//             of size 2n. Avoids recursive call overhead and safely supports
//             non-commutative associative operations with identity elements.
//             Queries use the semi-open range convention: [l, r).
// @time       O(n) for vector construction; O(log n) per single update/query
// @space      O(n) memory allocation (uses a single vector of size 2n)
// =============================================================================

#include <vector>

// Default functors to provide drop-in Range Sum Query behavior
template <typename T>
struct default_combine {
  T operator()(const T& a, const T& b) const { return a + b; }
};

template <typename T, typename Combine = default_combine<T>>
struct segment_tree {
  private:
  int n;
  T identity;
  std::vector<T> tree;
  Combine combine;

  public:
  // Base constructor accepting identities and custom functor instances
  segment_tree(int n, T identity = T(), Combine combine = Combine())
      : n(n), identity(identity), tree(n << 1, identity), combine(combine) {}

  // Interrogates the 1-based inclusive interval [l, r]
  // Separates left and right results to guarantee correct non-commutative orders
  T query(int l, int r) {
    if (l > r) return identity;
    T res_l = identity;
    T res_r = identity;
    for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
      if (l & 1) res_l = combine(res_l, tree[l++]);
      if (r & 1) res_r = combine(tree[--r], res_r);
    }
    return combine(res_l, res_r);
  }

  // Combines the existing value at 1-based index p with the new val
  void update(int p, const T& val, bool is_assign = false) {
    p += n;
    if (is_assign)
      tree[p] = val;
    else
      tree[p] = combine(tree[p], val);
    for (p >>= 1; p > 0; p >>= 1) {
      tree[p] = combine(tree[p << 1], tree[p << 1 | 1]);
    }
  }
};

#ifdef LOCAL
#include <iostream>
using namespace std;

int main() {
  cout << "--- Testing Iterative 0-Indexed Segment Tree Invariants ---" << endl;

  segment_tree<long long> st_sum(5, 0);
  st_sum.update(0, 5, true);  // Array: [5, 0, 0, 0, 0]
  st_sum.update(1, 5, true);  // Array: [5, 5, 0, 0, 0]
  st_sum.update(2, 5, true);  // Array: [5, 5, 5, 0, 0]
  st_sum.update(2, 2);        // Array: [5, 5, 7, 0, 0] (7 via combine update)
  st_sum.update(3, 2, true);  // Array: [5, 5, 7, 2, 0]
  st_sum.update(4, 2, true);  // Array: [5, 5, 7, 2, 2]

  // Querying [0, 3) covers indices 0, 1, 2 -> 5 + 5 + 7 = 17
  long long r1 = st_sum.query(0, 3);
  cout << "RSQ [0, 3) - Expected: 17 | Found: " << r1;
  cout << (r1 != 17 ? " [ERROR]" : "") << endl;

  // Querying [3, 5) covers indices 3, 4 -> 2 + 2 = 4
  long long r2 = st_sum.query(3, 5);
  cout << "RSQ [3, 5) - Expected: 4  | Found: " << r2;
  cout << (r2 != 4 ? " [ERROR]" : "") << endl;

  return 0;
}
#endif
