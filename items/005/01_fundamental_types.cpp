#include <iostream>
int main() {
  using Type = int;
  Type b;                               // (1)
  Type a = b;                           // (2)
  auto c = a + b + b++;                 // (3)
  auto d = 2 + 2 * Type{1} / Type(2.0); // (4)
  auto e = 2.5 + Type{1} / Type{2} * 5; // (5)
}