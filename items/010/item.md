1 // item status
# Exercise EX1.3

The exercise is available as a git repository at:
https://github.com/cppitems/ex1.3

# Possible solution
```pmans
// copy constructor
List::List(const List &other) {
  // use copy assign implementation
  *this /*b1*/ = other; 
}

// move constructor
List::List(List &&other) {
  // use move assign implementation  
  *this /*b1*/ = /*b9*/ std::move(other); 
}

// copy assignment operator
List &List::operator=(const List &other) {

  // cleanup current list
  while (_size > 0) 
    pop_front();

  // treat first element
  auto srcIt = other._head;
  if (!_head && srcIt) {
    _head = new Node(srcIt->value);
    srcIt = srcIt->next;
    ++_size;
  }

  // rest of elements
  auto desIt = _head;
  while (srcIt) {
    desIt->next = new Node(srcIt->value);
    desIt = desIt->next;
    srcIt = srcIt->next;
    ++_size;
  }

  return *this;
}

// move assignment operator
List &List::operator=(List &&other) {
  // swap indvidual members
  std::swap(other._head, _head);
  std::swap(other._size, _size);
  return *this;
}
```
## Benchmark results
```bash
Copy:
Old data before  | New data after   | Old Data after
  0x55ceb22bd790 |   0x55ceb28d7f90 | 0x55ceb22bd790

Move:
Old data before  | New data after   | Old Data after
  0x55ceb1995b90 |   0x55ceb1995b90 | 0

Copy Assign:
Old data before  | New data after   | Old Data after
  0x55ceb1fb0390 |   0x55ceb28d8050 | 0x55ceb1fb0390

Move Assign:
Old data before  | New data after   | Old Data after
  0x55ceb28d7eb0 |   0x55ceb28d7eb0 | 0x55ceb1995b90


Runtimes of constructing list objects:

Copy constructor List(const List&)      called as List(list)                1.67e-03 s -> allocations
Move constructor List(List&&)           called as List(std::move(list))     7.98e-07 s -> swapping pointers only
Copy assignment operator=(const List&)  called as listB = listA             2.40e-03 s -> deallocation + allocation
Move assignment operator=(List&&)       called as listB = std::move(listA)  5.80e-08 s -> swapping pointers only
```