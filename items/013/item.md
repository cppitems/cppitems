1 // item status
# Exercise EX2.1

The exercise is available as a git repository at:
https://github.com/cppitems/ex2.1

# Possible solutions
In the below solution a template parameter pack (1) was introduced and "unpacked" at the required symantic embeddings (2) and (3): 
```pmans
// template<class CALLABLE, /*b*/ class Arg /*x*/)> 
template <class CALLABLE, /*b*/ class... Args /*x*/> // (1)
// double measureTime(CALLABLE &&callable, /*b*/ Arg&& a, Arg&& b /*x*/) {
double measureTime(CALLABLE &&callable, /*b*/ Args&& ...args /*x*/) { // (2)
  using Clock = std::chrono::high_resolution_clock;
  auto start = Clock::now(); // start time measurement

  // forward the two arguments to the callable `call`
  // callable(/*b*/ std::forward<Arg>(a), std::forward<Arg>(b) /*x*/);
  callable(/*b*/ std::forward<Args>(args)... /*x*/); // (3)

  auto stop = Clock::now(); // stop time measurement
  return std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
}

```
