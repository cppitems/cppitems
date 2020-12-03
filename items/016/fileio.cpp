#include <cstdio>
#include <iostream>
#include <memory>

void parse(FILE *handle){};

// int main() {
//   auto filename = "data.json";
//   auto mode = "r";
//   FILE *handle = std::fopen(filename, mode);
//   if (handle) {
//     parse(handle);
//     fclose(handle);
//   }
// }

auto open(const char *name, const char *mode) {
  auto closer = [](FILE *handle) { fclose(handle); };
  return std::unique_ptr<FILE, decltype(closer)>(fopen(name, mode), closer);
};

int main() {
  auto filename = "data.json";
  auto mode = "r";
  auto file = open(filename, mode);
  if (file) {
    parse(file.get());
  }
}
