// =============================================================================
// @author     Jose A. Romero (jromero132)
// @algorithm  AVL Tree (Self-Balancing Binary Search Tree)
// @docs       A unique-key BST supporting order statistics (0/1-indexed kth
//             element and order_of_key lower-bound indexing).
// @time       $O(\log N)$ insertion, deletion, and retrieval; $O(N)$ traversal
// @space      $O(N)$
// =============================================================================

#include <algorithm>
#include <cassert>
#include <vector>

template <typename T>
struct avl {
  struct node {
    T key;
    int h, sz;
    node* ch[2];

    int bf() const { return ch[1]->h - ch[0]->h; }

    void update() {
      h = 1 + std::max(ch[0]->h, ch[1]->h);
      sz = 1 + ch[0]->sz + ch[1]->sz;
    }
  } *root, *null;

  private:
  node* new_node(const T& key) const {
    node* u = new node();
    u->key = key;
    u->h = u->sz = 1;
    u->ch[0] = u->ch[1] = null;
    return u;
  }

  node* rotate(node* u, bool d) {
    if (u == null || u->ch[!d] == null) return u;

    node* t = u->ch[!d];
    u->ch[!d] = t->ch[d];
    t->ch[d] = u;

    u->update();
    t->update();

    return t;
  }

  node* balance(node* u) {
    u->update();

    if (u->bf() > 1) {
      if (u->ch[1]->bf() <= 0) {  // Rotate right on right child
        u->ch[1] = rotate(u->ch[1], 1);
      }
      u = rotate(u, 0);  // Rotate left
    } else if (u->bf() < -1) {
      if (u->ch[0]->bf() >= 0) {  // Rotate left on left child
        u->ch[0] = rotate(u->ch[0], 0);
      }
      u = rotate(u, 1);  // Rotate right;
    }

    u->update();
    return u;
  }

  node* insert(node* u, const T& key) {
    if (u == null) return u = new_node(key);
    if (u->key == key) return u;

    bool d = (key > u->key);
    u->ch[d] = insert(u->ch[d], key);
    return u = balance(u);
  }

  node* erase(node* u, T key) {
    if (u == null) return u;

    T tmp = u->key;
    if (u->key == key) {
      if (u->ch[0] == null || u->ch[1] == null) {
        return u->ch[u->ch[0] == null];
      } else {
        node* t = u->ch[0];
        while (t->ch[1] != null) {
          t = t->ch[1];
        }
        key = u->key = t->key;
      }
    }

    bool d = (key > tmp);
    u->ch[d] = erase(u->ch[d], key);
    return u = balance(u);
  }

  std::vector<T> in_order(const node* u) const {
    if (u == null) return {};

    std::vector<T> order = in_order(u->ch[0]);
    order.push_back(u->key);
    std::vector<T> right = in_order(u->ch[1]);
    order.insert(order.end(), right.begin(), right.end());
    return order;
  }

  public:
  avl() {
    null = new node();
    null->h = null->sz = 0;
    null->ch[0] = null->ch[1] = 0;
    root = null;
  }

  avl(std::vector<T> initial_values) : avl() {
    for (auto v : initial_values) insert(v);
  }

  template <typename Iter>
  avl(Iter first, Iter last) : avl() {
    for (auto it = first; it != last; ++it) insert(*it);
  }

  void insert(const T& key) { root = insert(root, key); }
  void erase(T key) { root = erase(root, key); }

  bool find(const T& key) const {
    node* u = root;
    while (u != null && u->key != key) u = u->ch[key > u->key];
    return u != null;
  }

  int order_of_key(const T& key) const {
    int r = 0;
    node* u = root;
    while (u != null) {
      if (key <= u->key) {
        u = u->ch[0];
      } else {
        r += u->sz - u->ch[1]->sz;
        u = u->ch[1];
      }
    }
    return r;
  }

  T kth(int k, bool idx = 0) const {
    assert(k >= 0 + idx && k < root->sz + idx);
    idx = !idx;

    node* u = root;
    while (u->sz - u->ch[1]->sz - idx != k) {
      if (k <= u->ch[0]->sz) {
        u = u->ch[0];
      } else {
        k -= u->sz - u->ch[1]->sz;
        u = u->ch[1];
      }
    }
    return u->key;
  }

  std::vector<T> in_order() const { return in_order(root); }
};

#ifdef LOCAL
#include <iostream>
using namespace std;

int main() {
  vector<int> initial_values = {10, 20, 5};
  avl<int> tree(initial_values);

  cout << "Initial values:";
  for (int x : initial_values) {
    cout << " " << x;
  }
  cout << endl;

  cout << "In-order traversal:";
  for (int x : tree.in_order()) {
    cout << " " << x;
  }
  cout << endl;

  cout << "Element at rank 1 (0-indexed): " << tree.kth(1, 0);
  cout << " (Expected: 10)" << endl;
  return 0;
}
#endif
