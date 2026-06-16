// =============================================================================
// @author     Jose A. Romero (jromero132)
// @algorithm  Segment Tree with Lazy Propagation
// @docs       A 1-indexed generic recursive segment tree that abstracts
//             associative operators and identity values. Supports flexible
//             range updates and queries.
// @time       O(n) for layout vector initialization; O(log n) per update/query
// @space      O(n) heap memory storage footprint (safely bound to 4n vectors)
// =============================================================================

#include <vector>

// Default functors to provide drop-in Range Sum Query behavior
template <typename T>
struct default_combine {
  T operator()(const T& a, const T& b) const { return a + b; }
};

template <typename T>
struct default_lazy_apply {
  void operator()(T& tree_val, const T& lazy_val, int len) const { tree_val += lazy_val * len; }
};

template <typename T>
struct default_lazy_merge {
  void operator()(T& current_lazy, const T& new_lazy) const { current_lazy += new_lazy; }
};

template <typename T, typename Combine = default_combine<T>,
          typename LazyApply = default_lazy_apply<T>, typename LazyMerge = default_lazy_merge<T>>
struct segment_tree {
  private:
  int n;
  T identity, lazy_identity;
  std::vector<T> tree, lazy;
  Combine combine;
  LazyApply lazy_apply;
  LazyMerge lazy_merge;

  // Lazily propagates accumulated modifications down to direct child nodes
  void push(int node, int L, int R, int mid) {
    if (lazy[node] != lazy_identity) {
      lazy_apply(tree[node << 1], lazy[node], mid - L);
      lazy_apply(tree[node << 1 | 1], lazy[node], R - mid);
      lazy_merge(lazy[node << 1], lazy[node]);
      lazy_merge(lazy[node << 1 | 1], lazy[node]);
      lazy[node] = lazy_identity;
    }
  }

  // Internal recursive range query evaluator
  T query(int node, int L, int R, int ql, int qr) {
    if (ql <= L && R <= qr) return tree[node];
    int mid = (L + R) >> 1;
    push(node, L, R, mid);
    if (qr <= mid) return query(node << 1, L, mid, ql, qr);
    if (ql >= mid) return query(node << 1 | 1, mid, R, ql, qr);
    return combine(query(node << 1, L, mid, ql, qr), query(node << 1 | 1, mid, R, ql, qr));
  }

  // Internal recursive range modification handler
  void update(int node, int L, int R, int ql, int qr, const T& val) {
    if (ql <= L && R <= qr) {
      lazy_apply(tree[node], val, R - L);
      lazy_merge(lazy[node], val);
      return;
    }
    int mid = (L + R) >> 1;
    push(node, L, R, mid);
    if (ql < mid) update(node << 1, L, mid, ql, qr, val);
    if (mid < qr) update(node << 1 | 1, mid, R, ql, qr, val);
    tree[node] = combine(tree[node << 1], tree[node << 1 | 1]);
  }

  public:
  // Base constructor accepting identities and custom functor instances
  segment_tree(int n, T identity = T(), T lazy_identity = T(), Combine combine = Combine(),
               LazyApply lazy_apply = LazyApply(), LazyMerge lazy_merge = LazyMerge())
      : n(n),
        identity(identity),
        lazy_identity(lazy_identity),
        tree(n << 2, identity),
        lazy(n << 2, lazy_identity),
        combine(combine),
        lazy_apply(lazy_apply),
        lazy_merge(lazy_merge) {}

  // Interrogates the 1-based inclusive interval [ql, qr]
  T query(int ql, int qr) {
    if (ql > qr) return identity;
    return query(1, 1, n + 1, ql, qr + 1);
  }

  // Appends a uniform value transformation to the 1-based inclusive interval [ql, qr]
  void update(int ql, int qr, const T& val) {
    if (ql > qr) return;
    update(1, 1, n + 1, ql, qr + 1, val);
  }
};

#ifdef LOCAL
#include <iostream>
using namespace std;

int main() {
  cout << "--- Testing Lazy Segment Tree Local Invariants ---" << endl;

  // Test 1: Standard Range Sum Tree Configuration
  segment_tree<long long> st_sum(5, 0, 0);
  st_sum.update(1, 3, 5);  // Array state: [5, 5, 5, 0, 0]
  st_sum.update(3, 5, 2);  // Array state: [5, 5, 7, 2, 2]

  long long r1 = st_sum.query(1, 3);
  cout << "RSQ [1, 3] - Expected: 17 | Found: " << r1;
  cout << (r1 != 17 ? " [ERROR]" : "") << endl;

  long long r2 = st_sum.query(4, 5);
  cout << "RSQ [4, 5] - Expected: 4  | Found: " << r2;
  cout << (r2 != 4 ? " [ERROR]" : "") << endl;

  return 0;
}
#endif
