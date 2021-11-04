#include <utility>

struct Widget {
  int m;
};

template <typename T> struct DebugType { using T::notexisting; };
// usage: DebugType<decltype(...)> error;

template <typename AUTO> void func(AUTO &arg) {
  // DebugType<AUTO> error;
  DebugType<decltype(arg)> error;
}

int main() {

//   {
//     Widget expr{1};     // lvalue
//     func(expr);         // func<Widget>(Widget &arg)
//     auto &arg = (expr); // Widget &arg = ...
//   }
//   {
//     Widget a{1};
//     Widget &expr = a;   // lvalue reference
//     func(expr);         // func<Widget>(Widget &arg)
//     auto &arg = (expr); // Widget &arg = ...
//   }
//   {
//     Widget a{1};
//     const Widget &expr = a; // lvalue reference to const
//     func(expr);             // func<const Widget>(const Widget &arg)
//     auto &arg = (expr);     // const Widget &arg = ...
//   }
//   {
//     Widget a{1};
//     Widget *expr = &a;  // ptr
//     func(expr);         // func<Widget *>(Widget *&arg)
//     auto &arg = (expr); // Widget *&arg = ...
//   }
//   {
//     Widget a{1};
//     const Widget *expr = &a; // ptr to const
//     func(expr);              // func<const Widget *>(const Widget *&arg)
//     auto &arg = (expr);      // const Widget *&arg = ...
//   }
//   {
//     Widget a{1};
//     const Widget *const expr = &a; // const ptr to const
//     func(expr);         // func<const Widget *const>(const Widget *const &arg)
//     auto &arg = (expr); // const Widget *const & ar = ...
//   }
}