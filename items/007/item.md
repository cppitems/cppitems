1 // item status
# Exercise EX1.1

The exercise is available as a git repository at:
https://github.com/cppitems/ex1.1

# Possible solution

```pmans
Widget rvref::init(Vector &&vec) {
  return Widget{/*b*/ std::move( /*x*/vec/*b1*/ )}; 
  // add std::move to trigger a move of 'vec' instead of copy
  // this avoidance of a copy should be reflected in the benchmarks results
}

Widget lvref::init(Vector &vec) {
  return Widget{vec}; 
  // cannot apply std::move, caller would not expect his; 
  // no improvment in the benchmark resulsts expected
}

Widget value::init(Vector vec) {
  return Widget{/*b*/ std::move( /*x*/vec/*b1*/ )}; 
  // 'vec' not moved automatically without a std::move; although there are already proposals to do this in future c++ versions
  // std::move avoids a second copy; this should be eflect in the benchmark results
}
```

### Benchmark results before change
```bash
rvref::init(Vector&&) called as rvref::init(std::move(vec))  1.49e-02 s
lvref::init(Vector&)  called as lvref::init(vec)             1.48e-02 s
value::init(Vector)   called as value::init(std::move(vec))  1.64e-02 s
value::init(Vector)   called as value::init(vec)             2.89e-02 s
```

### Benchmark results after change
```bash
rvref::init(Vector&&) called as rvref::init(std::move(vec))  1.30e-03 s -> much faster (no copy at all now)
lvref::init(Vector&)  called as lvref::init(vec)             1.47e-02 s -> no changes (still one copy)
value::init(Vector)   called as value::init(std::move(vec))  1.30e-03 s -> much faster (no copy at all now)
value::init(Vector)   called as value::init(vec)             1.43e-02 s -> 50% faster (only one instead of two copies)
```


