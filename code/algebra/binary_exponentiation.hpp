#include <functional>

template <typename T>
T pow(
    T base, int exp, std::function<T(const T&)> identity = [](const T& x) { return 1; }
) {
    // O(max(T1, T2 * log(exp)))
    //   - T1: time complexity of identity function
    //   - T2: time complexity of multiplying two T values
    // e.g. powering integer or double values: O(log(exp))
    T ans = identity(base);
    for (; exp > 0; exp >>= 1) {
        if (exp & 1) ans = ans * base;
        base = base * base;
    }
    return ans;
}