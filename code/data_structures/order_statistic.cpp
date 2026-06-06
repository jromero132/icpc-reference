// =============================================================================
// @author     Jose A. Romero (jromero132)
// @algorithm  Policy-Based Ordered Set & Priority Queue Alias Layout
// @docs       Encapsulated wrapper over GNU PBDS tree. Maintains sorted unique
//             elements while providing rank queries and positional indexing.
// @time       $O(\log n)$ mutations, rank lookups, and positional indexing
// @space      $O(n)$
// =============================================================================

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/priority_queue.hpp>
#include <ext/pb_ds/tree_policy.hpp>

// =============================================================================
// ORDER STATISTIC SET METHODS DOCUMENTATION
// =============================================================================
// The 'order_statistic_set' behaves like a standard std::set (maintains unique,
// sorted elements) but introduces high-performance logarithmic-time queries:
//
// insert(x) [O(log n)]        :  Inserts element x if not present.
// erase(x) [O(log n)]         :  Removes element x from the tree.
// find(x) [O(log n)]          :  Returns an iterator to x, or end() if absent.
// empty() / size() [O(1)]     :  Standard container capacity checks.
// order_of_key(x) [O(log n)]  :  Returns the count of elements smaller than x.
//                             :  Acts as a 0-based rank query.
// find_by_order(k) [O(log n)] :  Returns an iterator to the k-th smallest
//                                element using 0-based indexing.
//                                Returns end() if out of bounds.
// =============================================================================
template <typename T, typename Compare = std::less<T>>
using order_statistic_set =
    __gnu_pbds::tree<T, __gnu_pbds::null_type, Compare, __gnu_pbds::rb_tree_tag,
                     __gnu_pbds::tree_order_statistics_node_update>;

// =============================================================================
// GNU PBDS PRIORITY QUEUE HEAP TAGS & METHODS DOCUMENTATION
// =============================================================================
// Supported Heap Tags (Passed as the 'Tag' argument):
// binary_heap_tag      : Array-backed layout identical to std::priority_queue.
//                        Excellent cache locality. merging/joining takes O(n).
// binomial_heap_tag    : Forest of heap-ordered trees. Worst-case O(log n) for
//                        all main operations, including merging/joining.
// pairing_heap_tag     : Multi-way tree structure. Push/join is amortized
//                        O(1), making it the fastest choice for Dijkstra's
//                        updates.
// rc_binomial_heap_tag : Binomial heap using redundant binary counting.
//                        Guarantees strict O(1) worst-case push operations.
// thin_heap_tag        : Optimized Fibonacci heap variant. Excellent for
//                        frequent key updates via decrease-key mechanics.
//
// Core Heap Methods:
// - push(x)          : Inserts element x into the heap.
// - pop()            : Removes the top element.
// - top()            : Returns a const reference to the top element.
// - empty() / size() : Standard capacity status checks.
// - join(other)      : Merges 'other' heap into this heap, completely
//                      clearing the 'other' instance.
// =============================================================================
template <typename T, typename Compare = std::less<T>, typename Tag = __gnu_pbds::binary_heap_tag>
using heap = __gnu_pbds::priority_queue<T, Compare, Tag>;

#ifdef LOCAL
#include <iostream>
using namespace std;

int main() {
  // ---------------------------------------------------------------------------
  // 1. Testing order_statistic_set
  // ---------------------------------------------------------------------------
  order_statistic_set<int> oss;
  oss.insert(10);
  oss.insert(20);
  oss.insert(5);

  cout << "--- Testing Container Size & Emptiness State ---" << endl;
  cout << "Empty? Expected: 0 | Found: " << oss.empty();
  cout << (oss.empty() != 0 ? " [ERROR]" : "") << endl;
  cout << "Size?  Expected: 3 | Found: " << oss.size();
  cout << (oss.size() != 3 ? " [ERROR]" : "") << endl;

  struct OrderTestCase {
    int query;
    int expected_order;
  } order_cases[] = {{5, 0}, {10, 1}, {15, 2}, {20, 2}, {25, 3}};

  cout << "\n--- Testing order_of_key(x) [Elements strictly < x] ---" << endl;
  for (const auto& tc : order_cases) {
    int res = oss.order_of_key(tc.query);
    cout << "Key: " << tc.query << " | Expected Order: " << tc.expected_order;
    cout << " | Found: " << res;
    if (res != tc.expected_order) cout << " [ERROR]";
    cout << endl;
  }

  struct KthTestCase {
    int index;
    int expected_val;
  } kth_cases[] = {{0, 5}, {1, 10}, {2, 20}};

  cout << "\n--- Testing find_by_order(k) [Locating element by 0-based rank] ---" << endl;
  for (const auto& tc : kth_cases) {
    auto it = oss.find_by_order(tc.index);
    if (it == oss.end()) {
      cout << "Index: " << tc.index << " | Expected Value: " << tc.expected_val;
      cout << " | Found: <End Iterator> [ERROR]" << endl;
    } else {
      int res = *it;
      cout << "Index: " << tc.index << " | Expected Value: " << tc.expected_val;
      cout << " | Found: " << res;
      if (res != tc.expected_val) cout << " [ERROR]";
      cout << endl;
    }
  }

  cout << "\n--- Testing Element Search & Erase Hooks ---" << endl;
  auto find_it = oss.find(10);
  cout << "Find 10? Expected: Yes | Found: ";
  cout << (find_it != oss.end() ? "Yes" : "No [ERROR]") << endl;

  oss.erase(10);
  find_it = oss.find(10);
  cout << "Find 10 after erasure? Expected: No | Found: ";
  cout << (find_it == oss.end() ? "No" : "Yes [ERROR]") << endl;
  cout << "Post-Erase Size? Expected: 2 | Found: " << oss.size();
  cout << (oss.size() != 2 ? " [ERROR]" : "") << endl;

  // ---------------------------------------------------------------------------
  // 2. Testing heap (Priority Queue) and Joining Capabilities
  // ---------------------------------------------------------------------------
  cout << "\n--- Testing heap (PBDS Priority Queue) & Join Methods ---" << endl;
  heap<int> h1;
  heap<int> h2;

  h1.push(30);
  h1.push(10);
  h2.push(40);
  h2.push(20);

  cout << "Heap 1 Top? Expected: 30 | Found: " << h1.top();
  cout << (h1.top() != 30 ? " [ERROR]" : "") << endl;
  cout << "Heap 2 Top? Expected: 40 | Found: " << h2.top();
  cout << (h2.top() != 40 ? " [ERROR]" : "") << endl;

  // Merging h2 elements straight into h1
  h1.join(h2);

  cout << "Post-Join Heap 1 Size? Expected: 4 | Found: " << h1.size();
  cout << (h1.size() != 4 ? " [ERROR]" : "") << endl;
  cout << "Post-Join Heap 2 Size? Expected: 0 | Found: " << h2.size();
  cout << (h2.size() != 0 ? " [ERROR]" : "") << endl;

  int expected_heap_order[] = {40, 30, 20, 10};
  cout << "Extracting merged heap elements sequentially:" << endl;
  for (int i = 0; i < 4; ++i) {
    int curr = h1.top();
    h1.pop();
    cout << "Extracted: " << curr << " | Expected: " << expected_heap_order[i];
    if (curr != expected_heap_order[i]) cout << " [ERROR]";
    cout << endl;
  }

  return 0;
}
#endif
