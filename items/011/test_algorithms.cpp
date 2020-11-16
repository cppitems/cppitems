#include "algorithms.h"
#include <iomanip>
#include <iostream>
#include <iterator>
#include <vector>

int main() {

  using value_type = int;
  using Vector = std::vector<value_type>;

  const std::size_t size = 10;
  const value_type start = 10;
  const value_type scale = 2;
  Vector vec(size);

  auto condition = [](const value_type &element) -> bool {
    return element < value_type{(start + size / 2) * scale};
  };

  populate_with_value(vec, value_type{1});
  print("populate_with_value", vec);

  populate_with_sequence(vec, start);
  print("populate_with_sequence", vec);

  multiply_with_value(vec, scale);
  print("multiply_with_value", vec);

  sort_descending(vec);
  print("sort_descending", vec);

  auto count = count_fullfills_cond(vec, condition);
  const value_type expected_count = size / 2;
  std::cout << std::setw(25) << "count_fullfills_cond: " << count
            << ", expected: " << expected_count << std::endl;

  Vector vec_copy = vec;
  auto iter = vec_copy.begin();
  std::advance(iter, vec_copy.size() / 2);
  *iter += value_type{1};
  print("modified copy", vec_copy);

  auto equal = check_are_equal(vec, vec_copy);
  std::cout << std::setw(25) << "check_are_equal: " << equal
            << ", expected: " << false << std::endl;

  auto sum = sum_of_elements(vec);
  const value_type expected_sum =
      (((size * size + size) / 2) + size * (start - 1)) * scale;
  std::cout << std::setw(25) << "sum: " << sum << ", expected: " << expected_sum
            << std::endl;

  multiply_with_value(vec, value_type{-1});
  print("multiply with -1", vec);
  auto abssum = abssum_of_elements(vec);
  const value_type expected_abssum =
      (((size * size + size) / 2) + size * (start - 1)) * scale;
  std::cout << std::setw(25) << "abssum: " << abssum
            << ", expected: " << expected_abssum << std::endl;
}
