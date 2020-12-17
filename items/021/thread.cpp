#include <chrono>
#include <future>
#include <iostream>
#include <memory>
#include <thread>
#include <type_traits>
#include <utility>

struct jthread {
  std::thread t;
  template <class... Args>
  explicit jthread(Args &&... args) : t(std::forward<Args>(args)...) {}
  ~jthread() { t.join(); }
};

int main() {
  {                 // construction
    std::thread t1; // not yet a thread

    { // callable + args
      auto callable = [](int a, int b) {
        std::cout << a + b << std::endl;
        return a + b;
      };
      int arg1 = 1;
      int arg2 = 1;
      std::thread thread(callable, arg1, arg2);
      thread.join();
    }
    { // callable + ref args
      auto callable = [](int &a, int &b) {
        std::cout << a + b << std::endl;
        return a + b;
      };
      int arg1 = 2;
      int arg2 = 2;
      std::thread thread(callable, std::ref(arg1), std::ref(arg2));
      thread.join();
    }
    { // joining on destruction
      auto callable = [](int &a, int &b) {
        std::cout << a + b << std::endl;
        return a + b;
      };
      int arg1 = 3;
      int arg2 = 3;
      jthread(callable, std::ref(arg1), std::ref(arg2));
    }
    { // package task
      auto callable = [](int &a, int &b) {
        return a + b;
      };
      int arg1 = 4;
      int arg2 = 4;
      auto task = std::packaged_task<int(int &, int &)>(std::move(callable));
      auto future = task.get_future();
    //   task(arg1,arg2); // using w/o thread
      jthread(std::move(task), std::ref(arg1), std::ref(arg2));
      std::cout << future.get() << std::endl ;
    }
  }
}