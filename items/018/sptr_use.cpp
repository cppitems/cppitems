#include <iostream>
#include <memory>

struct Widget {
  int m;
};

auto get_widget() {
  return std::make_shared<Widget>(); // (2) using public constructors indirectly
}

// int main() {
//   auto sp = get_widget();
//   std::cout << sp.use_count() << std::endl;
//   auto sp2 = sp; // copy ctor
//   std::cout << sp.use_count() << std::endl;
//   std::cout << sp2.use_count() << std::endl;
//   std::cout << (sp2 == sp) << std::endl;
//   {
//     auto sp3 = sp; // copy ctor
//     std::cout << sp.use_count() << std::endl;
//   }
//   std::cout << sp.use_count() << std::endl;
// }

// int main() {
//   // call get and use after sp is gone
//   Widget *ptr = nullptr;
//   {
//     auto sp = get_widget();
//     ptr = sp.get();
//   }
//   ptr->m = 5;
// }

// int main() {
//   // double managing same object
//   auto sp1 = get_widget();
//   auto sp2 = get_widget();
//   std::shared_ptr<Widget> sp3(sp1);
//   {
//     // safe?
//     std::shared_ptr<Widget> sp4(sp2.get());
//   }
// }

// struct Widget2 {
//   int m;
//   auto mfunc() { return std::shared_ptr<Widget2>(this); }
// };

// auto get_widget2() {
//   return std::make_shared<Widget2>();
// }

// int main() {
//   // return from inside
//   auto sp = get_widget2();
//   auto sp2 = sp->mfunc();
// }

// struct Widget3 : std::enable_shared_from_this<Widget3> {
//   int m;
//   auto mfunc() { return shared_from_this(); }
// };

// auto get_widget3() { return std::make_shared<Widget3>(); }

// int main() {
//   // return from inside
//   auto sp = get_widget3();
//   auto sp2 = sp->mfunc();
// }

int main() {
  // weak ptr
  std::weak_ptr<Widget> wp;
  {
    auto sp = std::make_shared<Widget>();
    wp = sp;
    auto sp2 = wp.lock();
  }
  if(!wp.expired()){
     auto sp = wp.lock();
     sp->m = 5;
  }
}
