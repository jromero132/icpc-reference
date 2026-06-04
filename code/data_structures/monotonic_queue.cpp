// =============================================================================
// @author     Jose A. Romero (jromero132)
// @algorithm  Monotonic Queue
// @docs       A deque-backed sliding window tracker that maintains elements
//             ordered according to a user-defined comparator template
//             parameter. Supports max-queue, min-queue, and custom item
//             tracking configurations.
// @time       Amortized $O(1)$ per operation
// @space      $O(W)$ where $W$ is the sliding window size constraint
// =============================================================================

#include <deque>
#include <functional>
#include <utility>

template <typename T, typename Compare = std::less_equal<T>>
struct monotonic_queue {
  public:
  monotonic_queue(const Compare& comp = Compare()) : comp(comp) {}

  // Adds a new element with key `k` and value `v` to the queue
  void add(int k, const T& v) {
    while (!deq.empty() && comp(deq.back().second, v)) {
      deq.pop_back();
    }
    deq.emplace_back(k, v);
  }

  // Removes the element with key `k` from the queue and all elements that are dominated by it
  void remove(int k) {
    if (!deq.empty() && deq.front().first <= k) {
      deq.pop_front();
    }
  }

  // Returns the best element in the queue according to the comparator
  T best() const { return deq.empty() ? T() : deq.front().second; }

  bool empty() const { return deq.empty(); }

  private:
  std::deque<std::pair<int, T>> deq;
  Compare comp;
};

#ifdef LOCAL
#include <iostream>
using namespace std;

int main() {
  struct TestStep {
    char action;  // 'A' = Add, 'R' = Remove
    int id;
    long long value;
    long long expected;
  };

  monotonic_queue<long long, std::greater<long long>> min_q;

  TestStep tests[] = {
      {'A', 0, 50, 50},  // Add 50 -> Min: 50
      {'A', 1, 20, 20},  // Add 20 -> Min: 20 (50 dominated)
      {'A', 2, 35, 20},  // Add 35 -> Min: 20
      {'R', 0, 0, 20},   // Remove 0 -> Min: 35
      {'R', 1, 0, 35},   // Remove 1 -> Min: 35 (20 leaves, 35 is next best)
      {'A', 3, 10, 10}   // Add 10 -> Min: 10
  };

  for (const auto& t : tests) {
    if (t.action == 'A') {
      min_q.add(t.id, t.value);
    } else {
      min_q.remove(t.id);
    }

    long long res = min_q.best();
    std::cout << "Op: " << t.action << " Id: " << t.id << " | Expected Best: " << t.expected
              << " | Found: " << res;
    if (res != t.expected) {
      std::cout << " [ERROR]";
    }
    std::cout << "\n";
  }
  return 0;
}
#endif
