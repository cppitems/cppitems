struct Widget {
  int i;
  // operator overloads as member functions
  // invocation: Widget c = a + b;
  // invocation: Widget c = a.member(b);
  auto member(const Widget &other) const {
      // "a" "*this" is the object
      // "b" "other" is just an ordinary argument  
      // if i just type "i" in this context?
      // -> lookup first happens in "this"
      return Widget{this->i + other.i}; 
  }
  auto operator-(const Widget &other) const { return Widget{i - other.i}; }
  auto operator*(const Widget &other) const { return Widget{i * other.i}; }
  Widget(int i) : i(i){}; // converting constructor for 'int' to 'Widget'
  Widget()
      : i{} {}; // explicit default constructor (implicit version is deleted due
                // to existence of user-defined constructor)
};

// operator overload as free function
// this template would be considerend for any type T, but will fail to be
// instantiated for incompatible types. how to restrict T to a strict subset of
// types which are known to be compatible, e.g. integral types?
template <typename T> auto operator+(const T &maywork, const Widget &widget) {
  return T{maywork + widget.i}; 
};

int main() {
  {
    // struct Widget {
    //   int i;
    // };
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