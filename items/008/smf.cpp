#include <functional>
#include <iostream>
#include <memory>
#include <vector>

static std::string log;

struct Log {
  Log() { log += "def-ctor "; }
  ~Log() { log += "dtor "; }
  Log(const Log &other) { log += "copy-ctor "; }
  Log(Log &&other) { log += "move-ctor "; }
  Log &operator=(const Log &rhs) {
    log += "copy-assign ";
    return *this;
  }
  Log &operator=(Log &&rhs) {
    log += "move-assign ";
    return *this;
  };
};

struct dummy {};

int main() {

  struct Widget : dummy {
    Log log;
  };

  auto record_smf = [](auto callable) {
    log = {};
    callable();
    std::cout << log << std::endl;
  };

  record_smf([]() {
    Widget a{};                          // (1a) def-ctor
    Widget b = Widget{};                 // (1b)
    Widget c(a);                         // (2a) copy-ctor
    Widget d = a;                        // (2b)
    Widget e(std::move(a));              // (3a) move-ctor
    Widget f = std::move(a);             // (3b)
    a = b;                               // (4) copy-assign
    a = std::move(b);                    // (5) move-assign
    auto pass_byvalue = [](Widget w) {}; //
    pass_byvalue(a);                     // (6)
    auto pass_lref = [](Widget &w) {};   //
    pass_lref(a);                        // (7)
    auto pass_rref = [](Widget &&w) {};  //
    pass_rref(std::move(a));             // (8)
  });
}