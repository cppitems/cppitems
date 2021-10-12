#include "../include/square.hpp"

// this provides the definition (actual functionality in the function body)
int square(const int a) {
  return a * a;
}

void use_square(){
    auto res = square(5);
}

// clang++ -c lib/square.cpp -o square.o