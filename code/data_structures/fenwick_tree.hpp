#include <vector>

template <typename T>
struct fenwick {  // 1-index
   private:
    const int n;
    std::vector<T> tree;

   public:
    fenwick(const int n) : n(n), tree(n + 1) {}

    T query(int pos) const {
        // Time complexity: O(max(T1, T2 * log(pos)))
        // Memory complexity: O(M1)
        //   - T1: time complexity of default function
        //   - T2: time complexity of adding and assigning two T values
        //   - M1: memory complexity of creating one T value
        // e.g. using integer or double values: O(log(pos)) ; O(1)

        T ans = T();
        for (; pos; pos -= pos & -pos) ans += tree[pos];
        return ans;
    }

    void update(int pos, const T val) {
        // Time complexity: O(max(T1 * log(pos)))
        // Memory complexity: O(1)
        //   - T1: time complexity of adding and assigning two T values
        // e.g. using integer or double values: O(log(exp))

        for (; pos <= n; pos += pos & -pos) tree[pos] += val;
    }
};