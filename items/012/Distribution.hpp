#include <cmath>
#include <iostream>
#include <vector>

template <typename T> struct Distribution {
  using value_type = T;
  T mean;
  T stddev; 

  Distribution(T mean, T stddev) : mean(mean), stddev(stddev){};

  Distribution(std::vector<T> &dataset) : mean(0), stddev(0) {
    // (Task 1) missing implementation which calculates
    //  mean and stddev from 'dataset'
  }

  // (Task 2) missing functionality to 
  // construct a Distribution from other container types
};
