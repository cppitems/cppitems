#include <initializer_list>
struct Widget {
  int m;
};

template <typename AUTO> void func(AUTO& arg){
    AUTO tmp{}; 
};
// specializations have to match "deductions types"
template <> void func<Widget>(Widget& arg){};
template <> void func<const Widget>(const Widget& arg){};

template <typename AUTO> void fref(AUTO&& arg){
    // AUTO tmp{}; 
};
// specializations have to match "deductions types"
template <> void fref<Widget &>(Widget& arg){};
template <> void fref<Widget>(Widget&& arg){};
template <> void fref<Widget &&>(Widget&& arg){};

int main() {
  Widget expr{1};    //
  func(expr);        // type of arg
  auto arg = (expr); // type of arg
}

