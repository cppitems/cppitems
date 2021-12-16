#include <cassert>
#include <chrono>
#include <future>
#include <thread>
#include <iostream>

int main() { // future and promise w/o thread
  auto promise = std::promise<int>();
  auto future = promise.get_future();
  {
    auto status = future.wait_for(std::chrono::milliseconds(1));
    assert(std::future_status::timeout == status);
  }
  promise.set_value(2);
  {
    auto status = future.wait_for(std::chrono::milliseconds(1));
    assert(std::future_status::ready == status);
    future.wait(); // blocking
    auto value = future.get(); // get 2
  }
}

// int main() { // using a thread to fullfill the promise
//   auto promise = std::promise<int>();
//   auto future = promise.get_future();
//   auto callable = [&promise]() {
//     std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     promise.set_value(4);
//     // promise.set_value_at_thread_exit(6); // exception
//   };
//   std::thread t(std::move(callable));
//   {
//     auto status = future.wait_for(std::chrono::milliseconds(1));
//     assert(std::future_status::timeout == status);
//   }
//   {
//     future.wait();             // blocking
//     auto value = future.get(); // get 4
//     // auto value2 = future.get(); // exception
//   }
//   t.join();
// }

// int main() { // using packaged_task (return is wrapped with a future)
//   auto callable = []() {
//     std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     return 6;
//   };
//   auto task = std::packaged_task<int()>(std::move(callable));
//   auto future = task.get_future();
//   std::thread t(std::move(task));
//   {
//     auto status = future.wait_for(std::chrono::milliseconds(1));
//     assert(std::future_status::timeout == status);
//   }
//   {
//     future.wait();             // blocking
//     auto value = future.get(); // get 6
//     std::cout << value << std::endl;
//   }
//   t.join();
// }