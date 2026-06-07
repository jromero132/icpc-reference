// =============================================================================
// @author     Jose A. Romero (jromero132)
// @algorithm  Persistent Treap (Randomized Binary Search Tree via Path-Copying)
// @docs       A fully persistent balanced search tree. Any structural modification
//             creates a new version of the root while leaving old versions fully
//             immutable. Provides standard logarithmic mutations as well as
//             rank selection queries and size counters.
// @time       $O(\log n)$ for all primary mutations and statistics lookups
// @space      $O(\log n)$ memory allocation footprint per operation
// =============================================================================

#include <algorithm>
#include <chrono>
#include <random>
#include <utility>

template <typename T>
struct treap {
  public:
  struct node {
    T key;
    int priority, size;
    node* child[2];

    node(T key, int priority, int size = 1, node* left = nullptr, node* right = nullptr)
        : key(key), priority(priority), size(size) {
      child[0] = left;
      child[1] = right;
    }
  };

  private:
  // Robust 32-bit pseudo-random engine to prevent priority collisions
  static int next_priority() {
    static std::mt19937 rng(1337);  // Seeded deterministically for contest reproducibility
    return rng();
  }

  // Allocates a fresh copy of an existing node to preserve persistence
  node* clone(node* u) {
    if (!u) return nullptr;
    return new node(u->key, u->priority, u->size, u->child[0], u->child[1]);
  }

  node* rotate(node* u, bool dir) {
    if (!u || !u->child[dir] || u->child[dir]->priority <= u->priority) return update(u);

    node* v = clone(u->child[dir]);
    u->child[dir] = v->child[!dir];
    v->child[!dir] = update(u);
    return update(v);
  }

  public:
  treap() {}

  int size(node* u) { return u ? u->size : 0; }

  // Recalculates subtree sizes based on child statistics
  node* update(node* u) {
    if (u) {
      u->size = 1 + size(u->child[0]) + size(u->child[1]);
    }
    return u;
  }

  // Factory generator for creating primary baseline leaves
  node* create_node(T key) { return new node(key, next_priority()); }

  // Counts how many elements are strictly less than the given key
  int less(node* u, T key) {
    if (!u) return 0;
    if (u->key >= key) return less(u->child[0], key);
    return size(u->child[0]) + 1 + less(u->child[1], key);
  }

  // Locates the k-th smallest node using 1-based indexing
  node* kth(node* u, int k) {
    if (!u || size(u->child[0]) + 1 == k) return u;
    if (size(u->child[0]) >= k) return kth(u->child[0], k);
    return kth(u->child[1], k - size(u->child[0]) - 1);
  }

  // ---------------------------------------------------------------------------
  // INTERFACE 1: ROTATION-BASED PERSISTENT MUTATIONS
  // ---------------------------------------------------------------------------

  // Inserts an element into the tree, returning the new version root pointer
  node* insert(node* u, T key) {
    if (!u) return create_node(key);

    node* c = clone(u);
    bool dir = key > c->key;
    c->child[dir] = insert(c->child[dir], key);
    return rotate(c, dir);
  }

  // Erases an element from the tree, returning the new version root pointer
  node* erase(node* u, T key) {
    if (!u) return nullptr;

    node* c = clone(u);
    if (c->key == key) {
      if (!c->child[0] || !c->child[1]) return c->child[0] ? c->child[0] : c->child[1];
      bool dir = c->child[1]->priority > c->child[0]->priority;
      node* v = c->child[dir];
      while (v->child[!dir]) v = v->child[!dir];
      c->key = v->key;
      c->child[dir] = erase(c->child[dir], v->key);
      return update(c);
    }

    bool dir = c->key < key;
    c->child[dir] = erase(c->child[dir], key);
    return update(c);
  }

  // ---------------------------------------------------------------------------
  // INTERFACE 2: SPLIT & MERGE PERSISTENT MUTATIONS
  // ---------------------------------------------------------------------------

  // Splits the subtree u into two pieces: left gets k items, right gets the rest
  std::pair<node*, node*> split(node* u, int k) {
    if (!u) return {nullptr, nullptr};

    node* c = clone(u);
    if (size(c->child[0]) >= k) {
      auto p = split(c->child[0], k);
      c->child[0] = p.second;
      return {p.first, update(c)};
    } else {
      auto p = split(c->child[1], k - size(c->child[0]) - 1);
      c->child[1] = p.first;
      return {update(c), p.second};
    }
  }

  // Merges two persistent subtrees where all keys in u are less than keys in v
  node* merge(node* u, node* v) {
    if (!u || !v) return u ? u : v;

    if (u->priority > v->priority) {
      node* c = clone(u);
      c->child[1] = merge(c->child[1], v);
      return update(c);
    } else {
      node* c = clone(v);
      c->child[0] = merge(u, c->child[0]);
      return update(c);
    }
  }

  // Alternative insertion using Split-Merge mechanics
  node* insert2(node* u, T key) {
    auto p = split(u, less(u, key));
    return merge(merge(p.first, create_node(key)), p.second);
  }

  // Alternative erasure using Split-Merge mechanics
  node* erase2(node* u, T key) {
    auto p1 = split(u, less(u, key));
    auto p2 = split(p1.second, 1);
    if (p2.first && p2.first->key == key) return merge(p1.first, p2.second);
    return u;
  }
};

#ifdef LOCAL
#include <iostream>
using namespace std;

int main() {
  treap<int> t;

  // Version 0: Empty baseline tree
  treap<int>::node* v0 = nullptr;

  // Version 1: Insert 10 into Version 0
  treap<int>::node* v1 = t.insert(v0, 10);
  // Version 2: Insert 20 into Version 1
  treap<int>::node* v2 = t.insert(v1, 20);
  // Version 3: Insert 5 into Version 2
  treap<int>::node* v3 = t.insert(v2, 5);

  cout << "--- Testing Persistent Sizes across Versions ---" << endl;
  cout << "Version 0 Size? Expected: 0 | Found: " << t.size(v0);
  cout << (t.size(v0) != 0 ? " [ERROR]" : "") << endl;
  cout << "Version 1 Size? Expected: 1 | Found: " << t.size(v1);
  cout << (t.size(v1) != 1 ? " [ERROR]" : "") << endl;
  cout << "Version 2 Size? Expected: 2 | Found: " << t.size(v2);
  cout << (t.size(v2) != 2 ? " [ERROR]" : "") << endl;
  cout << "Version 3 Size? Expected: 3 | Found: " << t.size(v3);
  cout << (t.size(v3) != 3 ? " [ERROR]" : "") << endl;

  cout << "\n--- Testing Order Statistics (1-based kth) on Version 3 ---" << endl;
  auto k1 = t.kth(v3, 1);
  auto k2 = t.kth(v3, 2);
  auto k3 = t.kth(v3, 3);

  cout << "1st smallest? Expected: 5  | Found: " << (k1 ? k1->key : -1);
  cout << (!k1 || k1->key != 5 ? " [ERROR]" : "") << endl;
  cout << "2nd smallest? Expected: 10 | Found: " << (k2 ? k2->key : -1);
  cout << (!k2 || k2->key != 10 ? " [ERROR]" : "") << endl;
  cout << "3rd smallest? Expected: 20 | Found: " << (k3 ? k3->key : -1);
  cout << (!k3 || k3->key != 20 ? " [ERROR]" : "") << endl;

  cout << "\n--- Testing Alternative Split-Merge Erase (Version 4) ---" << endl;
  // Version 4: Erase element 10 from Version 3 using erase2
  treap<int>::node* v4 = t.erase2(v3, 10);
  cout << "Version 4 Size? Expected: 2 | Found: " << t.size(v4);
  cout << (t.size(v4) != 2 ? " [ERROR]" : "") << endl;
  cout << "Version 3 Immutability Check? Expected: 3 | Found: " << t.size(v3);
  cout << (t.size(v3) != 3 ? " [ERROR]" : "") << endl;

  return 0;
}
#endif
