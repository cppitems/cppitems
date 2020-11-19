#include <utility>


template <typename T> struct DebugType { using T::notexisting;};
// usage: DebugType<decltype(...)> error;

template <typename /*f4*/ AUTO> 
struct Widget {
   Widget(/*b4*/ AUTO&& arg) {};
};

int main(){
    double lval =1.0;
    double &lref = lval;
    // Widget w1(lref); // DebugType<decltype(w2)> error
    Widget w2(std::move(std::as_const(lref))); // DebugType<decltype(w2)> error   
    // Widget w3(std::as_const(lref)); // DebugType<decltype(w3)> error
    Widget w4(std::move(lref)); // DebugType<decltype(w4)> error
    Widget w5(1.0); // DebugType<decltype(w4)> error
}