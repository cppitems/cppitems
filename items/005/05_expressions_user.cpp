int main() {
  {
    struct Widget {
      int i;
    };
    using Type = Widget;
    Type a{};
    Type b{};
    Type c{};
    a = b + c + c;                                      // (1)
    a = b + c * c;                                      // (2)
    a = 2 + c + 1 / 2;                                // (3)
    a = 2.5 + c + a;                                  // (4)
    a + b + c;                                          // (5)
    auto lambda = [](Type a, Type b) { return a - b; }; // (6)
    a = lambda(a, c) + b;                               // (7)
  }
}