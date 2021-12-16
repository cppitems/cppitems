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

  bool shutdown = false;

  // prepare list of widgets
  std::list<Widget> work_items;
  for (auto i : {1, 2, 3, 4, 5}) {
    work_items.push_back(Widget{i});
  }

  auto base_task = [&work_items, &shutdown]() {
    while (true) {
      Widget current;

      std::unique_lock<std::mutex> l(m); // get lock 

      v.wait(l, [&work_items,
                 &shutdown] { // release lock already and wait with predicate:
                              // all threads "sit here and idle"
        return !work_items.empty() ||
               shutdown; // wakeup when work to do or shutdown signal
      });

      if (!shutdown) { // -> do seme work
        current = std::move(work_items.front());
        work_items.pop_front();
        std::cout << "working on widget: "<< current.m  << std::endl;
        // perform work on current Widget here
      }

      if (shutdown) {
        std::cout << "shutdown signal" << std::endl;
        return; // end thread
      }

    }
  };

  // vector of futures/results
  const int num_workers = 10;
  std::vector<std::future<void>> handles;
  for (std::size_t i = 0; i < num_workers; ++i) {
    handles.push_back(std::async(std::launch::async, base_task));
  }

  while (!work_items.empty()) {
    v.notify_one();
  }

  // shutdown
  shutdown = true;
  v.notify_all();
}