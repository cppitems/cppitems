1 // item status
# Exercise EX2.2

The exercise is available as a git repository at:
https://github.com/cppitems/ex2.2

# Possible solutions
The below snippet is a solution using a deduction guide (and a regular template parameter for the constructor):
```pmans
//   Part 1:
//   Implement a type "Distribution" here with the following properties:
//   - "Distribution" should be a class template with a single template type
template <typename T> struct Distribution {
  //   - two public member variables "mean" and "stddev"
  T mean;
  T stddev;
  //   - public member typedef "value_type" which aliases the template parameter
  using value_type = T;
  //   - two constructors:
  //     - Distribution(T mean, T stddev) which sets the two member variables
  Distribution(T mean, T stddev) : mean(mean), stddev(stddev) {}
  //     - Distribution(const std::vector<T> &data) which calculates
  //       the two member variables from a Distribution of data
  //       - Part 2: make this constructor work with other containers and 
  template </*b*/ typename CTNR /*x*/> Distribution(const /*b4*/ CTNR &data) : mean{0}, stddev{0} {
    // mean
    for (const auto &item : data) { // used to support containers w/o random access
      //   mean += item; // removed to support SpaceVector
      mean = mean + item;
    }
    // mean /= data.size(); // removed to support SpaceVector
    mean = mean / data.size();
    // stddev
    for (const auto &item : data) {
      //  stddev += std::pow(item - mean, 2); // removed to support SpaceVector
      stddev = stddev + (item - mean) * (item - mean);
    }
    // stddev /= data.size(); // removed to support SpaceVector
    stddev = stddev / data.size();
    stddev = sqrt(stddev);
  }
};

// deduction guid
template <typename CTNR>
Distribution(const CTNR &data) -> Distribution<typename CTNR::value_type>;
```

An alternative (w/o the need for a deduction guide) is to introduce a template template parameter for the constructor. Note that this restricts the deduction to types which actually **are**  a template and take the element type as first template argument (this is not the case in the solution above):

```pmans
template <typename T> struct Distribution {
  ...
  // alternative to the deduction guide: use a template-template parameter
  template </*b*/ template<typename ... > class CTNR  /*x*/> Distribution(const /*b*/ CTNR<T> /*x*/ &data);
  ...
}
```