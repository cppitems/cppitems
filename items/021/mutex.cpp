#include <atomic>
#include <cassert>
#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main() {
  { // mutex only
    std::mutex m;
    std::vector<double> shared_data;
    auto manip = [&m, &shared_data]() {
      m.lock();
      // manipulate shared_data
      m.unlock();
    };
  }
  { // lock_guard
    std::mutex m;
    std::vector<double> shared_data;
    auto manip = [&m, &shared_data]() {
      std::lock_guard<std::mutex> lock(m);
      // manipulate shared_data
    };
  }
  { // double mutex lock
    std::mutex m1;
    std::mutex m2;
    std::vector<double> shared_data1;
    std::vector<double> shared_data2;
    auto manip = [&m1, &m2, &shared_data1, &shared_data2]() {
      std::unique_lock<std::mutex> dlock1(m1, std::defer_lock);
      std::unique_lock<std::mutex> dlock2(m1, std::defer_lock);
      std::lock(dlock1, dlock2); 
      // manipulate shared_data1 and shared_data2 together
    };
  }
}