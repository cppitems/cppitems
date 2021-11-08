struct Widget {
  int i;
  Widget operator+(const Widget &other) {
    i += other.i;
    return *this;
  }
};



// regular function for double
double add(double &a, double &b) { return a + b; };

// regular function for float
using T = float;
T add(T &a, T &b) { return a + b; }

// template definition
template <typename T> 
T add(int x) {  // start scope of T
    return T{}; 
} // end scope of T

// compiler generates this for you
Widget add(Widget &a, Widget &b) { return (a + b); } // will this work? -> fail for non-compatible types
// requirements for Widget:
// 1. operator+ available between two widgets: where found? -> as member or free function
// 2. operator must return something which can be converted to a Widget (return type)

struct Other {

};

int main(){
    Widget a;
    Widget b;
    add<int>(a,b); // use your template from above -> deducing T from argument provided -> T = Widget
}

// full template specialization: definition for T=int
template <> int add<int>(int &a, int &b) { return a + b; };

// overload for int
int add(const int &a,const int &b) { return a + b; };

// request for explicit instantiation for T=double
template double add(double &a, double &b);

// request for explicit instantiation for T=Widget
template Widget add(Widget &a, Widget &b);

#include <utility>
int main() {
  int a;
  int b;
  std::swap(a, b);
  auto cw = add(a, b);
  auto cint = add(1, 2);
  //  auto cint = func(1,2);
}