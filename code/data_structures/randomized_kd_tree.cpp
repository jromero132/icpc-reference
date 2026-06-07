// =============================================================================
// @author     Jose A. Romero (jromero132)
// @algorithm  Randomized K-D Tree
// @docs       A randomized spatial data structure that supports static
//             building, dynamic point insertion, and deletion via randomized
//             join/split operations, alongside efficient nearest-neighbor
//             lookups.
// @time       O(log n) average for insertion, deletion, and closest queries;
//             O(n log n) for static build.
// @space      O(n) total storage footprint
// =============================================================================

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <initializer_list>
#include <limits>
#include <utility>
#include <vector>

// NOTE ON EXTENSION: Change K = 2 to K = 3 to shift from 2D to 3D instantly.
template <size_t K = 2>
struct point {
  double x[K];

  point() {
    for (size_t i = 0; i < K; ++i) x[i] = 0.0;
  }

  point(std::initializer_list<double> l) {
    size_t i = 0;
    for (double val : l) {
      if (i < K) x[i++] = val;
    }
    while (i < K) x[i++] = 0.0;
  }

  double& operator[](size_t i) { return x[i]; }
  const double& operator[](size_t i) const { return x[i]; }
  bool operator==(const point& p) const {
    for (size_t i = 0; i < K; ++i) {
      if (x[i] != p.x[i]) return false;
    }
    return true;
  }
};

// Squared Euclidean distance helper across K dimensions
template <size_t K>
double dist_sq(const point<K>& a, const point<K>& b) {
  double sum = 0;
  for (size_t i = 0; i < K; ++i) {
    double d = a[i] - b[i];
    sum += d * d;
  }
  return sum;
}

template <size_t K = 2>
struct randomized_kd_tree {
  public:
  struct node {
    point<K> p;
    int dim;  // Axis index range: [0, K-1]
    int size;
    node *left, *right;

    node(point<K> p, int d) : p(p), dim(d), size(1), left(nullptr), right(nullptr) {}
  };

  private:
  node* root;

  int size(node* t) const { return t ? t->size : 0; }

  // Recalculates the size of the subtree rooted at t
  node* update(node* t) {
    if (t) {
      t->size = 1 + size(t->left) + size(t->right);
    }
    return t;
  }

  // Evaluates if point A is strictly left of point B along axis D
  bool is_left(const point<K>& a, const point<K>& b, int d) const { return a[d] < b[d]; }

  // Generates a static balanced subtree via median-finding
  node* build(std::vector<point<K>>& points, int low, int high, int dim) {
    if (low >= high) return nullptr;

    int mid = (low + high) / 2;
    std::nth_element(points.begin() + low, points.begin() + mid, points.begin() + high,
                     [dim](const point<K>& a, const point<K>& b) { return a[dim] < b[dim]; });

    node* t = new node({points[mid], dim});
    int next_dim = (dim + 1) % K;
    t->left = build(points, low, mid, next_dim);
    t->right = build(points, mid + 1, high, next_dim);
    return update(t);
  }

  // Partitions a subtree into two independent trees relative to a hyperplane
  std::pair<node*, node*> split(node* t, point<K> x_p, int x_dim) {
    if (!t) return {nullptr, nullptr};

    if (t->dim == x_dim) {
      if (is_left(t->p, x_p, x_dim)) {
        auto p = split(t->right, x_p, x_dim);
        t->right = p.first;
        return {update(t), p.second};
      } else {
        auto p = split(t->left, x_p, x_dim);
        t->left = p.second;
        return {p.first, update(t)};
      }
    } else {
      auto l = split(t->left, x_p, x_dim);
      auto r = split(t->right, x_p, x_dim);
      if (is_left(t->p, x_p, x_dim)) {
        t->left = l.first;
        t->right = r.first;
        return {update(t), join(l.second, r.second, t->dim)};
      } else {
        t->left = l.second;
        t->right = r.second;
        return {join(l.first, r.first, t->dim), update(t)};
      }
    }
  }

  // Randomly merges two distinct subtrees into a single integrated tree
  node* join(node* left, node* right, int dim) {
    if (!left) return right;
    if (!right) return left;

    if (std::rand() % (size(left) + size(right)) < size(left)) {
      if (left->dim == dim) {
        left->right = join(left->right, right, dim);
        return update(left);
      } else {
        auto p = split(right, left->p, left->dim);
        left->left = join(left->left, p.first, dim);
        left->right = join(left->right, p.second, dim);
        return update(left);
      }
    } else {
      if (right->dim == dim) {
        right->left = join(left, right->left, dim);
        return update(right);
      } else {
        auto p = split(left, right->p, right->dim);
        right->left = join(p.first, right->left, dim);
        right->right = join(p.second, right->right, dim);
        return update(right);
      }
    }
  }

  // Inserts a new spatial node into the subtree with randomized treap priority
  node* insert(node* t, node* x) {
    if (!t) return x;

    if (rand() % (size(t) + 1) == 0) {
      auto p = split(t, x->p, x->dim);
      x->left = p.first;
      x->right = p.second;
      return update(x);
    } else {
      if (is_left(x->p, t->p, t->dim)) {
        t->left = insert(t->left, x);
      } else {
        t->right = insert(t->right, x);
      }
      return update(t);
    }
  }

  // Locates and deletes an exact coordinate match
  node* remove(node* t, point<K> p) {
    if (!t) return nullptr;

    if (t->p == p) {
      return join(t->left, t->right, t->dim);
    }
    if (is_left(p, t->p, t->dim)) {
      t->left = remove(t->left, p);
    } else {
      t->right = remove(t->right, p);
    }
    return update(t);
  }

  // Recursively updates the global nearest-neighbor bound with spatial pruning
  void closest(node* t, point<K> p, std::pair<double, node*>& ub) const {
    if (!t) return;

    double r = dist_sq(t->p, p);
    if (r < ub.first) {
      ub = {r, t};
    }
    node *first = t->right, *second = t->left;
    double w = p[t->dim] - t->p[t->dim];
    if (w < 0) std::swap(first, second);
    closest(first, p, ub);
    if (ub.first > w * w) closest(second, p, ub);
  }

  void clear(node* t) {
    if (!t) return;
    clear(t->left);
    clear(t->right);
    delete t;
  }

  public:
  randomized_kd_tree() : root(nullptr) {}
  ~randomized_kd_tree() { clear(root); }

  int size() { return size(root); }

  void build(std::vector<point<K>>& points) {
    clear(root);
    root = build(points, 0, points.size(), 0);
  }

  void insert(point<K> p) { root = insert(root, new node({p, static_cast<int>(rand() % K)})); }
  void remove(point<K> p) { root = remove(root, p); }

  point<K> closest(point<K> p) const {
    std::pair<double, node*> ub(std::numeric_limits<double>::infinity(), 0);
    closest(root, p, ub);
    return ub.second ? ub.second->p : point<K>();
  }
};

#ifdef LOCAL
#include <iostream>
using namespace std;

int main() {
  std::srand(1337);  // Seed for reproducible tests

  cout << "--- Testing 2D Spatial Layout Tree Invariants ---" << endl;
  {
    randomized_kd_tree<2> tree;

    // Initial verification
    cout << "Initial size? Expected: 0 | Found: " << tree.size();
    cout << (tree.size() != 0 ? " [ERROR]" : "") << endl;

    // Test Case: Static Build Verification
    std::vector<point<2>> points = {{2.0, 3.0}, {5.0, 4.0}, {9.0, 6.0},
                                    {4.0, 7.0}, {8.0, 1.0}, {7.0, 2.0}};
    tree.build(points);

    cout << "Size after static build? Expected: 6 | Found: " << tree.size();
    cout << (tree.size() != 6 ? " [ERROR]" : "") << endl;

    // Closest Point Search Verification
    point<2> query1 = {9.0, 2.0};
    point<2> result1 = tree.closest(query1);
    cout << "Closest to (9, 2)? Expected: (8, 1) | ";
    cout << "Found: (" << result1[0] << ", " << result1[1] << ")";
    cout << (!(result1 == point<2>{8.0, 1.0}) ? " [ERROR]" : "") << endl;

    point<2> query2 = {1.0, 2.0};
    point<2> result2 = tree.closest(query2);
    cout << "Closest to (1, 2)? Expected: (2, 3) | ";
    cout << "Found: (" << result2[0] << ", " << result2[1] << ")";
    cout << (!(result2 == point<2>{2.0, 3.0}) ? " [ERROR]" : "") << endl;

    // Dynamic Insertion Verification
    tree.insert({1.0, 1.0});
    cout << "Size after dynamic insert? Expected: 7 | Found: " << tree.size();
    cout << (tree.size() != 7 ? " [ERROR]" : "") << endl;

    point<2> result3 = tree.closest(query2);
    cout << "Closest to (1, 2) after insert? Expected: (1, 1) | ";
    cout << "Found: (" << result3[0] << ", " << result3[1] << ")";
    cout << (!(result3 == point<2>{1.0, 1.0}) ? " [ERROR]" : "") << endl;

    // Dynamic Deletion Verification
    tree.remove({1.0, 1.0});
    cout << "Size after deletion? Expected: 6 | Found: " << tree.size();
    cout << (tree.size() != 6 ? " [ERROR]" : "") << endl;

    point<2> result4 = tree.closest(query2);
    cout << "Closest to (1, 2) after delete? Expected: (2, 3) | ";
    cout << "Found: (" << result4[0] << ", " << result4[1] << ")";
    cout << (!(result4 == point<2>{2.0, 3.0}) ? " [ERROR]" : "") << endl;
  }

  cout << "\n--- Testing 3D Spatial Generalization Tree Invariants ---" << endl;
  {
    randomized_kd_tree<3> tree3d;

    std::vector<point<3>> points3d = {{1.0, 1.0, 1.0}, {10.0, 10.0, 10.0}, {5.0, 5.0, 5.0}};
    tree3d.build(points3d);

    cout << "3D Size after build? Expected: 3 | Found: " << tree3d.size();
    cout << (tree3d.size() != 3 ? " [ERROR]" : "") << endl;

    point<3> query3d = {9.0, 8.0, 9.0};
    point<3> result3d = tree3d.closest(query3d);
    cout << "Closest 3D to (9, 8, 9)? Expected: (10, 10, 10) | ";
    cout << "Found: (" << result3d[0] << ", " << result3d[1] << ", " << result3d[2] << ")";
    cout << (!(result3d == point<3>{10.0, 10.0, 10.0}) ? " [ERROR]" : "") << endl;
  }
  return 0;
}
#endif
