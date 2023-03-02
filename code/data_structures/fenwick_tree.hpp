#include <vector>

template <typename T>
struct fenwick {  // 1-index
  private:
    const int n;
    std::vector<T> tree;

  public:
    fenwick(const int n) : n(n), tree(n + 1) {}

    T query(int pos) const {
        T ans = T();
        for(; pos; pos -= pos & -pos)
            ans += tree[pos];
        return ans;
    }

    void update(int pos, const T val) {
        for(; pos <= n; pos += pos & -pos)
            tree[pos] += val;
    }
};