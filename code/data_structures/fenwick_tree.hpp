#include <vector>

template <typename T>
struct fenwick {  // 1-index
   private:
    const int n;
    std::vector<T> tree;

   public:
    fenwick(const int n) : n(n), tree(n + 1) {}

    T query(int pos) const {
        // O(max(T1, T2 * log(exp)))
        //   - T1: time complexity of default function
        //   - T2: time complexity of adding two T values
        // e.g. using integer or double values: O(log(exp))
        T ans = T();
        for (; pos; pos -= pos & -pos) ans += tree[pos];
        return ans;
    }

    void update(int pos, const T val) {
        // O(max(T1 * log(exp)))
        //   - T1: time complexity of adding two T values
        // e.g. using integer or double values: O(log(exp))
        for (; pos <= n; pos += pos & -pos) tree[pos] += val;
    }
};