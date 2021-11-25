#include <functional>
#include <iostream>

struct Widget {
  int i;
};

int main() {

  {
    struct Widget {
      int m;
      auto member() {
        return [=]() { return m; };
      }
    };
    auto lambda = Widget{9}.member();
    std::cout << lambda() << std::endl;
  }

  {
    double g = 3;
    const int i{2};
    const Widget w{3};
    auto lambda = [=, &w](int b) { return g + i + w.i + b; };
    // auto lambda_copy = lambda; // copy ctor
    // auto lambda_move = lambda; // move ctor
    // lambda_copy = lambda; // error: copy assignment
    // lambda_copy = std::move(lambda); // error: move->copy assignment
  }

  {
    int i{};
    Widget w{};
    class ClosureType {
    private:
      int i;
      Widget &w;

    public:
      ClosureType() = delete;
      ClosureType(int i, Widget &w) : i(i), w(w){};
      ClosureType(const ClosureType &) = default;
      ClosureType(ClosureType &&) = default;
      ClosureType &operator=(const ClosureType &) = delete;
      // ClosureType &operator=(ClosureType &&); // not declared
      ~ClosureType() = default;
      auto operator()(int b) const -> int { return i + w.i + b; }
    } lambda(i, w);
    int res = lambda(4);
  }
}