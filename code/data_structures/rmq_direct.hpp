// Tested: https://judge.yosupo.jp/problem/staticrmq
//   - Submissions:
//       • https://judge.yosupo.jp/submission/173702
//       • https://judge.yosupo.jp/submission/173704

#include <algorithm>
#include <deque>
#include <functional>
#include <vector>

template <typename T, typename Compare = std::less<T>>
struct rmq_direct {  // 0-based
    rmq_direct() = default;

    template <typename Iter>
    rmq_direct(Iter first, Iter last, const Compare& comp = Compare())
        : n(distance(first, last)),
          comp(comp),
          BlockSize(32),  // BlockSize(max(static_cast<int>(1),std::__lg(n))),
          TotalBlocks((n + BlockSize - 1) / BlockSize),
          BlocksRMQ((std::__lg(TotalBlocks) + 1) * TotalBlocks),
          Masks(n) {
        // Time complexity: O(n * max(T1, T2))
        // Memory complexity: O(n)
        //   - T1: time complexity of arg_min function
        //   - T2: time complexity of comparing two data values
        //   - n: number of elements in the range [first, last)
        // e.g. using integer or double values: O(n) ; O(n)

        for (int i = 0, block = 0; i < n;) {
            for (int end = std::min(n, i + BlockSize), &min_pos = BlocksRMQ[block++] = i; ++i < end;) {
                if (comp(first[i], first[min_pos])) min_pos = i;
            }
        }
        auto cur_lvl = BlocksRMQ.begin();
        for (int m, mh = 1; (m = mh << 1) <= TotalBlocks; mh = m) {
            auto next_lvl = next(cur_lvl, TotalBlocks);
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

    template <typename Iter>
    int operator()(int l, int r, const Iter& data) const {  // [l, r) and returns the position in data
        // Time complexity: O(max(T1, T2))
        // Memory complexity: O(1)
        //   - T1: time complexity of arg_min function
        //   - T2: time complexity of in_block_query_function
        // This values are insignificant, so basically O(1)

        const int x = l / BlockSize, y = --r / BlockSize;
        int z = y - x;

        if (z == 0) return in_block_query(x, l, r);
        if (z == 1)
            return arg_min(
                in_block_query(x, l, x * BlockSize + BlockSize - 1), in_block_query(y, y * BlockSize, r), data
            );
        z = std::__lg(z - 1);
        return arg_min(
            arg_min(
                in_block_query(x, l, x * BlockSize + BlockSize - 1),
                arg_min(BlocksRMQ[TotalBlocks * z + x + 1], BlocksRMQ[TotalBlocks * z + y - (1 << z)], data), data
            ),
            in_block_query(y, y * BlockSize, r), data
        );
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
        // Time complexity: O(T1)
        // Memory complexity: O(1)
        //   - T1: time complexity of comparing two data values
        // e.g. using integer or double values: O(1) ; O(1)

        return comp(data[y], data[x]) ? y : x;
    }

    int in_block_query(int block, int l, int r) const {
        // Time complexity: O(log(L))
        // Memory complexity: O(1)
        //   - L: max possible value for the lowest bit set in r_mask. This value is insignificant, so basically O(1)

        int r_mask = Masks[r], pos = l - BlockSize * block;
        if (pos >= 1) r_mask &= ~((1 << pos) - 1);
        if (r_mask == 0) return r;
        const int ret = std::__lg(lowest_one(r_mask));
        return ret + block * BlockSize;
    }

    static inline size_t lowest_one(const size_t x) {
        // Time complexity: O(1)
        // Memory complexity: O(1)

        return x & -x;
    }
};