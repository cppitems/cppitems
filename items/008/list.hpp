#include <cstddef>
#include <utility>

template <class T> class List {
public:
  using value_type = T;
  using size_type = std::size_t;

  // nested class
  struct Node {
    value_type value;
    Node *next = nullptr;
    Node(const value_type &val) : value(val) {}
  }; 

private:
  Node *dataHead = nullptr; // root element
  size_type dataSize = 0; // current number of nodes in list

public:
  // default ctor
  List() {};
  // custom ctor
  List(size_type count, const value_type &value) : dataSize(count) {
    dataHead = new Node(value);
    Node *lastNode = dataHead; // root node
    for (size_type i = 1; i < dataSize; ++i) {
      Node *newNode = new Node(value);
      lastNode->next = newNode;
      lastNode = newNode;
    }
  }

  // you task it to implement the four currently deleted SMFs (1,2,3,4) below
  // according to the needs of this resource owning class

  List(const List &list) = delete; // (1) copy-ctor
  List(List &&list) = delete;   // (2) move-ctor
  List &operator=(const List &list) = delete;   // (3) copy-assignment
  List &operator=(List &&list) = delete;   // (4) move-assignment

  // dtor
  ~List() {
    Node *currentNode = dataHead;
    Node *nextNode;
    while (currentNode != nullptr) {
      nextNode = currentNode->next;
      delete currentNode;
      currentNode = nextNode;
    }
    dataSize = 0;
  }
  // access current number of nodes in list 
  size_type size() { return dataSize; }
  // obtain root element pointer
  Node *data() { return dataHead; }
  // access front element
  value_type &front() { return dataHead->value; }
  // push new list element at the front
  void push_front(const value_type &value) {
    Node *nextDataHead = new Node(value);
    nextDataHead->next = dataHead;
    dataHead = nextDataHead;
    ++dataSize;
  }
  // pop element at the front
  void pop_front() {
    if (dataHead != nullptr) {
      Node *oldDataHead = dataHead;
      dataHead = oldDataHead->next;
      delete oldDataHead;
      --dataSize;
    }
  }
};