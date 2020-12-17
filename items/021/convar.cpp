#include <condition_variable>
#include <future>
#include <iostream>
#include <list>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

struct Widget {
  int m;
};

std::mutex m;
std::condition_variable v;

int main() {
  std::vector<std::future<void>> handles;
  bool shutdown = false;
  std::list<Widget> work_items;
  for (auto i : {1, 2, 3, 4, 5}) {
    work_items.push_back(Widget{i});
  }

  auto base_task = [&work_items, &shutdown]() {
    while (true) {
      Widget current;
      {
        std::unique_lock<std::mutex> l(m);   // get lock
        v.wait(l, [&work_items, &shutdown] { // release lock and wait for notify
          return !work_items.empty() || shutdown;
        });
        if (!shutdown) {
          current = std::move(work_items.front());
          work_items.pop_front();
        }
      }
      if (shutdown) {
        std::cout << "shutdown" << std::endl;
        return;
      }
      std::cout << current.m << std::endl;
    }
  };

  for (std::size_t i = 0; i < 10; ++i) {
    handles.push_back(std::async(std::launch::async, base_task));
  }

  while (!work_items.empty()) {
    v.notify_one();
  }

  // shutdown
  shutdown = true;
  v.notify_all();
}