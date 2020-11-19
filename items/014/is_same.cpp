#include <iostream>

struct Flase {
  static const bool value = false;
};
struct True {
  static const bool value = true;
};
template <class A, class B>
struct is_same : Flase {}; // primary template, two parameters

template <class C>
struct is_same<C, C> : True {}; // partial specialization, one parameter left

int main(){
    std::cout << is_same<double,double>::value << std::endl;
    std::cout << is_same<double,float>::value << std::endl;
}