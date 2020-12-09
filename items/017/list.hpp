#pragma once
#include <cstddef>
#include <iterator>
#include <utility>

template <class T> class list {
public:
  using value_type = T;
  using size_type = std::size_t;

  // struct describing single Nodes of the list
  struct Node {
    T value;
    Node *next = nullptr;
    Node *previous = nullptr;
    Node() {}
    Node(const T &val) : value(val) {}
  };

  // START IMPLEMENTATION --------------------------------
  // Your implementation of the iterator should be entirely
  // inside of these two comment markers. All other parts
  // of the file should NOT be changed.

  // END IMPLEMENTATION ----------------------------------

private:
  Node *dataHead = nullptr;   // first element in list
  Node *dataEnd = nullptr;    // last element in list
  Node *pastTheEnd = nullptr; // past the end element because --end() must work
  size_type dataSize = 0;

public:
  list() {}
  list(size_type count, const T &value) {
    for (unsigned i = 0; i < count; ++i) {
      push_back(value);
    }
  }

  list(const list &other) { *this = other; }

  list(list &&other) { *this = std::move(other); }

  list &operator=(const list &other) {
    clear();
    if (other.dataHead != nullptr) {
      for (auto &it : other) {
        push_back(it);
      }
    }
    return *this;
  }

  list &operator=(list &&other) {
    clear();
    dataHead = other.dataHead;
    dataSize = other.dataSize;
    other.dataHead = nullptr;
    other.dataSize = 0;
    return *this;
  }

  bool operator==(const list &other) const {
    if (dataSize != other.dataSize) {
      return false;
    }
    auto it = begin();
    for (auto &otherIt : *this) {
      if (*it != otherIt) {
        return false;
      }
      ++it;
    }
    return true;
  }

  bool operator!=(const list &other) const { return !(*this == other); }

  ~list() { clear(); }

  size_type size() { return dataSize; }
  Node *data() { return dataHead; }
  value_type &front() { return dataHead->value; }
  value_type &back() { return dataEnd->value; }

  void push_back(const value_type &value) {
    if (dataSize == 0) {
      dataHead = new Node(value);
      dataEnd = dataHead;
      pastTheEnd = new Node;
    } else {
      dataEnd->next = new Node(value);
      dataEnd->next->previous = dataEnd;
      dataEnd = dataEnd->next;
    }
    pastTheEnd->previous = dataEnd;
    dataEnd->next = pastTheEnd;
    ++dataSize;
  }

  void clear() {
    if (dataHead != nullptr) {
      Node *currentNode = dataHead;
      Node *nextNode;
      while (currentNode != nullptr) {
        nextNode = currentNode->next;
        delete currentNode;
        currentNode = nextNode;
      }
    }

    dataSize = 0;
    dataHead = nullptr;
    dataEnd = nullptr;
    pastTheEnd = nullptr;
  }
};