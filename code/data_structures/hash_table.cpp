// =============================================================================
// @author     Jose A. Romero (jromero132)
// @algorithm  Chained Hash Table (Fixed-Capacity Variant)
// @docs       Static tracking map utilizing linked arrays. Safely handles
//             signed and negative keys via standardized absolute remainder
//             mapping.
// @time       Amortized $O(1)$ operations
//             Worst-case $O(N)$ under deep collisions
// @space      $O(N + \text{MOD})$
// =============================================================================

#include <cstring>
#include <vector>

template <typename H, typename T>
struct hash_table {
  public:
  static const int N = 5e6 + 10, MOD = 5e5 + 7;

  hash_table() : f(-1), p(-1), c(0), table(N), value(N), link(N), last(MOD, -1) {}

  T* find(H hash) {
    const int idx = get_idx(hash);
    for (f = -1, p = last[idx]; p != -1; f = p, p = link[p]) {
      if (table[p] == hash) return &value[p];
    }
    return nullptr;
  }

  void set(H hash, T val) {
    if (find(hash) != nullptr) {
      value[p] = val;
    } else {
      const int idx = get_idx(hash);
      table[c] = hash;
      value[c] = val;
      link[c] = last[idx];
      last[idx] = c++;
    }
  }

  void erase(H hash) {
    if (find(hash) == nullptr) return;
    const int idx = get_idx(hash);
    if (f == -1) {
      last[idx] = link[p];
    } else {
      link[f] = link[p];
    }
  }

  private:
  int f, p, c;
  std::vector<H> table;
  std::vector<T> value;
  std::vector<int> link, last;

  inline int get_idx(H hash) const {
    long long rem = static_cast<long long>(hash) % MOD;
    if (rem < 0) rem += MOD;
    return static_cast<int>(rem);
  }
};

#ifdef LOCAL
#include <iostream>
using namespace std;

int main() {
  hash_table<int, std::string> ht;
  pair<int, string> test_cases[] = {{42, "Answer"},
                                    {-42, "Negative Key Safe"},
                                    {1000000000, "Large Positive Key"},
                                    {-1000000000, "Large Negative Key"},
                                    {0, "Zero Key"},
                                    {-1, "Negative One Key"},
                                    {123456789, "Random Key"},
                                    {-123456789, "Negative Random Key"}};
  for (const auto& test_case : test_cases) {
    ht.set(test_case.first, test_case.second);
  }

  for (const auto& test_case : test_cases) {
    string* result = ht.find(test_case.first);
    cout << "Key: " << test_case.first << " | Expected: " << test_case.second
         << " | Found: " << (result ? *result : "<Not Found>");
    if (!result || *result != test_case.second) {
      cout << " [ERROR]";
    }
    cout << endl;
  }
  return 0;
}
#endif
