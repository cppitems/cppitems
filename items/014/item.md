1 // item status
# Iterators
Using iterators (not only iterators from the stdlib, but any iterator-like concept) to operate on data stored in a data structure (i.e., a container) abstracts from details of the implementation:
If all usage scenarios are covered by appropriate iterators, acquiring detailed knowledge of the underlying data structure/container implementation is not required.

> So, is the iterator approach a good idea?

> Examples for interfaces with iterators (not stdlib)? 


Let's look at a simple example:
```pmans
struct /*b6*/ Widget {
  /*f*/ std::vector<double> /*x*/ /*b4*/ data;
  /*f*/ std::vector<bool> /*x*/ /*b6*/ active;
  Widget(std::size_t size) : data(size, 0.0), active(size, false) {}
  void set_value(std::size_t idx, double value) { data[idx] = value; }
  void set_status(std::size_t idx, bool status) { active[idx] = status; }
};
```
This `Widget` can be considered a data structure which holds values together with a flag indicating "activity" via a Boolean status for each value.
Let's assume the only usage scenario is to sweep over all active values. This could look like this:
```pmans
    Widget w(10);
    w.set_value(5, 60.0);
    w.set_status(5, true);

    // iterate over 'active' values in Widget
    for (std::size_t i = 0; i < w.data.size(); ++i) {
      if (/*f*/ w.active[i] /*x*/) {
        // read/write using '/*f*/ w.data[i] /*x*/'
      }
    }
```  
Although a simple example, it reveals that the user is required to digest the details of the implementation of `Widget`.
Let's assume a slight modification resulting in `Widget2`:
```pmans
struct /*b7*/ Widget2 {
  /*f*/ std::vector<std::pair<bool, double>> /*x*/ /*b4*/ data;
  Widget2(std::size_t size) : data(size, {0.0, false}) {}
  void set_value(std::size_t idx, double value) { data[idx].second = value; }
  void set_status(std::size_t idx, bool status) { data[idx].first = status; }
};
```
This change in the implementation details requires again to digest the details of the implementation to conduct the same task:
```pmans
    Widget2 w(10);
    w.set_value(5, 60.0);
    w.set_status(5, true);
    // iterate over 'active' values in Widget2
    for (std::size_t i = 0; i < w.data.size(); ++i) {
      if (/*f*/ w.data[i].first /*x*/) {
        // read/write using '/*f*/ w.data[i].second /*x*/ '
      }
    }
```
If an iterator for this use case "visit all active values" would be available, this could look like this (independent of the implementation details of `Widget`):
```pmans
    // iterate over 'active' values in Widget
    for (auto /*f4*/ iter = w./*b*/ begin_active() /*x*/; /*f4*/ iter /*b2*/ != w./*b*/ end_active() /*x*/; /*b2*/ ++/*f4*/ iter) {
      double &value = /*b1*/ */*f4*/ iter;
    }
```
> Describe apparent properties of the iterator object above and how this object is obtained!


## Implementation example: iterator for `Widget`
Without thinking too much about iterators defined in the stdlib, let's try to implement a `IteratorActive` for the `Widget` compatible to the snippet above.
The requirements for the `IteratorActive` are:
- start/begin iterator obtainable by-value from `Widget::begin_active()`
- one-past-the-end iterator obtainable by-value from `Widget::end_active()`
- has `operator!=` and `operator++` defined
- `operator*` results in a reference to the value allowing read/write access

Mapping these requirements to declarations looks like this (simply nesting the iterator class inside `Widget`): 
```pmans
struct Widget {
  std::vector<double> data;
  std::vector<bool> active;
  ...
  struct /*f*/ IteratorActive /*x*/ {
    double &/*b*/ operator* /*x*/();
    /*f*/ IteratorActive /*x*/ &/*b*/ operator++ /*x*/();
    bool /*b*/ operator!= /*x*/(const /*f*/ IteratorActive /*x*/ &other);
  /*f*/ IteratorActive /*x*/ /*b*/ begin_active /*x*/();
  /*f*/ IteratorActive /*x*/ /*b*/ end_active /*x*/();
};
```

Finally, an implementation of a lightweight iterator over active values might look like this:
```pmans
struct Widget {
  ...
  struct IteratorActive {
    Widget &ref;             // reference to associated object
    std::size_t pos;         // current state
    const std::size_t end;   // fixed after construction
    double &/*b*/ operator* /*x*/() { return ref.data[pos]; } // wrapping access 
    IteratorActive &/*b*/ operator++ /*x*/() { // increment to next active value
      while (++pos != end) {
        if (ref.active[pos])
          break;
      }
      return *this;
    }
    bool /*b*/ operator!= /*x*/(const IteratorActive &other) { // wrapping compare
      return this->pos != other.pos;
    }
  };
  IteratorActive /*b*/ begin_active /*x*/() {
    IteratorActive iter = {*this, 0, data.size()};
    return iter.ref.active[iter.pos] ? iter : ++iter; // increment if first value is not active
  }
  IteratorActive /*b*/ end_active /*x*/() {
    return IteratorActive{*this, data.size(), data.size()}; // return iterator in state "end"
  }
};
```

### Iterator invalidation 
Let's extend the `Widget` class with a `push_back` function which appends an entry (status and value):
```pmans
struct /*f6*/ Widget {
  std::vector<double> data;
  std::vector<bool> active;
  ...
  void /*b*/ push_back /*x*/(bool status, double value) {
    data.push_back(value);
    active.push_back(status);
  }
};
```
It is apparent that this new function can lead to a re-allocations of the dynamic memory owned by `data` and `active`. 

> Do such "push backs" have consequences for `IteratorActive` instances associated with a `Widget` object?

## Range-based for loop
Above we have implemented an iterator for the special use case "visit all active values in a `Widget`".
Let's resort to a simpler class `WidgetArray` which only holds a single field to explore requirements for a class object to be compatible a *range-based* `for`-loop:

```pmans
template <typename T, int N> 
struct /*f*/ WidgetArray /*x*/ {
  T data[N];}
};
// usage in range-based for loop
WidgetArray<double,10> array;
for (/*b*/ auto &item /*x*/ : /*f*/ array /*x*/){
  // statements using named variable /*f*/ item /*x*/;
  item = 7; // no dereferencing required
}
```
A range-based `for`-loop has the following pattern:
```pmans
for (/*b*/ declaration /*x*/ : /*f*/ object_with_begin_end /*x*/) {
    // statements using name variable from /*f*/ declaration /*x*/
}
```
is transformed by the compiler to
```pmans
{
  auto &&range = /*f*/ object_with_begin_end /*x*/;
  auto begin = /*f*/ range.begin() /*x*/; // fallback is begin(object_wo_begin_end) 
  auto end = /*f*/ range.end() /*x*/;     // fallback is end(object_wo_begin_end) 
  for ( ; begin != end; ++begin) {
    /*b*/ declaration /*x*/ = *begin;
    // statements using name variable from /*f*/ declaration /*x*/
  }
}
```
This reveals the requirements:
- the object is required to have member functions `being` and `end` (fallback are free functions `begin` and `end`)
- the return type of `begin` and `end` has to support `operator++` and `operator!=` 
- additionally, `operator*` is compatible with the declaration of the named variable 

If we would like to support *range-based* `for`-loop to access `WidgetArray` one option is to extend the class by appropriate `begin` and `end` member functions:
```pmans
template <typename T, int N> 
struct /*f*/ WidgetArray /*x*/ {
  T data[N];
  T */*b5*/ begin() { return data; } // iterator is T*
  T */*b3*/ end() { return data + N; } 
  // why no need to overload operators?
};
```
Alternatively, free functions can be used, too:
```pmans
template <typename T, int N> 
T */*b5*/ begin(WidgetArray<T, N> &array) {
  return array.data;
}
template <typename T, int N> 
T */*b3*/ end(WidgetArray<T, N> &array) {
  return array.data + N;
}
```
This is already sufficient for `WidgetArray` as a pointer is used directly as iterator: 
raw pointers full fill the requirements for `operator!=`, `operator++` (pointer arithmetic), and `operator*` (dereferencing).

Note: Observing the above, iterators can be though of as a "generalization of pointer arithmetic" for situations where the underlying implementation does not map to a plain array or other logic ("visit only active values") is desired.

> How would we have to adopt `Widget` and `IteratorActive` above to also work with a range-based for-loop?


## Stdlib iterators, containers, and algorithms 


> Is considering further iterators for `Widget` above, which cover use cases like the following a good idea?


Considering the thoughs above, it becomes clear why the standard library separated functionality into: 
- containers (provide access to iterators they support, i.e., iterators of a certain category)
- algorithms (using iterators exclusively, no direct coupling to data structures/containers)

In the following we will look at the iterator categories and and their relation to algorithms in the standard library.

### Iterator categories

The standard library uses iterators extensively: all containers support iterators.
Depending of the typically expected underlying implementation for each container in a standard library, a specific category of iterator is supported.
These are the five categories

- random-access-iterator: `std::array`, `std::vector`,  `std::deque`
```pmans
    auto iter = iter + std::rand() % vec.size(); // random access 
    *iter = 5; // writing 
    value = *iter; // reading 
    ++iter; // inc
    --iter; // dec
    iter == iter2; iter != iter2; // compare
    iter = iter2 // assignment/copy does result in two "usable" iterators
```
-  bidirectional iterator: `std::list`, `std::set` (const iter), `std::map` (const iter)
```pmans
    *iter = 5; // writing 
    value = *iter; // reading 
    ++iter; // inc
    --iter; // dec
    iter == iter2; iter != iter2; // compare 
    iter = iter2 // assignment/copy does result in two "usable" iterators     
```
- forward iterator: `std::forward_list`, `std::unordered_set` (const iter), `std::unordered_map` (const iter)
```pmans
    *iter = 5; // writing 
    value = *iter; // reading 
    ++iter; // inc
    iter == iter2; iter != iter2; // compare 
    iter = iter2 // assignment/copy does result in two "usable" iterators     
``` 
- input iterators: 
```pmans
    ++iter; // inc
    value = *iter; // reading 
    iter == iter2; iter != iter2; // compare  
    iter = iter2 
    // assignment/copy does is allowed but:
    // but only one of the two can be used further (the other then becomes invalid)
```
- output iterators: 
```pmans
    ++iter; // inc
    *iter = value; // writing
    // assignment/copy does is allowed but:
    // but only one of the two can be used further (the other then becomes invalid)    
```

> Does it make sense for a container to implement multiple categories of this hierarchy?


### Const iterators

Const iterators (iterators over const values) are available for all containers in the stdlib using `cbegin` and `cend`
```pmans
    std::vector<int> vec(100,1);
    const auto iter1 = vec.begin(); // const iterator object
    ++iter1;    // error
    *iter1 = 7; // ok
    auto iter2 = vec.cbegin();      //  iterator over const values
    ++iter2;    // ok
    *iter2 = 7; // error
```

### Algorithms
The algorithms in the standard library base their interfaces on iterators.
Always the lowest category in the hierarchy of iterator categories which is sufficient for the algorithms is selected for the interface.
This means not all containers support all algorithms.
Let's see a list of examples:

- `std::random_shuffle`, `std::sort` -> random access iterators; [random_shuffle](https://en.cppreference.com/w/cpp/algorithm/random_shuffle)
- `std::reverse_copy`, `std::next_permutation` -> bidirectional iterators [reverse_copy](https://en.cppreference.com/w/cpp/algorithm/reverse_copy)
- `std::fill`, `std::replace`,  `std::search` `std::find`  -> forward iterators [find](https://en.cppreference.com/w/cpp/algorithm/find)
- `std::transform` -> input and output iterators [transform](https://en.cppreference.com/w/cpp/algorithm/transform)
- `std::for_each` -> input iterators [for_each](https://en.cppreference.com/w/cpp/algorithm/for_each)
- `std::fill_n` -> output iterator [fill_n](https://en.cppreference.com/w/cpp/algorithm/fill_n)

> The links above point to the documention of th algorithms on cppreference.com . The documentation also contains a "possible implementation" which can be used to reason about the iterator category which is required for an algorithm, e.g. for 'fill_n'
>```pmans
>// possible implementations of 'std::fill_n' which requires an output iterator
>template<class OutputIt, class Size, class T>
>OutputIt fill_n(OutputIt first, Size count, const T& value)
>{
>    for (Size i = 0; i < count; i++) {
>        *first++ = value; // only 'increment' and 'assign' is used -> 'lowest' supporting category: output iterator
>    }
>    return first; // copy is allowed and unproblematic, as the local copy goes out of scope
>}
>```

> Can I implement stdlib compatible iterators for my class/container?


## Links

- Containers and iterators: https://en.cppreference.com/w/cpp/container
- Stdlib compatible iterators https://www.fluentcpp.com/2018/05/08/std-iterator-deprecated/
- Random access iterator: https://codereview.stackexchange.com/questions/202157/basic-iterator-supporting-vector-implementation
- Invalidation example: https://stackoverflow.com/questions/22969860/can-a-c-stdlist-iterator-move-past-the-end
- Range-based for-loop: https://en.cppreference.com/w/cpp/language/range-for
- Iterator categories: https://stdcxx.apache.org/doc/stdlibug/2-2.html
- Iterators for trees: https://www.cs.odu.edu/~zeil/cs361/latest/Public/treetraversal/index.html
- Container performance: http://john-ahlgren.blogspot.com/2013/10/stl-container-performance.html