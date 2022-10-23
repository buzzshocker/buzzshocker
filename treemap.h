#ifndef TREEMAP_H_
#define TREEMAP_H_

#include <cstddef>
#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <stack>
#include <utility>
#include <stdexcept>

template <typename K, typename V>
class Treemap {
 public:
  //
  // @@@ The class's public API below should _not_ be modified @@@
  //

  // * Capacity
  // Returns number of key-value mappings in map --O(1)
  size_t Size();  /// Done
  // Returns true if map is empty --O(1)
  bool Empty();  /// Done

  // * Modifiers
  // Insert @key in map --O(log N) on average
  // Throws exception if key already exists
  void Insert(const K &key, const V &value);  /// Done
  // Remove @key from map --O(log N) on average
  // Throws exception if key doesn't exist
  void Remove(const K &key);  /// Done

  // * Lookup
  // Return value corresponding to @key --O(log N) on average
  // Throws exception if tree is empty or key doesn't exist
  const V& Get(const K &key);  /// Done

  // Return greatest key less than or equal to @key --O(log N) on average
  // Throws exception if tree is empty or no floor exists for key
  const K& FloorKey(const K &key);  /// Done
  // Return least key greater than or equal to @key --O(log N) on average
  // Throws exception if tree is empty or no ceil exists for key
  const K& CeilKey(const K &key);  /// Done

  // Return whether @key is found in map --O(log N) on average
  bool ContainsKey(const K& key);  /// Done
  // Return whether @value is found in map --O(N)
  bool ContainsValue(const V& value);  /// Done

  // Return max key in map --O(log N) on average
  // Throws exception if tree is empty
  const K& MaxKey();  /// Done
  // Return min key in map --O(log N) on average
  // Throws exception if tree is empty
  const K& MinKey();  /// Done

 private:
  // Private member variables
  struct Node {
      K key;  // The key of the node
      V value;  // The value of the node
      std::unique_ptr<Node> left;  // Pointer to the left child
      std::unique_ptr<Node> right;  // Pointer to the right child
  };

  // Private constants
  std::unique_ptr<Node> root;  // Pointer for the root
  unsigned size = 0;  // Variable for recording size
  // Assists in constant time return of size of treemap

  // Private methods
  // Returns minimum value
  Node* Min(Node* n);  /// Done
  // Returns maximum value
  Node* Max(Node* n);  /// Done
  // Inserts key and value into the treemap, helper method
  void Insert_P(std::unique_ptr<Node> &n,
                const K& key, const V& value);  /// Done
  // Removes key and value from the treemap, helper method
  void Remove_P(std::unique_ptr<Node> &n, const K& key);  /// Done
  // Preorder Traversal
  bool PreorderTraverse(Node* n, const V& Value);  /// Done
};

template <typename K, typename V>
size_t Treemap<K, V>::Size() {
    // Returns class member size
    return size;
}

template <typename K, typename V>
bool Treemap<K, V>::Empty() {
    if (size == 0) {
        // If size 0
        return true;
    } else {
        return false;
    }
}

template <typename K, typename V>
typename Treemap<K, V>::Node* Treemap<K, V>::Min(Node *n) {
    if (n -> left) {
        // If there exist values to left of n, go there and check
        return (Min(n -> left.get()));
    } else {
        // If not, return that node
        return n;
    }
}

template <typename K, typename V>
const K& Treemap<K, V>::MinKey() {
    if (Empty()) {
        // Throws exception if tree is empty
        throw std::logic_error("Empty tree");
    }
    // Calls helper method, returns key at node found
    return ((Min(root.get())) -> key);
}

template <typename K, typename V>
typename Treemap<K, V>::Node* Treemap<K, V>::Max(Node *n) {
    if (n -> right) {
        // If there exist values to the right of n, go there
        return (Max(n -> right.get()));
    } else {
        // If not, return n
        return n;
    }
}

template <typename K, typename V>
const K& Treemap<K, V>::MaxKey() {
    if (Empty()) {
        // Throw exception if tree is empty
        throw std::logic_error("Empty tree");
    }
    // Calls helper method, returns key at node found
    return (Max(root.get()) -> key);
}

template <typename K, typename V>
bool Treemap<K, V>::ContainsKey(const K &key) {
    Node* n = root.get();
    while (n) {
        // If key exists, return true
        if (n -> key == key) {
            return true;
        } else if (key > n -> key) {
            // If key greater than key at current node, go right
            n = n -> right.get();
        } else {
            // If key less than key at current node, go left
            n = n -> left.get();
        }
    }
    // If key not found, return false
    return false;
}

template <typename K, typename V>
const V& Treemap<K, V>::Get(const K &key) {
    Node* n = root.get();
    if (!ContainsKey(key)) {
        // If key does not exist, throw exception
        throw std::invalid_argument("Invalid key");
    } else if (size == 0) {
        // If tree is empty, throw exception
        throw std::logic_error("Empty tree");
    } else {
        while (n) {
            if (n -> key == key) {
                // Return value when key found
                return n -> value;
            } else if (n -> key > key) {
                // If key less than key at current node, go left
                n = n -> left.get();
            } else {
                // If key more than key at current node, go right
                n = n -> right.get();
            }
        }
    }
    return n -> value;
}

template <typename K, typename V>
bool Treemap<K, V>::ContainsValue(const V& value) {
    // Calls helper method
    return PreorderTraverse(root.get(), value);
}

template <typename K, typename V>
bool Treemap<K, V>::PreorderTraverse(Node* n, const V &value) {
    /* Local variables to be used as return values */
    bool result1, result2 = false;
    if (!n) {
        return result2;
    }
    if (n -> value == value) {
        // If value found, return value
        return true;
    }
    // If not found, recurse through left subtree.
    result1 = PreorderTraverse(n -> left.get(), value);
    if (result1) {
        // Return result1 as true if found in left subtree
        return result1;
    }
    // If not found, recurse through right subtree
    result2 = PreorderTraverse(n -> right.get(), value);
    // Final result will be whatever result2 is
    return result2;
}

template <typename K, typename V>
void Treemap<K, V>::Insert_P(std::unique_ptr<Node> &n,
                            const K &key, const V &value) {
    if (!n) {
        // If node doesn't exist, insert
        n = std::unique_ptr<Node>(new Node{key, value});
        size++;
    } else if (key < n -> key) {
        // If key less than key at current node, go left
        Insert_P(n -> left, key, value);
    } else if (key > n -> key) {
        // If key more than key at current node, go right
        Insert_P(n -> right, key, value);
    } else {
         // If key already exists, throw exception
        throw std::invalid_argument("Duplicate key");
    }
}

template <typename K, typename V>
void Treemap<K, V>::Insert(const K &key, const V &value) {
    // Calls helper method
    Insert_P(root, key, value);
}

template <typename K, typename V>
void Treemap<K, V>::Remove_P(std::unique_ptr<Node> &n, const K &key) {
    if (!n) {
        // If key does not exist in tree
        throw std::invalid_argument("Invalid key");
    }

    if (key < n -> key) {
        // If key is less than key at current node, go left
        Remove_P(n -> left, key);
    } else if (key > n -> key) {
        // If key is more than key at current node, go right
        Remove_P(n -> right, key);
    } else {
        // Key found
        if (n -> left.get() && n -> right.get()) {
            // If node has both children
            n -> key = (Min(n -> right.get())) -> key;
            // Replace key and value with min of right subtree
            n -> value = (Min(n -> right.get())) -> value;
            // Reduce the size
            size--;
        } else {
            // If only one child, replace with that
            // or nullptr if no child
            n = std::move((n -> left) ? n -> left : n -> right);
            // Reduce the size
            size--;
        }
    }
}

template <typename K, typename V>
void Treemap<K, V>::Remove(const K &key) {
    // Calls helper method
    Remove_P(root, key);
}

template <typename K, typename V>
const K& Treemap<K, V>::FloorKey(const K &key) {
    // If given key < minimum key
    if (key < MinKey()) {
        // Since no key can be less than minimum key,
        // the key would be invalid
        throw std::invalid_argument("Invalid key\n");
    }
    if (Empty()) {  // If tree is empty
        // Then there are no keys to compare
        throw std::logic_error("Empty tree\n");
    }
    // If given key == minimum key
    if (key == MinKey()) {
        // Return minimum key as there'll be nothing less than that
        return MinKey();
    }
    // Temporary variable
    K *temp;
    // Assign demo node to be root
    Node* n = root.get();

    while (n) {
        // If key at node is equal to key
        if (n -> key == key) {
            // Return key at node
            return n -> key;
        } else if (key < n -> key) {  // If key is less than node key
            // Check left subtree
            n = n -> left.get();
        } else if (key > n -> key) {  // If key is more than node key
            // Save a reference to that node
            temp = &n -> key;
            // Check right subtree
            n = n -> right.get();
        }
    }
    // Return value of temp variable if this point is reached
    return *temp;
}

template <typename K, typename V>
const K& Treemap<K, V>::CeilKey(const K &key) {
    // If given key > maximum key
    if (key > MaxKey()) {
        // Since no key can be more than maximum key,
        // the key would be invalid
        throw std::invalid_argument("Invalid key\n");
    }
    if (Empty()) {  // If tree is empty
        // Then there are no keys to compare
        throw std::logic_error("Empty tree\n");
    }
    // If given key == maximum key
    if (key == MaxKey()) {
        // Return maximum key as there'll be nothing more than that
        return MaxKey();
    }
    // Temporary variable
    K *temp;
    // Assign demo node to be root
    Node* n = root.get();

    while (n) {
        // If key at node is equal to key
        if (n -> key == key) {
            // Return key at node
            return n -> key;
        } else if (key > n -> key) {  // If key is more than node key
            // Check right subtree
            n = n -> right.get();
        } else if (key < n -> key) {  // If key is less than node key
            // Save a reference to that node
            temp = &n -> key;
            // Check left subtree
            n = n -> left.get();
        }
    }
    // Return value of temp variable if this point is reached
    return *temp;
}

#endif  // TREEMAP_H_
