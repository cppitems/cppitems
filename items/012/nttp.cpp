template <typename T, int N> 
struct Widget { T data[N]; };

template <typename T, bool option = false> struct Widget2 {
  T calculate() {
    if constexpr (option) {
      return T{0};
    } else {
      return T{1};
    }
  }
};

int main() {
  Widget<double, 10> w10;
  Widget<double, 100> w100;
  Widget2<double> wf;
  Widget2<double,true> wt;
}