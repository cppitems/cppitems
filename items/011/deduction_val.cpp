#include <utility>

struct Widget {
  int m;
};

template <typename T> struct DebugType { using T::notexisting; };
// usage: DebugType<decltype(...)> error;

template <typename AUTO> void func(AUTO arg) {
  // DebugType<AUTO> error;
  DebugType<decltype(arg)> error;
}

int main() {

//   {
//     Widget expr{1};    // lvalue
//     func(expr);        // func<Widget>(Widget arg)
//     auto arg = (expr); // Widget arg = ...
//   }
//   {
//     Widget a{1};
//     Widget &expr = a;  // lvalue reference
//     func(expr);        // func<Widget>(Widget arg)
//     auto arg = (expr); // Widget arg = ...
//   }
//   {
//     Widget a{1};
//     const Widget &expr = a; // lvalue reference to const
//     func(expr);             // func<Widget>(Widget arg)
//     auto arg = (expr);      // Widget arg = ...
//   }
//   {
//     // rvalue
//     func(Widget{1});                 // func<Widget>(Widget arg)
//     auto arg = std::move(Widget{1}); // Widget arg = ...
//   }
//   {
//     Widget expr{1};             // rvalue reference
//     func(std::move(expr));      // func<Widget>(Widget arg)
//     auto arg = std::move(expr); // Widget arg = ...
//   }
//   {
//     Widget a{1};
//     Widget *expr = &a; // ptr
//     func(expr);        // func<Widget *>(Widget *arg)
//     auto arg = (expr); // Widget *arg = ...
//   }
//   {
//     Widget a{1};
//     using PointerToConst = const Widget *;
//     PointerToConst expr = &a; // ptr to const
//     func(expr);               // func<const Widget *>(const Widget *arg)
//     auto arg = (expr);        // const Widget *arg = ...
//   }
//   {
//     Widget a{1};
//     using PointerToConst = const Widget *;
//     using ConstPointerToConst = const PointerToConst;
//     ConstPointerToConst expr = &a; // const ptr to const
//     func(expr);                    // func<const Widget *>(const Widget *arg)
//     auto arg = (expr);             // const Widget *arg = ...
//   }
}