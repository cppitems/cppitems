#include <algorithm>
#include <cmath>
#include <iterator>
#include <vector>

int main() {

  using Container = std::vector<double>;
  Container vec(10);
  Container::value_type value;

  // "classic for-loop with index"
  for (std::size_t i = 0; i < vec.size(); ++i) {
    value = vec[i]; // access value using operator[index]
  }

  // classic for-loop with iterators
  for (auto iter = std::begin(vec); iter != std::end(vec); ++iter) {
    value = *iter; // access value using dereferenced iterator
  }

  { // range based for-loop (requires and used iterators under the hood)
    for (auto &item : vec) {
      item = value; // direct access to value
    }
    { // this is how it looks "under the hood"
      auto begin = vec.begin();
      auto end = vec.end();
      for (; begin != end; ++begin) {
        double &item = *begin;
        item = value;
      }
    }
  }

  { // obtaining iterators using <iterator>
    [[maybe_unused]] auto iter_begin = std::begin(vec); // [[maybe_unused]] avoids compiler warning 
    [[maybe_unused]] auto iter_end = std::end(vec);
  }

  { // obtaining iterators from member function
    [[maybe_unused]] auto iter_begin = vec.begin();
    [[maybe_unused]] auto iter_end = vec.end();
  }

  { // the end element is "one past the last element"
    auto iter_end = vec.end();
    value = *iter_end; // access out-of-bounds
    if (iter_end != vec.end()) {
      value = *iter_end; // guarded access
    }
  }

  { // apply a transformation (sin) to vector and store to new vector
    Container newvec(10);
    std::transform(
        vec.begin(), vec.end(), newvec.begin(),
        [](const Container::value_type &element) { return std::sin(element); });
  }
}