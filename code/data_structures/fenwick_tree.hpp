#include <vector>

template <typename T>
struct fenwick {  // 1-index
    int n;
    std::vector<T> tree;

    fenwick(int n) : n(n), tree(n + 1) {}

    T query(int pos) {
        T ans = T();
        for(; pos; pos -= pos & -pos)
            ans += tree[pos];
        return ans;
    }

    void update(int pos, T val) {
        for(; pos <= n; pos += pos & -pos)
            tree[pos] += val;
    }
};