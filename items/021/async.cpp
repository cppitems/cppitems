#include <chrono>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>
#include <utility>

std::mutex m_cout;

int main() {

  auto callable = [](int N, const std::string &str) {
    for (int i = 0; i < N; ++i)
      std::cout << str << std::endl;
  };
  int arg1 = 3;
  { // handle of async block on destruction
    auto f1 = std::async(callable, arg1, "default");
    auto f2 = std::async(std::launch::deferred, callable, arg1, "deferred");
    auto f3 = std::async(std::launch::async, callable, arg1, "async");
    auto f4 = std::async(std::launch::async, callable, arg1, "async2");
    f4.wait();
    f3.wait();
    f1.wait();
    f2.wait(); // deferred one
    // this futures here are anyway blocking in their destructors (only for std::async )
  }
  { // temporay object (future) block on destruction
    std::async(callable, arg1, "never ...");
    std::async(callable, arg1, "... async");
  }
}