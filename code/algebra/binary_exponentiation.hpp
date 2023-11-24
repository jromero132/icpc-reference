#include <functional>

template <typename T>
T pow(
    T base, int exp, const std::function<T(const T&)>& identity = [](const T& x) { return 1; }
) {
    // Time complexity: O(max(T1, T2 * log(exp)))
    // Memory complexity: O(M1)
    //   - T1: time complexity of identity function
    //   - T2: time complexity of multiplying and assigning two T values
    //   - M1: memory complexity of creating one T value
    // e.g. powering integer or double values: O(log(exp)) ; O(1)

    T ans = identity(base);
    for (; exp > 0; exp >>= 1) {
        if (exp & 1) ans = ans * base;
        base = base * base;
    }
    return ans;
}

template <typename T>
T pow(
    T base, int exp, T mod, const std::function<T(const T&)>& identity = [](const T& x) { return 1; }
) {
    // Time complexity: O(max(T1, T2 * log(exp)))
    // Memory complexity: O(M1)
    //   - T1: time complexity of identity function
    //   - T2: time complexity of multiplying and assigning two T values
    //   - M1: memory complexity of creating one T value
    // e.g. powering integer or double values: O(log(exp)) ; O(1)

    T ans = identity(base);
    for (; exp > 0; exp >>= 1) {
        if (exp & 1) ans = (ans * base) % mod;
        base = (base * base) % mod;
    }
    return ans;
}