9 // item status
# (EX1.2 preliminary version)

- given is the benchmark in `benchmarking_constructors.cpp`
- replace `std::vector<double>` with your own type that does not use the stdlib (i.e., manual implemenation of copy/move constructor is required)


## Reference benchmark
As reference, a benchmark is provided in `benchmarking_constructors.cpp`. It analogously measures the run time of
```pmans
    Widget var = Widget(size, std::rand());
    Widget w = /*f*/ std::move /*x*/(var) ; // invoking implicit move construction
```
and
```pmans
    Widget var = Widget(size, std::rand());
    Widget w = var; // invoking implicit copy construction
```    
for the implicitly defined copy/move constructors. They are explicitly defaulted to reveal the similarity of their prototypes (i.e., overloading using different reference types) to your initialization functions: 
```pmans
struct Widget {
  std::vector<double> vec;
  Widget(size_t size, double init) : vec(size, init) {}
  Widget(/*b*/ const Widget & /*x*/) = default; // implicit copy constructor
  Widget(/*b*/ Widget && /*x*/) = default;      // implicit move constructor
};
```