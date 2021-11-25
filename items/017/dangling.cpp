int main() {
  auto lambda = []() {
    int x{};
    return [&x]() { return x + 5; };
  };
  auto res = lambda();
}