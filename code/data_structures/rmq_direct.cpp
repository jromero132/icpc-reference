// =============================================================================
// @author     Jose A. Romero (jromero132)
// @algorithm  Direct Range Minimum Query (Fischer-Heun Method)
// @docs       Static array range lookup structure providing 0-based index
//             results. Queries operate on half-open intervals [l, r) where l<r.
// @time       Preprocessing: $O(N)$, Query: $O(1)$
// @space      $O(N)$
// =============================================================================

#include <algorithm>
#include <deque>
#include <functional>
#include <iterator>
#include <vector>

template <typename T, typename Compare = std::less<T>>
struct rmq_direct {  // 0-based
  rmq_direct() = default;

  template <typename Iter>
  rmq_direct(Iter first, Iter last, const Compare& comp = Compare())
      : n(distance(first, last)),
        comp(comp),
        BlockSize(std::max(static_cast<int>(1), std::__lg(n))),
        TotalBlocks((n + BlockSize - 1) / BlockSize),
        BlocksRMQ((std::__lg(TotalBlocks) + 1) * TotalBlocks),
        Masks(n) {
    for (int i = 0, block = 0; i < n;) {
      for (int end = std::min(n, i + BlockSize), &min_pos = BlocksRMQ[block++] = i; ++i < end;) {
        if (comp(first[i], first[min_pos])) min_pos = i;
      }
    }
    auto cur_lvl = BlocksRMQ.begin();
    for (int m, mh = 1; (m = mh << 1) <= TotalBlocks; mh = m) {
      auto next_lvl = std::next(cur_lvl, TotalBlocks);
      for (int i = 0; i + mh < TotalBlocks; ++i) {
        next_lvl[i] = arg_min(cur_lvl[i], cur_lvl[i + mh], first);
      }
      cur_lvl = next_lvl;
    }
    std::deque<int> deq;
    for (int i = 0; i < n; i += BlockSize) {
      while (!deq.empty() && deq.front() < i) {
        deq.pop_front();
      }
      const int end = std::min(n, i + BlockSize);
      for (int j = i; j < end; ++j) {
        while (!deq.empty() && comp(first[j], first[deq.back()])) {
          deq.pop_back();
        }
        if (!deq.empty()) {
          Masks[j] = Masks[deq.back()] | (1 << (deq.back() - i));
        }
        deq.push_back(j);
      }
    }
  }

  // Returns the position of the minimum element in the range [l, r) of the input data
  template <typename Iter>
  int operator()(int l, int r, const Iter& data) const {  // [l, r) and returns the position in data
    const int x = l / BlockSize, y = --r / BlockSize;
    int z = y - x;

    if (z == 0) return in_block_query(x, l, r);
    if (z == 1)
      return arg_min(in_block_query(x, l, x * BlockSize + BlockSize - 1),
                     in_block_query(y, y * BlockSize, r), data);
    z = std::__lg(z - 1);
    return arg_min(arg_min(in_block_query(x, l, x * BlockSize + BlockSize - 1),
                           arg_min(BlocksRMQ[TotalBlocks * z + x + 1],
                                   BlocksRMQ[TotalBlocks * z + y - (1 << z)], data),
                           data),
                   in_block_query(y, y * BlockSize, r), data);
  }

  private:
  int n;
  Compare comp;
  int BlockSize;
  int TotalBlocks;
  std::vector<int> BlocksRMQ;
  std::vector<int> Masks;

  template <typename Iter>
  inline int arg_min(int x, int y, const Iter& data) const {
    return comp(data[y], data[x]) ? y : x;
  }

  int in_block_query(int block, int l, int r) const {
    int r_mask = Masks[r], pos = l - BlockSize * block;
    if (pos >= 1) r_mask &= ~((1 << pos) - 1);
    if (r_mask == 0) return r;
    const int ret = std::__lg(lowest_one(r_mask));
    return ret + block * BlockSize;
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
      if (idx != min_idx) {
        cout << " [ERROR]";
      }
      cout << endl;
    }
  }
  return 0;
}
#endif
