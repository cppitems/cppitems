1 // item status
# Exercise EX2.3

The exercise is available as a git repository at:
https://github.com/cppitems/ex2.3

# Possible solutions
... will be announced here after the discussions
```pmans
// populate container with constant value
template <typename C>
auto populate_with_value(C &ctnr, typename C::value_type value) {
  //   for (std::size_t i = 0; i < ctnr.size(); ++i) {
  //     ctnr[i] = value;
  //   }
  /*b*/ std::fill /*x*/(ctnr.begin(), ctnr.end(), value);
}

// populate container with increasing sequence of values
template <typename C>
auto populate_with_sequence(C &ctnr, typename C::value_type start) {
  //   for (std::size_t i = 0; i < ctnr.size(); ++i) {
  //     ctnr[i] = start++;
  //   }
  /*b*/ std::generate /*x*/(ctnr.begin(), ctnr.end(), [&start]() { return start++; });
}

// multiply each element in container by value
template <typename C>
auto multiply_with_value(C &ctnr, typename C::value_type value) {
  //   for (std::size_t i = 0; i < ctnr.size(); ++i) {
  //     ctnr[i] *= value;
  //   }
  using T = typename C::value_type;
  /*b*/ std::for_each /*x*/(ctnr.begin(), ctnr.end(), [&value](T &item) { item *= value; });
}

// reverse order of elements
template <typename C> auto reverse_order(C &ctnr) {
  //   std::size_t lastElement = ctnr.size() - 1;
  //   for (std::size_t i = 0; i < ctnr.size() / 2; ++i) {
  //     std::swap(ctnr[i], ctnr[lastElement - i]);
  //   }
  /*b*/ std::reverse /*x*/(ctnr.begin(), ctnr.end());
}

// check how many elements fulfill a condition
template <typename C, typename UnaryPredicate>
auto count_fulfills_cond(C &ctnr, UnaryPredicate condition) {
  //   std::size_t count = 0;
  //   for (std::size_t i = 0; i < ctnr.size(); ++i) {
  //     if (condition(ctnr[i])) {
  //       ++count;
  //     }
  //   }
  //   return count;
  return /*b*/ std::count_if /*x*/(ctnr.begin(), ctnr.end(), condition);
}

// check if two containers are equal in length and content
template <typename C>
auto first_n_equal(const C &a, const C &b, std::size_t n) {
  //   // if containers are not at least n long,
  //   // the first n elements cannot be equal
  if (a.size() < n || b.size() < n) {
    return false;
  }

  //   for (std::size_t i = 0; i < n; ++i) {
  //     // if elements do not match, the ranges are not equal
  //     if (a[i] != b[i]) {
  //       return false;
  //     }
  //   }
  //   return true;
  return /*b*/ std::equal /*x*/(a.begin(), std::next(a.begin(), n), b.begin());
}

// sum all elements in the container
template <typename C> auto sum_of_elements(const C &ctnr) {
  //   typename C::value_type sum{};
  //   for (std::size_t i = 0; i < ctnr.size(); ++i) {
  //     sum += ctnr[i];
  //   }
  //   return sum;
  using T = typename C::value_type;
  return /*b*/ std::accumulate /*x*/(ctnr.begin(), ctnr.end(), T{});
}

// sum of absolute values of elements in the container
template <typename C> auto abssum_of_elements(C &ctnr) {
  //   typename C::value_type sum{};
  //   for (std::size_t i = 0; i < ctnr.size(); ++i) {
  //     sum += std::abs(ctnr[i]);
  //   }
  //   return sum;
  using T = typename C::value_type;
  return /*b*/ std::accumulate /*x*/(
      ctnr.begin(), ctnr.end(), T{},
      [](const T &accu, const T &current) { return accu + std::abs(current); });
}

// print container elements
template <typename C> auto print(std::ostream &os, const C &ctnr) {
  //   os << "[";
  //   std::size_t lastElement = ctnr.size() - 1;
  //   for (std::size_t i = 0; i < lastElement; ++i) {
  //     os << ctnr[i] << ", ";
  //   }
  //   os << ctnr[lastElement] << "]" << std::endl;
  os << "[";
  using T = typename C::value_type;
  /*b*/ std::for_each /*x*/(ctnr.begin(), --ctnr.end(),
                [&os](const T &item) { os << item << ", "; });
  os << *(--ctnr.end()) << "]" << std::endl;
}
```
