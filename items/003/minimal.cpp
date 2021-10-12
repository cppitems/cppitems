#include <iostream> // copy paste all content of stdlib header here
#include "include/square.hpp" // copy past of all code from this header

int main() {
  const int b = square(13);
  std::cout << b << std::endl;
  return 0;
}

// 1a) clang++ minimal.cpp -c 
// 1b) clang++ minimal.cpp -c -o minimal.o
// 2) clang++ minimal.o -o minimal.exe

// with two objects
// 1) compile square.o (has "T" type for square function)
// clang++ -c lib/square.cpp
// 2) compile minimal.o (has "U" type for square function)
// clang++ -c minimal.cpp
// 3) linking both objects to construct an executable: checks if all symbols are available
// clang++ square.o minimal.o -o minimal_from_two_objects