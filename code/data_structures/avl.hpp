// Tested: https://judge.yosupo.jp/problem/predecessor_problem
//   - Submission: https://judge.yosupo.jp/submission/173664

#include <algorithm>
#include <cassert>
#include <functional>
#include <vector>

template <typename T>
struct avl {
    // This structure does not accept repeated values.
    // In case you want to use another order or comparison, you can create your own structure, i.e. my_int, custom_int
    //     and define your desire comparison in there. You will need to define == and <, the other 4 comparisons can be
    //     define with these two. There is an example of this in the tests.
    //         - not equal (!(==))
    //         - less than or equal to (< || ==)
    //         - greater than (a > b == b < a)
    //         - greater than or equal (!(<))
    // In case you want to repeat values you can do it defining a pair of <value, cnt> with custom comparator.
    //     cnt: number of times `value` has been inserted in the structure. You can track this using a map.

    struct node {
        T key;
        int h, sz;
        node* ch[2];

        int bf() const {
            // Time complexity: O(1)
            // Memory complexity: O(1)

            return ch[1]->h - ch[0]->h;
        }

        void update() {
            // Time complexity: O(1)
            // Memory complexity: O(1)

            h = 1 + std::max(ch[0]->h, ch[1]->h);
            sz = 1 + ch[0]->sz + ch[1]->sz;
        }
    } *root, *null;

    node* new_node(const T& key) const {
        // Time complexity: O(T1)
        // Memory complexity: O(M1)
        //   - T1: time complexity of creating one T value
        //   - M1: memory complexity of creating one T value
        // e.g. using integer or double values: O(1) ; O(1)

        node* u = new node();
        u->key = key;
        u->h = u->sz = 1;
        u->ch[0] = u->ch[1] = null;
        return u;
    }

    node* rotate(node* u, bool d) {
        // Time complexity: O(1)
        // Memory complexity: O(1)
        // If d == 0, rotates left otherwise rotates right;

        if (u == null || u->ch[!d] == null) return u;

        node* t = u->ch[!d];
        u->ch[!d] = t->ch[d];
        t->ch[d] = u;

        u->update();
        t->update();

        return t;
    }

    node* balance(node* u) {
        // Time complexity: O(1)
        // Memory complexity: O(1)

        u->update();

        if (u->bf() > 1) {
            if (u->ch[1]->bf() <= 0) {  // Rotate right on right child
                u->ch[1] = rotate(u->ch[1], 1);
            }
            u = rotate(u, 0);  // Rotate left
        }
        else if (u->bf() < -1) {
            if (u->ch[0]->bf() >= 0) {  // Rotate left on left child
                u->ch[0] = rotate(u->ch[0], 0);
            }
            u = rotate(u, 1);  // Rotate right;
        }

        u->update();
        return u;
    }

    node* insert(node* u, const T& key) {
        // Time complexity: O(T1 * log(n))
        // Memory complexity: O(1)
        //   - T1: time complexity of comparing two T values
        //   - n: number of elements in the tree
        // e.g. using integer or double values: O(log(n)) ; O(1)

        if (u == null) return u = new_node(key);
        if (u->key == key) return u;

        bool d = (key > u->key);
        u->ch[d] = insert(u->ch[d], key);
        return u = balance(u);
    }

    void insert(const T& key) {
        // Time complexity: O(T1 * log(n))
        // Memory complexity: O(1)
        //   - T1: time complexity of comparing two T values
        //   - n: number of elements in the tree
        // e.g. using integer or double values: O(log(n)) ; O(1)

        root = insert(root, key);
    }

    node* erase(node* u, T key) {
        // Time complexity: O(T1 * log(n))
        // Memory complexity: O(1)
        //   - T1: time complexity of comparing two T values
        //   - n: number of elements in the tree
        // e.g. using integer or double values: O(log(n)) ; O(1)

        if (u == null) return u;

        T tmp = u->key;
        if (u->key == key) {
            if (u->ch[0] == null || u->ch[1] == null) {
                return u->ch[u->ch[0] == null];
            }
            else {
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

    void erase(T key) {
        // Time complexity: O(T1 * log(n))
        // Memory complexity: O(1)
        //   - T1: time complexity of comparing two T values
        //   - n: number of elements in the tree
        // e.g. using integer or double values: O(log(n)) ; O(1)

        root = erase(root, key);
    }

    bool find(const T& key) const {
        // Time complexity: O(T1 * log(n))
        // Memory complexity: O(1)
        //   - T1: time complexity of comparing two T values
        //   - n: number of elements in the tree
        // e.g. using integer or double values: O(log(n)) ; O(1)

        node* u = root;
        while (u != null && u->key != key) {
            u = u->ch[key > u->key];
        }
        return u != null;
    }

    int order_of_key(const T& key) const {
        // Returns the position 0-based of the smallest key which is greater than or equal to key.
        // Time complexity: O(T1 * log(n))
        // Memory complexity: O(1)
        //   - T1: time complexity of comparing two T values
        //   - n: number of elements in the tree
        // e.g. using integer or double values: O(log(n)) ; O(1)

        int r = 0;
        node* u = root;
        while (u != null) {
            if (key <= u->key) {
                u = u->ch[0];
            }
            else {
                r += u->sz - u->ch[1]->sz;
                u = u->ch[1];
            }
        }
        return r;
    }

    T kth(int k, bool idx = 0) const {
        // Returns the kth element ordered from lowest to greatest.
        //   - idx: if you are searching 0-based or 1-based
        // Time complexity: O(T1 * log(n))
        // Memory complexity: O(1)
        //   - T1: time complexity of comparing two T values
        //   - n: number of elements in the tree
        // e.g. using integer or double values: O(log(n)) ; O(1)

        assert(k >= 0 + idx && k < root->sz + idx);
        idx = !idx;

        node* u = root;
        while (u->sz - u->ch[1]->sz - idx != k) {
            if (k <= u->ch[0]->sz) {
                u = u->ch[0];
            }
            else {
                k -= u->sz - u->ch[1]->sz;
                u = u->ch[1];
            }
        }
        return u->key;
    }

    std::vector<T> in_order(const node* u) const {
        // Time complexity: O(n)
        // Memory complexity: O(n)
        //   - n: number of elements in the subtree that has u as the root (u->sz)

        if (u == null) return {};

        std::vector<T> order = in_order(u->ch[0]);
        order.push_back(u->key);
        std::vector<T> right = in_order(u->ch[1]);
        order.insert(order.end(), right.begin(), right.end());
        return order;
    }

    std::vector<T> in_order() const {
        // Time complexity: O(n)
        // Memory complexity: O(n)
        //   - n: number of elements in the tree

        return in_order(root);
    }

    avl() {
        // Time complexity: O(T1)
        // Memory complexity: O(M1)
        //   - T1: time complexity of creating one T value
        //   - M1: memory complexity of creating one T value
        // e.g. using integer or double values: O(1) ; O(1)

        null = new node();
        null->h = null->sz = 0;
        null->ch[0] = null->ch[1] = 0;
        root = null;
    }

    avl(std::vector<T> initial_values) : avl() {
        // Time complexity: O(n * T1)
        // Memory complexity: O(M1)
        //   - T1: time complexity of insert function
        //   - M1: memory complexity of insert function
        //   - n: number of elements in the vector

        for (auto v : initial_values) {
            insert(v);
        }
    }

    template <typename Iter>
    avl(Iter first, Iter last) : avl() {
        for (auto it = first; it != last; ++it) {
            insert(*it);
        }
    }
};