// testing custom closer as deleter
#include "../../Widget.h"
#include "../../unique_ptr.hpp"
#include <cassert>
#include <iostream>
#include <type_traits>
#include <utility>
#include <cstdio>


int count_closer_calls = 0;

void parse(FILE *handle){
    fprintf(handle,"%s", "TestF");
};

auto open(const char *name, const char *mode) {
  auto closer = [](FILE *handle) {
    fclose(handle);
    ++count_closer_calls;
  };
  return unique_ptr(fopen(name, mode), closer);
};

int main() {
  std::cout << "TestF_deleter_file: ";

  { // custom "closer" as deleter
    auto filename = "data.json";
    auto mode = "ab+";
    auto file = open(filename, mode);
    if (file != nullptr) {
      parse(file.get());
    }
  }
  assert(count_closer_calls == 1);

  // reaching here is success
  std::cout << "[ SUCCESS ]" << std::endl;

  return 0;
}