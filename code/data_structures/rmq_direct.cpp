// =============================================================================
// @author     Jose A. Romero (jromero132)
// @algorithm  Direct Range Minimum Query (Fischer-Heun Method)
// @docs       Static array range lookup structure providing 0-based index
//             results. Queries operate on half-open intervals [l, r) where l<r.
//             Works for any idempotent & associative operator.
// @time       Preprocessing: $O(N)$, Query: $O(1)$
// @space      $O(N)$
// =============================================================================

#include <algorithm>
#include <deque>
#include <functional>
#include <iterator>
#include <vector>

template <typename T, typename Operator = std::less<T>>
struct rmq_direct {  // 0-based
  rmq_direct() = default;

  template <typename Iter>
  rmq_direct(Iter first, Iter last, const Operator& op = Operator())
      : n(distance(first, last)),
        op(op),
        block_size(std::max(static_cast<int>(1), std::__lg(n))),
        total_blocks((n + block_size - 1) / block_size),
        blocks_rmq((std::__lg(total_blocks) + 1) * total_blocks),
        masks(n) {
    for (int i = 0, block = 0; i < n;) {
      for (int end = std::min(n, i + block_size), &min_pos = blocks_rmq[block++] = i; ++i < end;) {
        if (op(first[i], first[min_pos])) min_pos = i;
      }
    }
    auto cur_lvl = blocks_rmq.begin();
    for (int m, mh = 1; (m = mh << 1) <= total_blocks; mh = m) {
      auto next_lvl = std::next(cur_lvl, total_blocks);
      for (int i = 0; i + mh < total_blocks; ++i) {
        next_lvl[i] = arg_min(cur_lvl[i], cur_lvl[i + mh], first);
      }
      cur_lvl = next_lvl;
    }
    std::deque<int> deq;
    for (int i = 0; i < n; i += block_size) {
      while (!deq.empty() && deq.front() < i) {
        deq.pop_front();
      }
      const int end = std::min(n, i + block_size);
      for (int j = i; j < end; ++j) {
        while (!deq.empty() && op(first[j], first[deq.back()])) {
          deq.pop_back();
        }
        if (!deq.empty()) {
          masks[j] = masks[deq.back()] | (1 << (deq.back() - i));
        }
        deq.push_back(j);
      }
    }
  }

  // Returns the position of the minimum element in the range [l, r) of the input data
  template <typename Iter>
  int operator()(int l, int r, const Iter& data) const {  // [l, r) and returns the position in data
    const int x = l / block_size, y = --r / block_size;
    int z = y - x;

    if (z == 0) return in_block_query(x, l, r);
    if (z == 1)
      return arg_min(in_block_query(x, l, x * block_size + block_size - 1),
                     in_block_query(y, y * block_size, r), data);
    z = std::__lg(z - 1);
    return arg_min(arg_min(in_block_query(x, l, x * block_size + block_size - 1),
                           arg_min(blocks_rmq[total_blocks * z + x + 1],
                                   blocks_rmq[total_blocks * z + y - (1 << z)], data),
                           data),
                   in_block_query(y, y * block_size, r), data);
  }

  private:
  int n;
  Operator op;
  int block_size;
  int total_blocks;
  std::vector<int> blocks_rmq;
  std::vector<int> masks;

  template <typename Iter>
  inline int arg_min(int x, int y, const Iter& data) const {
    return op(data[y], data[x]) ? y : x;
  }

  int in_block_query(int block, int l, int r) const {
    int r_mask = masks[r], pos = l - block_size * block;
    if (pos >= 1) r_mask &= ~((1 << pos) - 1);
    if (r_mask == 0) return r;
    const int ret = std::__lg(lowest_one(r_mask));
    return ret + block * block_size;
  }

  static inline size_t lowest_one(const size_t x) { return x & -x; }
};

#ifdef LOCAL
#include <iostream>
using namespace std;

int main() {
  vector<int> a = {4, 1, 8, 2, 9, 0, 3};
  rmq_direct<int> rmq(a.begin(), a.end());

  cout << "Initial array:";
  for (int i = 0; i < a.size(); ++i) {
    cout << " " << a[i];
  }
  cout << endl;

  for (int i = 0; i < a.size(); ++i) {
    int min_idx = i;
    for (int j = i + 1; j <= a.size(); ++j) {
      min_idx = a[j - 1] < a[min_idx] ? j - 1 : min_idx;
      int idx = rmq(i, j, a);
      cout << "Minimum index in range [" << i << ", " << j << "): " << idx;
      cout << " (Expected: " << min_idx << ", Value: " << a[min_idx] << ")";
      if (a[idx] != a[min_idx]) {
        cout << " [ERROR]";
      }
      cout << endl;
    }
  }
  return 0;
}
#endif
