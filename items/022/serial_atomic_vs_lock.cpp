
#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <vector>

template <typename CALLABLE, typename... ARGS>
auto track_time(const std::string &name, int iter, CALLABLE &&func,
                ARGS &&... args) {
  using Clock = std::chrono::steady_clock;
  using Duration = std::chrono::duration<double>;
  using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
  func(std::forward<ARGS>(args)...); // warmup
  TimePoint start = Clock::now();
  for (int i = 0; i < iter; ++i)
    func(std::forward<ARGS>(args)...);
  TimePoint stop = Clock::now();
  auto timespan = Duration(stop - start).count() / (iter - 1);
  std::cout << name << ": " << std::scientific << timespan << "s" << std::endl;
}

int main() {
  int N = 1'000'000;
  int iter = 3;
  { // no synchronization
    std::vector<int> vec(N, 1.0);
    int sum = 0;
    auto accumulate = [&sum, &vec]() {
      for (auto &&item : vec) {
        sum = sum + 1;
      }
    };
    track_time("no sync", 3, accumulate);
    std::cout << sum << std::endl;
  }
  { // using single atomic<int>
    std::vector<int> vec(N, 1.0);
    std::atomic<int> sum(0.0);
    auto accumulate = [&sum, &vec]() {
      for (auto &&item : vec) {
        sum = sum + 1; // OK
        // sum.fetch_add(1); // equivalent
      }
    };
    track_time("atomic", 3, accumulate);
    std::cout << sum << std::endl;
  }
  { // using locked region
    std::vector<int> vec(N, 1.0);
    std::mutex m;
    int sum = 0;
    auto accumulate = [&m, &sum, &vec]() {
      for (auto &&item : vec) {
        std::lock_guard<std::mutex> lock(m);
        sum += 1;
      }
    };
    track_time("lock", 3, accumulate);
    std::cout << sum << std::endl;
  }
}