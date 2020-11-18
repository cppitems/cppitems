#include "timer.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>
#include <iostream>

template <typename T> void func1(std::vector<T> &a) {
  std::transform(std::begin(a), std::end(a), std::begin(a),
                 [](const T &item) { return std::sin(item); });
}

template <typename T> void func2(const std::vector<T> &a, std::vector<T> &res) {
  std::transform(std::begin(a), std::end(a), std::begin(res),
                 [](const T &item) { return std::sin(std::sin(item)); });
}

template <typename T> void func3(const std::vector<T> &a, std::vector<T> &res) {
  std::transform(a.begin(), a.end(), res.begin(), res.begin(),
                 [](const T &a, const T &b) { return sin(a) + sin(b); });
}

template <typename T>
void func4(const std::vector<T> &a, const std::vector<T> &b,
           std::vector<T> &res) {
  std::transform(a.begin(), a.end(), b.begin(), res.begin(),
                 [](const T &a, const T &b) { return sin(a) + sin(b); });
}

template <typename CTNR> void print(const CTNR &ctnr) {
  using value_type = typename CTNR::value_type;
  std::for_each(ctnr.begin(), ctnr.end(),
                [](const value_type &item) { std::cout << item << ", "; });
  std::cout << std::endl;
};

int main() {

  const std::size_t size = 5'000'000;
  {
    std::cout << "using T = double;" << std::endl;
    using T = double;

    std::vector<T> vec1(size, 1.0);
    std::vector<T> vec2(size, 2.0);
    std::vector<T> vec3(size, 3.0);

    // regular calls
    func1<T>(vec1);
    func2<T>(vec1, vec2);
    func3<T>(vec1, vec2);
    func4<T>(std::vector<T>(size, 1.0), vec2, vec3);

    // using 'track_time' from 'timer.h'
    track_time(func1<T>, vec1);
    track_time(func2<T>, vec1, vec2);
    track_time(func3<T>, vec1, vec2);
    track_time(func4<T>, std::vector<T>(size, 1.0), vec2, vec3);
  }

  {
    std::cout << "using T = float;" << std::endl;
    using T = float;
    std::vector<T> vec1(size, 1.0);
    std::vector<T> vec2(size, 2.0);
    std::vector<T> vec3(size, 3.0);

    // regular calls
    func1<T>(vec1);
    func2<T>(vec1, vec2);
    func3<T>(vec1, vec2);
    func4<T>(std::vector<T>(size, 1.0), vec2, vec3);
    // using 'track_time' from 'timer.h'
    track_time(func1<T>, vec1);
    track_time(func2<T>, vec1, vec2);
    track_time(func3<T>, vec1, vec2);
    track_time(func4<T>, std::vector<T>(size, 1.0), vec2, vec3);
  }
  {
    std::cout << "lambda function object" << std::endl;
    using T = double;
    auto lambda_fill = [](auto &ctnr) {
      std::fill(ctnr.begin(), ctnr.end(), T{1});
    };
    std::vector<T> vec(size);
    lambda_fill(vec);
    track_time(lambda_fill, vec);
  }
}