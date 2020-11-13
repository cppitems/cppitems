#include <iostream>

int main() {

  struct Widget {
    int /*b1*/ m;
    auto member() {
      return [/*b1*/ &]() { return /*b7*/ this->m; }; // (1) 'this' is captured by reference
    //   return [/*b1*/ = ]() {return /*b1*/ m; };       // (2) 'this' is still captured by reference
    }
  };
  auto func = []() {return Widget{9}.member(); };  // temporary 'Widget{9}' is destroyed after this line
  auto lambda = func();                            // lambda hols a dangling reference to the temporary
  std::cout << lambda() << std::endl;              // printing undefined value
}