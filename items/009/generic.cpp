struct Widget {
  int i;
};

class ClosureType {
private:
  int i;    // (1a) capture by-value (default)
  Widget w; // (1b) capture by-value (default)

public:
  ClosureType(int i, Widget &w) : i(i), w(w){}; // (1)
  template <typename AUTO> auto operator()(AUTO b, double g) {
    return g + w.i + b + i;
  } // (2)
};

int main() {
  {
    const int i{2};
    Widget w{3};
    // generic lambda
    auto lambda = [=](auto b, double g) { return g + w.i + b + i; };
    auto lambda1 = [=](auto &b, double g) { return g + w.i + b + i; };
    auto lambda2 = [=](auto &&b, double g) { return g + w.i + b + i; };
  }
  {
    const int i{2};
    Widget w{3};
    ClosureType lambda(i, w); // (3)
    int res = lambda(4, 4);   // (4)
  }
}