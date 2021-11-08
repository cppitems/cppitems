#include <utility>

struct Widget {
  int m;
};

template <typename T> struct DebugType { using T::notexisting; };
// usage: DebugType<decltype(...)> error;

template <typename AUTO> void func(const AUTO &arg) {
  // DebugType<AUTO> error;
  DebugType<decltype(arg)> error;
}

int main() {

//   {
//     Widget expr{1};
//     func(expr);               // func<Widget>(const Widget &arg) for lvalue
//     const auto &arg = (expr); // Widget &arg = ...
//   }
//   {
//     Widget a{1};
//     const Widget &expr = a;   // lvalue reference to const
//     func(expr);               // func<Widget>(const Widget &arg) for const lvalue
//     const auto &arg = (expr); // const Widget &arg = ...
//   }
//   {
//     Widget expr{};  
//     func(std::move(std::as_const(expr)));     // func<Widget>(const Widget &arg) for rvalue
//     const auto &arg = (Widget{}); // const Widget &arg = ...
//   }
//   {
//     Widget a{1};
//     Widget *expr = &a;        // ptr
//     func(expr);               // func<Widget *>(Widget *const &arg) for ptr
//     const auto &arg = (expr); // Widget *const &arg = ...
//   }
//   {
//     Widget a{1};
//     const Widget *expr = &a; // ptr to const
//     func(expr); // func<const Widget *>(const Widget *const &arg) for ptr to const
//     const auto &arg = (expr); // const Widget *const &arg = ...
//   }
//   {
//     Widget a{1};
//     const Widget *const expr = &a; // const ptr to const
//     func(expr); // func<const Widget *const>(const Widget *const &arg) for const ptr to const
//     const auto &arg = (expr); // const Widget *const & ar = ...
//   }
}