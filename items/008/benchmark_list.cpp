#include "list.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>

using ListD = List<double>;

// 'main' measures the runtime for move/copy construction and assignment
int main() {

  // { // example usage of list
  //   ListD lst;
  //   size_t n = 10;
  //   for (size_t i = 0; i < n; ++i) {
  //     lst.push_front(i);
  //   }
  //   ListD::Node *iter = lst.data();
  //   for (unsigned i = 0; i < lst.size(); ++i) {
  //     std::cout << iter->value << std::endl;
  //     iter = iter->next;
  //   }
  //   for (size_t i = 0; i < n; ++i) {
  //     lst.pop_front();
  //   }
  //   for (unsigned i = 0; i < lst.size(); ++i) {
  //     std::cout << iter->value << std::endl;
  //     iter = iter->next;
  //   }    
  // }

  size_t size = 100'000; // vector length 0.1M == ~0.7MB
  size_t n = 10;         // iterations for averaging runtime

  auto testMoveCtor = [size]() {
    ListD var = ListD(size, std::rand());
    const auto tmp = var.data();
    ListD w = std::move(var); // invoking move-ctor
    if (var.data() != nullptr || w.data() != tmp) {
      std::cout << "List was not move constructed, this can be improved!"
                << std::endl;
    }
  };
  auto testMoveAssign = [size]() {
    ListD var = ListD(size, std::rand());
    const auto tmp = var.data();
    ListD w;
    w = std::move(var); // invoking move-assign
    if (var.data() != nullptr || w.data() != tmp) {
      std::cout << "List was not move assigned, this can be improved!"
                << std::endl;
    }
  };
  auto testCopyCtor = [size]() {
    ListD var = ListD(size, std::rand());
    ListD w = var; // invoking copy-ctor
    if (w.data() == nullptr) {
      std::cout << "List was moved from lvalue, should not happen!"
                << std::endl;
    }
    if (var.size() != w.size()) {
      std::cout << "Lists of not equal size or capacity, should not happen!"
                << std::endl;
    }
    auto varNode = var.data();
    auto wNode = w.data();
    for (unsigned i = 0; i < var.size(); ++i) {
      if (varNode->value != wNode->value) {
        std::cout << "List elements " << i
                  << " are not the same, should not happen!" << std::endl;
      }
      varNode = varNode->next;
      wNode = wNode->next;
    }
  };
  auto testCopyAssign = [size]() {
    ListD var = ListD(size, std::rand());
    ListD w;
    w = var; // invoking copy-assign
    if (w.data() == nullptr) {
      std::cout << "List was moved from lvalue, should not happen!"
                << std::endl;
    }
    if (var.size() != w.size()) {
      std::cout << "Lists of not equal size or capacity, should not happen!"
                << std::endl;
    }
    auto varNode = var.data();
    auto wNode = w.data();
    for (size_t i = 0; i < var.size(); ++i) {
      if (varNode->value != wNode->value) {
        std::cout << "List elements " << i
                  << " are not the same, should not happen!" << std::endl;
      }
      varNode = varNode->next;
      wNode = wNode->next;
    }
  };

  auto runtest = [n](auto &&test) {
    using Clock = std::chrono::steady_clock;
    using Duration = std::chrono::duration<double>;
    auto start = Clock::now();
    for (size_t i = 0; i < n; ++i) {
      test();
    }
    auto stop = Clock::now();
    return Duration(stop - start).count() / n;
  };

  auto timeMoveCtor = runtest(testMoveCtor);
  auto timeCopyCtor = runtest(testCopyCtor);
  auto timeMoveAssign = runtest(testMoveAssign);
  auto timeCopyAssign = runtest(testCopyAssign);

  std::cout << std::setw(18) << "Move-Ctor took:" << std::scientific
            << timeMoveCtor << " s" << std::endl;
  std::cout << std::setw(18) << "Copy-Ctor took:" << std::scientific
            << timeCopyCtor << " s" << std::endl;
  std::cout << std::setw(18) << "Ctor-Speedup:" << std::fixed
            << std::setprecision(2) << (timeCopyCtor / timeMoveCtor) << "x"
            << std::endl;

  std::cout << std::setw(18) << "Copy-Assign took:" << std::scientific
            << timeCopyAssign << " s" << std::endl;
  std::cout << std::setw(18) << "Move-Assign took:" << std::scientific
            << timeMoveAssign << " s" << std::endl;
  std::cout << std::setw(18) << "Assign-Speedup:" << std::fixed
            << std::setprecision(2) << (timeCopyAssign / timeMoveAssign) << "x"
            << std::endl;
}