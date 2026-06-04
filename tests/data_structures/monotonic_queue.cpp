// =============================================================================
// @author     Jose A. Romero (jromero132)
// @unit_test  Custom Comparator Monotonic Queue Test Suite
// @docs       Verifies optimal properties under sliding configurations,
//             including min-queue inversion mapping, structural value duplicate
//             preservations, negative values, and custom comparison items.
// =============================================================================

#include "../../code/data_structures/monotonic_queue.cpp"

#include <functional>

#include "../doctest.h"

// Custom item to explicitly test multi-parameter criteria evaluation structures
struct JobItem {
  int priority;
  int weight;

  // Necessary default constructors for fallback initialization calls
  JobItem() : priority(0), weight(0) {}
  JobItem(int p, int w) : priority(p), weight(w) {}

  bool operator==(const JobItem& o) const { return priority == o.priority && weight == o.weight; }
};

// Functor checking custom prioritization order rules
struct JobComparator {
  bool operator()(const JobItem& a, const JobItem& b) const {
    if (a.priority != b.priority) {
      return a.priority < b.priority;  // Prefer higher priorities
    }
    return a.weight > b.weight;  // Tie-breaker: prefer smaller weight footprints
  }
};

TEST_SUITE("Monotonic Queue Suite") {
  TEST_CASE("Dummy & Blank State Evaluations") {
    monotonic_queue<int> mq_max;
    monotonic_queue<int, std::greater<int>> mq_min;

    CHECK(mq_max.empty() == true);
    CHECK(mq_max.best() == 0);
    CHECK(mq_min.best() == 0);
  }

  TEST_CASE("Inverted Min-Queue Operation Tracking via std::greater") {
    monotonic_queue<int, std::greater<int>> min_queue;

    // Elements entering: 100, then 50 (dominates 100), then 75 (sits behind 50)
    min_queue.add(0, 100);
    CHECK(min_queue.best() == 100);

    min_queue.add(1, 50);
    CHECK(min_queue.best() == 50);

    min_queue.add(2, 75);
    CHECK(min_queue.best() == 50);  // 50 remains absolute lowest minimum element

    min_queue.remove(1);            // Evict position index 1 (the 50 element)
    CHECK(min_queue.best() == 75);  // 75 correctly takes over
  }

  TEST_CASE("Tricky Edge Cases, Negative Bounds, and Duplicate Item Streams") {
    monotonic_queue<long long> max_queue;

    SUBCASE("Processing streaming lists containing duplicate values") {
      max_queue.add(0, 42LL);
      max_queue.add(1, 42LL);  // Duplicate entries must survive safely
      max_queue.add(2, 10LL);

      CHECK(max_queue.best() == 42LL);
      max_queue.remove(0);              // Evict first instance
      CHECK(max_queue.best() == 42LL);  // Second instance preserves maximum profile state
      max_queue.remove(1);              // Evict second instance
      CHECK(max_queue.best() == 10LL);
    }

    SUBCASE("Extreme signed negative value tracking arrays") {
      monotonic_queue<int> neg_queue;
      neg_queue.add(0, -50);
      neg_queue.add(1, -10);  // Dominates -50 from back
      neg_queue.add(2, -30);  // Sits behind -10

      CHECK(neg_queue.best() == -10);
      neg_queue.remove(1);
      CHECK(neg_queue.best() == -30);
    }
  }

  TEST_CASE("Complex Custom Struct Comparator Multi-Field Optimizations") {
    monotonic_queue<JobItem, JobComparator> job_queue;

    job_queue.add(0, JobItem(10, 200));  // Priority 10, Weight 200
    job_queue.add(1, JobItem(15, 500));  // Higher priority -> dominates and evicts index 0
    job_queue.add(2, JobItem(15, 100));  // Same priority, but lower weight -> dominates index 1

    CHECK(job_queue.best() == JobItem(15, 100));
  }
}
