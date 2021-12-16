#include <atomic>
#include <cassert>
#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

// struct Other { // using atomic
//   std::atomic<int> a{5};
//   std::atomic<int> b{5};
// };

// struct Widget {
//   Other o;
//   void mod1() {
//       --o.a;
//       ++o.b;
//     // o.a = o.a - 1; // also OK?
//     // o.b = o.b + 1; // also OK?
//   }
//   void mod2() {
//       ++o.a;
//       --o.b;
//     // o.a = o.a + 1;  // also OK?
//     // o.b = o.b - 1;  // also OK?
//   }

//   int inspect() const { return o.a + o.b; }
// };

struct Other {
  int a = 5;
  int b = 5;
}; // a+b is always 10;

struct Widget { // unsynchronized
  Other o;
  void mod1() {
    --o.a;
    ++o.b;
  }
  void mod2() {
    ++o.a;
    --o.b;
  }
  int inspect() const { return o.a + o.b; }
};

// struct Other {
//   int a = 5;
//   int b = 5;
// };              // a+b is always 10;
// struct Widget { // using two lock_guards
//   std::mutex m_mod1;
//   std::mutex m_mod2;
//   Other o;
//   void mod1() {
//     const std::lock_guard<std::mutex> lock(m_mod1);
//     --o.a;
//     ++o.b;
//   }
//   void mod2() {
//     const std::lock_guard<std::mutex> lock(m_mod2);
//     ++o.a;
//     --o.b;
//   }
//   int inspect() const { return o.a + o.b; }
// };

// struct Other {
//   int a = 5;
//   int b = 5;
// };              // a+b is always 10;
// struct Widget { // using single lock_guard
//   std::mutex m_mod;
//   Other o;
//   void mod1() {
//     const std::lock_guard<std::mutex> lock(m_mod);
//       --o.a;
//       ++o.b;
//   }
//   void mod2() {
//     const std::lock_guard<std::mutex> lock(m_mod);
//       ++o.a;
//       --o.b;
//   }
//   int inspect() const { return o.a + o.b; }
// };

int main() {
  Widget w;
  using Clock = std::chrono::steady_clock;
  using Duration = std::chrono::duration<double>;
  using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

  { // introduce threads wich use mod1 and mod2

    auto a1 = std::async(std::launch::async, [&w]() {
      TimePoint start = Clock::now();
      double timespan = 0;
      while (timespan < 2.0) {
        w.mod2();
        TimePoint stop = Clock::now();
        timespan = Duration(stop - start).count();
      }
    });
    auto a2 = std::async(std::launch::async, [&w]() {
      TimePoint start = Clock::now();
      double timespan = 0;
      while (timespan < 2.0) {
        w.mod1();
        TimePoint stop = Clock::now();
        timespan = Duration(stop - start).count();
      }
    });
  }

  //   if (w.inspect() != 10) {
  std::cout << w.o.a << "|" << w.o.b << ",  sum = " << w.o.a + w.o.b << std::endl;
  //   }
}