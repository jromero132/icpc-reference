#include <functional>

template <typename T>
T pow(
    T base, int exp, std::function<T(const T&)> identity = [](const T& x) { return 1; }
) {
    T ans = identity(base);
    for (; exp > 0; exp >>= 1) {
        if (exp & 1) ans = ans * base;
        base = base * base;
    }
    return ans;
}