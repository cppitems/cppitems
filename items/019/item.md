1 // item status
# Exercise EX3.1

The exercise is available as a git repository at:
https://github.com/cppitems/ex3.1

# Possible solution

**Note**: The implementation of `List` we handed out had a flaw (thanks for pointing out!): as the object used for the `root`-node was not dynamically allocated (but was referencing the plain member `sentinel`) all "swaps" led to an "entanglement" between the swapped objects via the `sentinel` member. The easiest correction is to also dynamically allocate the object for the `root`-node in all construction situations (ctor/cctor/mctor). Nevertheless, this had no influence on the tests we provided and the tasks in the assignments.

```pmans
template <class T>
class List { 
[...]
  // ------------ SOLUTION START
public:
  template <typename VALUE_TYPE> struct Iterator {

    using value_type = VALUE_TYPE;                             // (TestH)
    using iterator_category = std::bidirectional_iterator_tag; // (TestI)
    using difference_type = std::ptrdiff_t;                    // (TestI)
    using pointer = VALUE_TYPE *;                              // (TestI)
    using reference = VALUE_TYPE &;                            // (TestI)

    Node *node;

    Iterator operator++() { // increment (TestB)
      node = node->next;
      return *this;
    }
    Iterator operator--() { // decrement (TestC)
      node = node->prev;
      return *this;
    }
    VALUE_TYPE &operator*() { // dereference (TestA)
      return node->value;
    }
    VALUE_TYPE *operator->() { // member of pointer (TestE)
      return &node->value;
    }
    bool operator==(const Iterator &other) const { // compare (TestF)
      return node == other.node;
    }
    bool operator!=(const Iterator &other) const { // compare (TestF)
      return node != other.node;
    }
  };
  auto begin() { // iterator to start (TestA)
    return Iterator<T>{root->next};
  }
  auto end() { // iterator to end (TestD)
    return Iterator<T>{root};
  }
  auto begin() const { // iterator to start obtained for const List (TestH)
    return Iterator<const T>{root->next};
  }
  auto end() const { // iterator to end obtained for const List (TestH)
    return Iterator<const T>{root};
  }
  auto cbegin() const { // iterator over const values (TestH)
    return Iterator<const T>{root->next};
  }
  auto cend() const { // iterator over const values (TestH)
    return Iterator<const T>{root};
  }

  using iterator = Iterator<T>;             // (TestH)
  using const_iterator = Iterator<const T>; // (TestH)

  // ------------ SOLUTION END
};
```
